# ft_irc — Security & Completeness Audit

Scope: `includes/` + `src/` (26 files, ~2325 lines, C++98 / epoll). 33 findings, ordered by folder → subfolder → file, then split into 3 balanced parts for the team. Each finding: what it is, why it matters, how to fix it.

Severity tags: 🔴 Critical (crash/auth-bypass/privilege-escalation) · 🟠 High (DoS/data-corruption) · 🟡 Medium (protocol/logic gap) · ⚪ Low (hygiene/robustness)

---

## PART 1 — Core, Connection Lifecycle & Session State
*(includes/Server.hpp, includes/User.hpp, includes/Parser.hpp, src/main.cpp, src/core/)*

### includes/Server.hpp
- 🟡 **No cap on simultaneous connections or per-IP connections.**  
  There's no field or check limiting how many clients (total or per source) can connect at once.
  A single peer can open sockets until the process hits its file-descriptor limit, starving legitimate users.
  → Add a `MAX_CLIENTS` constant and reject `accept()` past it; optionally track a `map<ip,count>` for a per-IP cap.

### includes/Parser.hpp
- ⚪ **Parser is entirely static/shared, mutable global state (`_prefix`, `_command`, `_parameters`…).**  
  This works only because the loop is single-threaded and strictly sequential; it's a latent trap the moment anyone parallelizes I/O.
  It also makes the parser impossible to unit-test in isolation from the rest of the program.
  → Make `Parser::parse()` return a `ParsedMessage` value/struct instead of stashing results in static members.

### src/main.cpp
- 🟠 **`server.init()` / `server.startLoop()` aren't wrapped in try/catch.**  
  Both throw `std::runtime_error` on socket/bind/listen/epoll failures, and `startLoop()` can propagate exceptions from deep inside command handling.
  Any uncaught exception terminates the whole server, dropping every connected client at once.
  → Wrap the two calls in a top-level `try { ... } catch (const std::exception&)` that logs and exits cleanly (or restarts).

- 🟡 **No signal handling (SIGINT/SIGTERM).**  
  `Server::stop()` closes every fd and clears state, but it only runs from the destructor — which a `kill`/Ctrl-C won't trigger.
  Restarting the server after a hard stop can hit `EADDRINUSE`-style issues and leaves clients with sockets that never got a clean close.
  → Install a signal handler that sets an `sig_atomic_t` flag, checked each loop iteration, to call `stop()` and exit gracefully.

- ⚪ **Server password is passed as a plaintext CLI argument (`av[2]`).**  
  Any local user on the box can read it from `ps aux` or `/proc/<pid>/cmdline` while the server runs.
  This is a standard secrets-handling anti-pattern for anything meant to gate access.
  → Accept the password via an env var or a restricted-permission config file instead of argv, if the subject allows it.

### src/core/Server.cpp
- 🔴 **`removeUser()` never removes the user from the channels they were in.**  
  It only erases the entry from `_users` and closes the fd — `Channel::_members` / `_operators` / `_invitedUsers` still hold the raw `User*` for that fd.
  The very next broadcast/kick/mode touching that channel dereferences a pointer into freed memory (use-after-free), which is a crash and a potential exploitation primitive.
  → Before erasing from `_users`, iterate `user->getJoinedChannels()` and call `channel.removeMember(&user)` on each; delete now-empty channels too.

- 🟠 **`inputBuffer` has no size ceiling while waiting for `\r\n`.**  
  A client can stream bytes forever without ever sending a line terminator, and `Server.cpp` keeps `.append()`-ing to `User::inputBuffer` unbounded.
  One connection is enough to grow server memory without limit — a trivial single-socket DoS.
  → Enforce the RFC-1459 512-byte line cap; if `inputBuffer` exceeds it before a `\r\n`, disconnect the client (or drop and resync).

- 🟠 **No cap on accepted connections (fd exhaustion).**  
  `startLoop()` calls `accept()` unconditionally whenever `_serverFd` fires — there's no check against a maximum client count.
  A connect-flood exhausts the process's file descriptors, taking down the server for every user, not just the attacker.
  → Track `_users.size()`; once at the limit, `accept()` then immediately `close()` the new fd instead of registering it.

