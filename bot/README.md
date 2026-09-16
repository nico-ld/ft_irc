*This is an unnecessary complex bot for ft_irc and can be a full project I guess*

# Description

**RouxBot** is an IRC bot that manage Uno and Werewolf game in a channel. The game master is RouxBot, so everyone can play. You can play only one game by channel with  no limit of player.

> _Unfortunately there is only Uno, for timing reason the Werewolf cannot be correctly dev, so we decided to not add it :(_

# Instructions
### Build 
Build the bot : 
```bash
make
```

Clean files and rebuild the bot : 
```bash
make re
```

Clean bot : 
```bash
make clean
make fclean # remove also executable and log file
```

---

### Connect RouxBot
When the server is started launch Rouxbot with the host, the port and the password : 
```bash
# Syntax
./RouxBot <host> <port> <password>

# Example
./RouxBot 127.0.0.1 6667 password
```

If every informations are correct, the bot will connect itself to the server. Then you can invite it to every channel by classic IRC command : 
```bash
INVITE #channel RouxBot
```

--- 

### Create/start a game
When the bot is on the channel you can create a game with :
```bash
!game create <uno/werewolf>
```

Then everyone can join or leave it with : 
```bash
!game join
!game leave
```

When there is enought player (2 for uno, 4 for werewolf), you can start the game with :
```bash
!game start
```

---

### Uno commands

> Uno command has this syntax `!uno <command> [parameter]`

| Command | Description |
|:-------:|:------------|
| rules | Work like an help command, show uno rules and commands |
| turn | Indicates wich player must play |
| hand | Send by private message your deck |
| cards | Indicates every player cards amount |
| top | Indicates the last card played |
| uno | If you're in Uno position, it yell "UNO", otherwhise it counter someone that forgot to tell it |
| play | Play a card, have to indicate card index (and color if this is a wild card) |
| draw | Draw one or severals cards (in case of draw 2 or draw 4 |
| challenge | If last player have played a wild draw 4, you can challenge him |

---

### Werewolf commands

> Not implemented
