_This project has been created as part of the 42 curriculum by jdessoli, afons, nile-dai_

# Description

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
* RouxBot

---

### Implemented commands

| Commands | Syntax | Description |
|:-------- |:------|:-----------|
| PASS | `PASS <password>` | Send password to server, if password is correct the user is registered |
| NICK | `NICK <nickname>` | Define user nickname, it have to be unique and must follow somes [specific rules](#authentication) |
| USER | `USER <user> <mode> <unused> <:realname>` | Define user name and realname |
| JOIN | `JOIN <channel>` | Join or create one or severals channel |
| PART | `PART <channel> [:reason]` | Leave one or severals channel |
| KICK | `KICK <channel> <kicked> [:reason]` | Kick a user from the channel _(only for channel operator)_ |
| QUIT | `QUIT [:reason]` | Leave the server |
| PRIVMSG | `PRIVMSG <channel/user> <:message>` | Send a message to a channel or an user |
| NOTICE | `NOTICE <channel/user> <:message>` | Same as `PRIVMSG` but send no replies in case of error |
| INVITE | `INVITE <user> <channel>` | Invite a user to the channel |
| TOPIC | `TOPIC [:topic]` | If no topic is specified it return the current topic, otherwise it replace the topic |
| MODE | `MODE [flag(s)] [parameter(s)]` | Modify channel permissions, mode or channel operators _(only for channel operator)_ |

### Channel Modes

| Flag | Syntax | Description |
|:----:|:------:|:------------|
| i | `+/-i` | Set the channel in invite only, so only user invited by a channel operator |
| t | `+/-t` | Restric the channel topic, only a channel operator can change it |
| o | `+/-o <user>` | Give or remove channel operator rights |
| k | `+k <key> / -k` | Set a private key to join the channel |
| l | `+l <limit> / -t` | Set a user limit on the channel |

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

---

### Authentication

In first you have to enter the password (this is automatic on IRC client but not for client like **nc**) : 
```bash
PASS password
```

Then You have to enter a nickname and a realname. The nickname follow somes specific rules :
- That cannot be longer than 9 character.
- That must start with alpha character or a valid one *([\]^_`{|})*.
- The 8 other characters also must be valid, be can be numeric or '-'.
- Your nickname must be unique.

```bash
NICK nickname
USER user . . :real name
```

And that's it ! You're authenticate !

--- 

### RouxBot
> See README.md in `bot/` directory

# Resources
We used [rfc 1459](https://www.rfc-editor.org/rfc/rfc1459) and [rfc 2812](https://www.rfc-editor.org/rfc/rfc2812) to structure the project, then somes websites like [wikipedia](https://en.wikipedia.org/wiki/IRC) to understand irc.

### AI usage
Somes AI have been used to translate this page, explain somes features and help us to find somes bugs.
