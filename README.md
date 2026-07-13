_This project has been created as part of the 42 curriculum by jdessoli, afons, nile-dai_

# ft_irc

> A simple Internet Relay Chat (IRC) server written in C++98 as part of the 42 curriculum.

### Overview

**ft_irc** is an implementation of an IRC server compatible with standard IRC clients. The goal of this project is to understand network programming, socket management, multiplexing, and protocol implementation while respecting the C++98 standard.

The server accepts multiple simultaneous client connections, manages users and channels, and processes IRC commands according to the project specifications.

---

### Learning Objectives

This project focuses on:

* Socket programming
* Network protocols
* Event-driven programming
* Client/server architecture
* Parsing protocols
* Resource management
* Object-oriented design in C++98

---

### Features

* Multiple client connections
* User authentication
* Nickname and username management
* Channel creation and management
* Private messaging
* Channel messaging
* IRC command parsing
* Operator privileges
* Graceful client disconnection

> **Implemented commands**
>
> * PASS
> * NICK
> * USER
> * JOIN
> * PART
> * PRIVMSG
> * NOTICE
> * QUIT
> * KICK
> * INVITE
> * TOPIC
> * MODE

---

### Channel Modes

Implemented channel modes:

* [ ] `i` — Invite-only
* [ ] `t` — Topic restricted
* [ ] `k` — Password protected
* [ ] `o` — Channel operator
* [ ] `l` — User limit

# Instruction

### Requirements

* C++98
* GNU Make
* POSIX sockets

---

### Build

```bash
make
```

Clean files:

```bash
make clean
make fclean
```

Rebuild:

```bash
make re
```

---

### Usage

Start the server:

```bash
./ircserv <port> <password>
```

Example:

```bash
./ircserv 6667 password
```

---

### Connecting

Using **nc**:

```bash
nc localhost 6667
```

Using an IRC client (HexChat, irssi, WeeChat, etc.):

```
Server: localhost
Port: 6667
Password: <password>
```

# Resources
We used [rfc 1459](https://www.rfc-editor.org/rfc/rfc1459) and [rfc 2812](https://www.rfc-editor.org/rfc/rfc2812) to structure the project, then somes websites like [wikipedia](https://en.wikipedia.org/wiki/IRC) to understand irc.

### AI usage
Somes AI have been used to translate this page, explain somes features and help us to find somes bugs.