- 🟡 **The client's real IP/hostname is never captured.**  
  `accept()`'s `sockaddr_in clientAddr` is filled in and then discarded — `User::_hostname` stays hardcoded to `"127.0.0.1"` for every connection (see `User.cpp`).
  This kills any hope of per-IP bans, abuse tracing, or accurate `nick!user@host` prefixes sent to clients.
  → Call the already-written (but unused) `NetworkUtils::getHostname(clientAddr)` right after `accept()` and store it via `user.setHostname(...)`.

### src/core/Channel.cpp
- 🟡 **Copy constructor/assignment shallow-copy raw `User*` members with no ownership contract.**  
  Two `Channel` copies can end up pointing at the same `User` objects with nothing tracking who "owns" the pointer or when it becomes invalid.
  Combined with the dangling-pointer bug above, this makes it easy to reintroduce use-after-free bugs anywhere a `Channel` gets copied (e.g. the temporary channels built by the parser).
  → Document/enforce that `Channel` never outlives the `User`s it references, or switch `_members`/`_operators`/`_invitedUsers` to store fds only and resolve through `Server::getUserById()`.

### src/core/User.cpp
- 🟠 **`joinChannel()` / `leaveChannel()` are defined but never called anywhere in the codebase.**  
  `User::_joinedChannels` is therefore always empty — no code path actually keeps it in sync with real channel membership.
  This is the root cause enabling the dangling-pointer bug above (there's no reliable way to know "which channels does this user need cleanup from"), and it silently breaks any future feature relying on it (e.g. `/list`, `WHOIS`).
  → Call `user->joinChannel(name)` in every successful `JOIN` path and `user->leaveChannel(name)` in every `PART`/`KICK`/disconnect path; then use it to drive the `removeUser()` fix above.

---

## PART 2 — Parsing, Command Dispatch & Channel Join/Leave Flow
*(includes/Replies.hpp cross-refs, src/parser/, src/commands/Join.cpp, Part.cpp, Kick.cpp)*

### src/parser/dispatcher/dispatcher.cpp
- **_[FIXED]_** ~~🔴 **The `PASS` command's password check is commented out.**~~   
  `userCommandsDispatch()` sets `hasProvidedPassword(true)` unconditionally on any non-empty `PASS` argument; the line comparing it to `Server::getPassword()` is dead code.
  This means the server-wide password protecting the whole IRC network is not enforced at all — total authentication bypass for every connecting client.
  → Restore the comparison (fix it to call the member function on the actual `Server` instance, not `Server::` statically), reject with `ERR_PASSWDMISMATCH` (464) on mismatch, and use a constant-time string compare to avoid timing side-channels.

- **_[FIXED]_** ~~🟠 **No registration gating — channel/message commands work before authentication completes.**~~  
  `dispatchCommand()` routes straight into `channelCommandsDispatch`/`messageCommandsDispatch` regardless of `user.isAuthenticated()`; `ERR_NOTREGISTERED` (451) is defined in `Replies.hpp` but never referenced.
  An unauthenticated socket can `JOIN`, `KICK`, or `PRIVMSG` immediately, defeating the whole point of the password gate (and of PASS/NICK/USER ordering in general).
  → At the top of `dispatchCommand()`, reject with 451 any command outside `{pass, nick, user}` when `!user.isAuthenticated()`.

- **_[FIXED]_** ~~⚪ **Command errors are only logged server-side (`std::cerr`), not consistently surfaced to the client.**~~  
  Several throw sites (e.g. empty `NICK`/`USER` parameters) never call `notification()` before throwing, so the client silently gets nothing back.
  A real client has no way to know its command was rejected or why, which makes the server feel broken/unresponsive during testing.
  → Ensure every throw site in the three dispatch functions sends a matching numeric reply first.

### src/parser/Parser/Parser.cpp
- **_[FIXED]_** ~~🟠 **No nickname validation and no duplicate-nickname check.**~~  
  `NICK` accepts any string as-is; two clients can hold the identical nickname, and `getUserByNickname()` just returns the first match it iterates to.
  That ambiguity lets a second client "shadow" an existing nick — private messages or kicks aimed at one user can resolve to the wrong socket, which is an impersonation/message-interception risk, not just a cosmetic bug.
  → Validate against RFC nick-name grammar and reject with `ERR_ERRONEUSNICKNAME` (432); reject duplicates with `ERR_NICKNAMEINUSE` (433) via a lookup before accepting the new nick.

- 🟡 **`checkNameChannel()` / `getlistChannel()` don't handle empty or malformed names safely.**  
  `checkNameChannel` indexes `nameChannel[0]` without checking the string isn't empty, and a channel list like `"#a,,#b"` silently produces an empty-named `Channel`.
  Depending on standard-library behavior this is undefined access on an empty string, and empty-named channels can end up stored in `_channels`, corrupting later lookups.
  → Guard with `if (nameChannel.empty()) return false;` and skip zero-length tokens when splitting comma lists.

### src/parser/Parser/ParserInit.cpp
- 🟡 **No `QUIT` command and no `PING`/`PONG` keep-alive support.**  
  The command tables only cover join/kick/invite/topic/mode/part, privmsg/notice(unhandled), and user/nick/pass — there's no graceful disconnect command and no liveness check.
  Dead or NAT-dropped connections never get cleaned up (no way to detect them), slowly leaking fds and memory, and other clients never see a proper "X has quit" message.
  → Add `quit` to `_commandsUser` (broadcast a quit message, then `removeUser()`), and implement periodic `PING`/expect-`PONG` with a timeout-based disconnect.

### src/commands/Join.cpp
- 🟠 **No limit on how many channels one user can join.**  
  `ERR_TOOMANYCHANNELS` (405) is defined in `Replies.hpp` but never used anywhere — a single client can `JOIN` an unbounded number of channels.
  Each joined channel consumes memory and grows every broadcast fan-out for that user, so this is a straightforward resource-exhaustion vector.
  → Track a per-user join count and reject further `JOIN`s past a reasonable cap (e.g. 10–20) with 405.

- ⚪ **Off-by-one inconsistency between the two `join()` overloads for `+l`.**  
  The no-key overload rejects at `getMemberCount() >= getUserLimit()`, the keyed overload rejects at `> getUserLimit()` — the keyed path lets a channel exceed its own limit by one member.
  This is a small correctness gap, but it means the `+l` mode isn't reliably enforced depending on which join path a client takes.
  → Use the same `>=` comparison in both overloads.

- ⚪ **`broadcast()` fires before `addMember()` in the keyed-join path.**  
  The joining member is notified of their own join *before* they're actually in `_members`, and other members receive the message off-order relative to the actual state change.
  It's mostly cosmetic today, but it's the kind of ordering bug that turns into a real race once writes become asynchronous (see the unused `NetworkBuffer` in Part 3).
  → Call `addMember()` first, then `broadcast()`, consistently in both overloads.

### src/commands/Part.cpp
- 🔴 **`part(vector<Channel>&, User*)` dereferences `_channels.find()` without checking for `end()`.**  
  If a client sends `PART` for a channel name that doesn't exist in `_channels`, `getChan->second` dereferences the end iterator directly.
  This is undefined behavior reachable from a single, unauthenticated, one-line client message — a trivial remote crash/DoS.
  → Add `if (getChan == _channels.end()) { notification(...ERR_NOSUCHCHANNEL 403...); throw ...; }` before touching `getChan->second`, matching the pattern already used in `Kick.cpp`/`Mode.cpp`.

- 🟠 **`part(vector<Channel>&, std::string reason, User*)` operates on throwaway local `Channel` copies instead of the real channel in `_channels`.**  
  The `it` iterator here comes straight from `Parser::getlistChannel()` — a freshly-constructed `Channel` with no members ever added — so `it->isMember(...)` is always false and the reasoned-PART feature can never succeed for a real user.
  If this gets "fixed" naively by removing the `isMember` guard, the code would then call `it->getMembers().empty()` (always true on the local copy) and `_channels.erase(it->getName())`, deleting the *real* channel even while it still has members — turning a broken feature into a data-loss bug.
  → Rewrite this overload to `_channels.find(it->getName())` first (exactly like the other `part()` overload) and operate on that entry, not on the parser's temporary object.

### src/commands/Kick.cpp
- 🟡 **The kicked user's `_joinedChannels` list isn't updated, and the reply code is semantically wrong.**  
  `removeMember()` cleans up the `Channel` side, but nothing calls `kicked->leaveChannel(...)`; separately, "user not on channel" replies with `442 ERR_NOTONCHANNEL`, which per RFC means *"you (the sender) aren't on that channel"*, not *"the target isn't"* — that's `441 ERR_USERNOTINCHANNEL`.
  The first leaves user-side state stale (ties into the Part 1 finding on unused `joinChannel`/`leaveChannel`); the second will confuse any real IRC client trying to interpret the reply.
  → Call `kicked->leaveChannel(channel.getName())` alongside `removeMember()`, and swap the reply code to 441.

---

## PART 3 — Permissions, Topic/Mode Enforcement & Network/Output Layer
*(src/commands/Mode.cpp, Topic.cpp, src/network/, src/Message.cpp, src/Replies/)*

### src/commands/Mode.cpp : DONE
- 🔴 **`Server::mode()` never checks that the calling user is a channel operator before applying any mode change.**  
  Any connected client — member or not — can send `MODE #chan +i`/`+t`/`+k`/`+l`/`+o` and it is applied unconditionally; `isOperator()` exists but is never called from this path.
  This is a full privilege-escalation / broken-access-control bug: it lets anyone lock, key, limit, or take over any channel on the server.
  → At the top of `mode()`, look up the channel's operator status for `user->getFd()` and reject with `482 ERR_CHANOPRIVSNEEDED` before calling `launchMode()`.

- 🔴 **`+o` always grants operator to the *sender*, ignoring the target nickname parameter.**  
  Real IRC syntax is `MODE #chan +o <nick>` to op *someone else*; this implementation calls `channel.addOperator(user)` — i.e. it ops whoever typed the command, never resolves `*it_params` to a target user.
  Combined with the missing operator gate above, this means any user can self-promote to operator on any channel with one message, and there is no way to properly op/deop other members at all.
  → Resolve `*it_params` via `server.getUserByNickname(...)`, validate they're a channel member, and call `channel.addOperator()`/`removeOperator()` on *that* user, not the caller.

- 🟡 **The `-` (mode-removal) parsing loop has an off-by-one that walks one character past the last flag.**  
  `split_mode`/`launchMode`'s `-` branch increments `i` before reading, so on a string like `"-it"` it ends up reading the null terminator as if it were a mode character and throws "unknown mode" — after `-i` and `-t` have already been applied.
  This makes multi-flag removals non-atomic: some flags silently take effect while the command as a whole reports failure to the client, leaving channel state and client expectations out of sync.
  → Fix the loop indexing (check-then-increment, not increment-then-check) and validate the whole modestring before applying any flag, so failures are all-or-nothing.

- ⚪ **No validation on the numeric argument to `+l`.**  
  `std::stringstream ss(*it_params); ss >> limit;` doesn't check `ss.fail()` — non-numeric input leaves `limit` uninitialized/garbage before it's passed to `setUserLimit()`.
  A malformed `MODE #chan +l abc` can therefore set an unpredictable channel limit instead of being rejected.
  → Check `ss.fail()` (or use `strtol` and check `endptr`) and reply `461 ERR_NEEDMOREPARAMS` / reject on non-numeric input.

### src/commands/Topic.cpp
- 🟡 **No membership check before reading or setting a channel's topic.**  
  Both `topic()` overloads only gate on `isTopicRestricted()` + operator status when *changing* the topic; there's no check that the calling user is even a member of the channel for either viewing or (when `+t` is off) setting it.
  A non-member can read or overwrite the topic of any channel on the server just by knowing its name.
  → Require `channel.isMember(user->getFd())` (reply `442 ERR_NOTONCHANNEL` otherwise) at the top of both overloads, before any topic access.

### src/network/NetworkBuffer.cpp / NetworkBuffer.hpp
- 🟠 **A fully-implemented outgoing write-buffer (backpressure) class exists but is never instantiated or used anywhere in the codebase.**  
  Real sends all go through raw `send()` calls in `Message.cpp`/`Replies.cpp` instead, on sockets that are explicitly set non-blocking (`fcntl(..., O_NONBLOCK)`).
  Without this buffer (or equivalent `EPOLLOUT` handling), a slow client can cause `send()` to fail/partially-write under load, and that data is simply lost — no queuing, no retry.
  → Wire `NetworkBuffer` into the per-user send path: queue via `queueWriteData()`, register `EPOLLOUT` when `hasPendingWrite()`, and flush on the write-ready event.

### src/network/NetworkUtils.cpp / NetworkUtils.hpp
- 🟡 **`getIpString()` / `getHostname()` are fully implemented but never called.**  
  This is the same root cause as the Part 1 finding on `Server.cpp` not capturing client addresses — the utility to do it correctly already exists and just isn't wired in.
  Without it, the server has no real audit trail of who connected from where, and no way to build IP-based bans, rate limits, or abuse detection later.
  → Call `NetworkUtils::getHostname(clientAddr)` right after `accept()` in `Server::startLoop()` and store the result via `User::setHostname()`.

### src/Message.cpp
- ⚪ **`send()` failures are only logged via `perror()`, never actually handled.**  
  Every broadcast/notification helper checks `send() == -1` and prints an error, but does nothing else — no retry, no marking the connection dead, no removal of a client whose socket has actually failed.
  A client whose fd has gone bad (e.g., abrupt network drop not yet caught by `recv()`) keeps being iterated and "sent to" every broadcast indefinitely, wasting cycles and, if not `MSG_NOSIGNAL`-protected everywhere, risking a `SIGPIPE`.
  → On repeated/fatal send errors (e.g. `EPIPE`, `ECONNRESET`), call the same cleanup path as `removeUser()` for that client.

### src/Replies/Replies.cpp
- 🟡 **`Server::sendReply()` — the only function that builds an RFC-2812-correct numeric reply (`:server CODE nick <text>\r\n`) — is defined but never called anywhere.**  
  Every actual error/reply in the codebase goes through `notification()` with hand-rolled strings like `"473 ERR_INVITEONLYCHAN"`, which is not a valid IRC protocol line — no `:server` prefix, no proper spacing, and the macro name is sent literally instead of a human-readable message.
  Real IRC clients (irssi, WeeChat, HexChat…) parse replies by position and numeric code; they won't recognize these as server replies at all, so anything beyond a raw `nc` test client will show garbled or missing errors.
  → Replace every `notification(user, "<code> <MACRO_NAME>")` call across `commands/` with `sendReply(user, CODE, "<proper trailing text>")`, using the numerics already defined in `Replies.hpp`.

---

## Split summary
| Part | Owner | Files | Findings | Critical |
|---|---|---|---|---|
| 1 | — | `Server.hpp`, `User.hpp`, `Parser.hpp`, `main.cpp`, `core/*` | 12 | 1 (dangling pointer) |
| 2 | — | `parser/*`, `commands/Join.cpp`, `Part.cpp`, `Kick.cpp` | 12 | 2 (PASS bypass, PART crash) |
| 3 | — | `commands/Mode.cpp`, `Topic.cpp`, `network/*`, `Message.cpp`, `Replies/*` | 9 | 2 (MODE priv-esc, self-op) |

Each part is self-contained by directory so whoever picks it up isn't jumping between unrelated files. Fill in the "Owner" column and go — Part 1 and 2 each carry one server-crashing bug, Part 3 carries the privilege-escalation pair, so severity is balanced even though the item count differs slightly.
