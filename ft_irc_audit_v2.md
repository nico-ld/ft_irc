# ft_irc — Follow-up Audit (Fix Verification + New Pass)

Scope: same `includes/` + `src/` tree (now split further: `ServerHelper.cpp`, `ModeHelper.cpp`, `ParserHelper.cpp`, `dispatchChanCmd/dispatchMessage/dispatchUser.cpp` are new files carved out of the old ones). The `bot/` and `dashboard/` additions are new components outside the original audit's scope — not reviewed here in depth; flag if you'd like a separate pass on them, since the bot in particular talks to the server over the network.

**Headline: the team fixed 30 of the 33 original findings — a genuinely strong pass, including all three of the most severe ones (PASS bypass, MODE privilege escalation, dangling-pointer use-after-free).** But the fix for one of them (registration gating) was silently undone by one line elsewhere, and hardening the error handling opened two brand-new single-message crashes. Read the "🔴 New critical" section before anything else.

---

## Part 1 — Fix verification (against the 33 original findings)

✅ = confirmed fixed · ⚠️ = fixed but with a caveat (see New Findings) · ❌ = still open

| # | Original finding | Status |
|---|---|---|
| 1 | No cap on simultaneous/per-IP connections | ❌ still open |
| 2 | Parser static/shared mutable state | ✅ fixed — `Parser` is now a local per-call object |
| 3 | `main.cpp`: no try/catch around `init()`/`startLoop()` | ⚠️ fixed, but see **New-B/C/F**: the single top-level catch means one bad client message can now take the whole server down |
| 4 | No SIGINT/SIGTERM handling | ✅ fixed — `SIGINT` handled, clean `stop()` on shutdown |
| 5 | ~~Password passed as plaintext CLI arg~~ | Subject requirement |
| 6 | `removeUser()` leaves dangling `User*` in channels | ✅ fixed — iterates `getJoinedChannels()` and cleans up each one |
| 7 | Unbounded `inputBuffer` growth (memory DoS) | ✅ ~~still open — no size cap before `\r\n`~~ |
| 8 | No cap on accepted connections (fd exhaustion) | ❌ still open |
| 9 | Real client IP/hostname never captured | ✅ fixed — `NetworkUtils::getHostname()` now called on accept |
| 10 | `Channel` shallow-copies raw `User*` | ❌ still open (lower risk now that #6 is fixed, but still a latent footgun) |
| 11 | `User::joinChannel()/leaveChannel()` dead code | ✅ fixed — called from `Join.cpp` and centrally from `Channel::removeMember()` |
| 12 | `PASS` check commented out (auth bypass) | ✅ fixed — real comparison restored, `ERR_PASSWDMISMATCH` on mismatch |
| 13 | No registration gating before auth | ✅ ~~fixed in `dispatcher.cpp`... but see **New-A**: undone by a line in `Server.cpp`~~ |
| 14 | Errors only logged server-side | ✅ fixed — `sendReply()` used consistently |
| 15 | No nickname validation / duplicate check | ✅ ~~fixed, but see **New-D**: the validation itself has a logic-inversion bug~~ |
| 16 | `checkChannelName`/`getlistChannel` unsafe on empty names | ✅ fixed |
| 17 | No `QUIT`, no `PING`/`PONG` | ⚠️ half-fixed — `QUIT` implemented; `PING`/`PONG` keep-alive still absent |
| 18 | No per-user channel-join cap | ✅ fixed — capped at 15, `ERR_TOOMANYCHANNELS` used |
| 19 | Off-by-one between `join()` overloads for `+l` | ✅ fixed — both use `>=` now |
| 20 | `broadcast()` fired before `addMember()` | ✅ fixed |
| 21 | `PART`: `end()` iterator dereferenced (crash) | ✅ fixed |
| 22 | `PART`: operated on throwaway local `Channel` copies | ✅ fixed — rewritten around `getChannelByName()` |
| 23 | `KICK`: stale `_joinedChannels`, wrong reply code | ✅ fixed — `leaveChannel()` centralized in `removeMember()`, reply codes corrected |
| 24 | `MODE`: no operator check at all (priv-esc) | ✅ fixed — gated at the top of `Server::mode()` |
| 25 | `MODE +o`: always ops the sender, not the target | ✅ fixed — resolves and validates the named target |
| 26 | `MODE -`: off-by-one parsing loop | ✅ fixed — check-then-increment now correct |
| 27 | `MODE +l`: no numeric validation | ✅ fixed — `ss.fail()` and `<= 0` both checked |
| 28 | `TOPIC`: no membership check | ✅ fixed — enforced in `dispatchChanCmd.cpp` before either `topic()` overload is called (note: the comment inside `Topic.cpp` claiming this guard lives there is stale/misleading — harmless, but worth a one-line fix so the next person doesn't go looking for it in the wrong file) |
| 29 | `NetworkBuffer` built but unused | ✅ fixed — fully wired via `queueWrite()`/`flushWrite()` |
| 30 | `NetworkUtils` IP/hostname resolution unused | ✅ fixed |
| 31 | `send()` failures only logged, never handled | ✅ fixed — fatal errors now schedule cleanup, transient ones queue+retry |
| 32 | `sendReply()` (RFC-correct reply) never used | ✅ fixed — used everywhere |

30/33 fixed outright, 2 fixed-but-undermined, 5 still open (including 2 that were reclassified above as "still open" rather than double-counted).

---

## Part 2 — New findings this pass

### 🔴 New critical

- **_[FIXED]_** **`src/core/Server.cpp` — every new connection is marked authenticated immediately on `accept()`, silently reopening the auth bypass that #12/#13 were supposed to close.**
  Right after registering the socket, the accept handler calls `getUserById(clientFd)->setAuthenticated(true);` — before the client has sent `PASS`, `NICK`, or `USER`. `User.hpp`'s own comment says `_isAuthenticated` should "become true only when the top three are true," so this line contradicts the class's own contract.
  The `ERR_NOTREGISTERED` gate added in `dispatcher.cpp` (fix #13) checks exactly this flag — since it's already `true` at connect time, the gate never actually blocks anything. A client can open a raw TCP connection and immediately send `JOIN`, `PRIVMSG`, `KICK`, or `MODE` with zero registration, no password, no nick, nothing.
  → Delete that line. `setAuthenticated(true)` must only be set from the `PASS && NICK && USER` check already correctly implemented in `dispatchUser.cpp`.

- **_[FIXED]_** **`src/commands/ModeHelper.cpp` — `split_mode()` crashes the *entire server* on a MODE string with no leading `+`/`-`.**
  If the flags argument doesn't start with `+` or `-` (e.g. a client sends `MODE #chan l`), the substr-index math computes `pos_tmp - listMode.begin() - 1` as `-1`; passed to `std::string::substr()` as an unsigned `size_t`, that underflows to a huge value and `substr()` throws `std::out_of_range`.
  Nothing between here and `main()` catches per-command exceptions — the only `try/catch` left is the one wrapped around the entire `server.startLoop()` call in `main.cpp` (fix #3). That catch logs the exception and then `main()` returns, ending the process. **One malformed line from any single client takes the whole server down for every connected user.**
  → Validate that the mode-flags parameter starts with `+`/`-` in `dispatchChanCmd.cpp` before calling `server.mode()`, and reply `ERR_UNKNOWNMODE` otherwise; separately, wrap command dispatch itself in a per-command try/catch so no single client can ever crash the shared process again — this is a structural gap, not just a `MODE`-specific one.

- **_[FIXED]_** **`src/parser/dispatcher/dispatchMessage.cpp` — missing `return` after a `PRIVMSG` parameter-count error causes an out-of-bounds vector access.**
  The `parameters.size() == 0` branch sends `ERR_NEEDMOREPARAMS` but doesn't `return`. Execution falls into the next `if (parameters.size() > 1 || !parser.getTrailing().empty())` check — which is `true` for a message like `PRIVMSG :hi` (no target, just trailing text) — and then reads `parameters[0][0]` on an **empty vector**. `std::vector::operator[]` performs no bounds checking; this is undefined behavior, not a catchable exception, so no try/catch anywhere in the call stack will save the process. In practice this is a guaranteed crash/segfault from one client sending one line.
  → Add `return;` right after the `sendReply(... ERR_NEEDMOREPARAMS ...)` call, so the function can never proceed to indexing `parameters[0]` with zero parameters.

### 🟠 New medium

- **_[FIXED]_** **`src/parser/dispatcher/dispatchUser.cpp` — `isNicknameValid()`'s per-character check has inverted logic and rejects legal nicknames containing digits.**
  The condition `!isalpha(c) && isdigit(c) && !strchr(VALID_CHAR, c) && c != '-'` is true for any digit character (a digit is never alpha, and is always a digit) — so any nickname like `user1` or `bob42` is rejected with `ERR_ERRONEUSNICKNAME`, even though digits are explicitly legal in IRC nicknames (just not as the first character, which is already checked separately on the line above). This will confuse real users and real clients during testing/defense, since a very common style of nickname simply won't work.
  → The digit branch shouldn't reject at all — drop the `isdigit(c)` clause from the rejection condition (digits are allowed anywhere except position 0, which is handled by the separate first-character check).

- **_[FIXED]_** **`src/parser/dispatcher/dispatchMessage.cpp` — `NOTICE` is a recognized command that does nothing.**
  `notice` is registered in `ParserInit.cpp`'s `_commandsMessage` list (so it correctly routes into `messageCommandsDispatch`), but the function only has an `if (command == "privmsg")` branch — there's no `else if (command == "notice")` at all. A client sending `NOTICE` gets silent, total non-behavior: no message delivered, no error reply, nothing in the logs pointing at why.
  → Add a `notice` branch mirroring `privmsg`'s logic, with one difference per RFC: `NOTICE` must never generate an automatic error reply back to the sender (that's the whole point of using `NOTICE` for bot/server messages — it avoids reply loops), so skip the `sendReply(... ERR_* ...)` calls on that path and just silently drop invalid `NOTICE`s server-side (log only).

### 🟡 New low / structural

- `main.cpp`'s single top-level try/catch is an architectural single point of failure.**
  This isn't a new bug on its own — it's the mechanism that turns the two crashes above (and any future one like them) into "kill the whole server" instead of "drop one bad connection." Right now, *any* uncaught `std::exception` thrown anywhere during command processing takes down every connected client's session at once, not just the offending one.
  → Wrap the body of `dispatchCommand()` (or the command-execution call inside `Server::startLoop()`) in its own try/catch that logs and calls `removeUser()` (or just drops the malformed line) for that one client, instead of letting exceptions propagate up to `main()`. Keep `main()`'s catch as a last-resort safety net for startup failures only.

- **_[FIXED]_** **`src/core/Server.cpp` still calls `getUserById(clientFd)` twice in the accept handler** (once into `newUser`, again inline for `->setAuthenticated(true)`) instead of reusing the pointer already checked for null — harmless today, but combine with the fix above (deleting that `setAuthenticated` line) and this goes away naturally.

- **_[NOT ACCURATE PROBLEM]_** **`src/commands/Topic.cpp`'s comment is now inaccurate** (see Part 1, item 28) — says the membership guard is "shared for both overloads" in this file, but it actually lives in `dispatchChanCmd.cpp`. Doesn't cause a bug since the check does run before `Topic.cpp` is ever reached, but will mislead whoever reads this file next without also reading the dispatcher.

---

## Priority order for the next sprint

1. **Delete the `setAuthenticated(true)` line in `Server.cpp`'s accept handler** — one line, restores the entire registration gate.
2. **Add the missing `return;` in `dispatchMessage.cpp`** — one line, closes a guaranteed crash.
3. **Validate the MODE flags string starts with `+`/`-` before calling `server.mode()`**, and/or wrap per-command dispatch in its own try/catch — closes the other guaranteed crash and hardens against the next one like it.
4. Fix the nickname digit-rejection logic and add the missing `NOTICE` branch — both quick, both visible the moment someone tests with a normal client.
5. Everything still marked ❌ in the Part 1 table (connection caps, input-buffer cap, plaintext password arg, PING/PONG, Channel's shallow pointer copy) — none of these are regressions, they're just the original backlog items nobody picked up yet.

Items 1–3 are each a one-to-three-line fix and each one, on its own, is a "any single client can take down the server for everyone" bug — I'd treat those as blocking before your next defense/demo, ahead of anything else on the list.
