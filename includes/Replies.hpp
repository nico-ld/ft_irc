/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 14:28:48 by nile-dai          #+#    #+#             */
/*   Updated: 2026/08/19 12:07:00 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
# define REPLIES_HPP

/*
** ============================================================
**  IRC Numeric Replies — RFC 1459 / RFC 2812
**  Scoped to what a typical ft_irc server needs:
**  connection registration, JOIN/PART/QUIT, PRIVMSG/NOTICE,
**  NICK/USER, TOPIC, MODE, KICK, INVITE, and common errors.
** ============================================================
**
**  Every real IRC reply line has the shape:
**    :<server_name> <numeric> <target_nick> <params...> :<trailing>
**
**  <target_nick> is the nick of the client the reply is sent to
**  (use "*" if not registered yet). <trailing> is prefixed with
**  ':' and can contain spaces; everything before it is
**  space-separated tokens with no spaces inside them.
**
**  The macros below are just the numeric codes (as in RFC 2812).
**  The helper functions after them build ready-to-send lines so
**  you don't hand-format ":" and spaces everywhere and typo them.
*/

// ---------------------------------------------------------------
// Connection registration replies (001-005 range)
// ---------------------------------------------------------------
# define RPL_WELCOME            "001" // "Welcome to the Internet Relay Network <nick>!<user>@<host>"
# define RPL_YOURHOST           "002" // "Your host is <servername>, running version <ver>"
# define RPL_CREATED            "003" // "This server was created <date>"
# define RPL_MYINFO             "004" // "<servername> <version> <available user modes> <available channel modes>"

// ---------------------------------------------------------------
// Command responses
// ---------------------------------------------------------------
# define RPL_UMODEIS            "221" // "<user mode string>"

# define RPL_CHANNELMODEIS      "324" // "<channel> <mode> <mode params>"
# define RPL_NOTOPIC            "331" // "<channel> :No topic is set"
# define RPL_TOPIC              "332" // "<channel> :<topic>"
# define RPL_INVITING           "341" // "<channel> <nick>"

# define RPL_NAMREPLY           "353" // "<sym> <channel> :<nick1> <nick2> ..."   sym = '=' / '*' / '@'
# define RPL_ENDOFNAMES         "366" // "<channel> :End of /NAMES list"

# define RPL_MOTDSTART          "375" // ":- <server> Message of the day - "
# define RPL_MOTD               "372" // ":- <text>"
# define RPL_ENDOFMOTD          "376" // ":End of /MOTD command"

// ---------------------------------------------------------------
// Error replies
// ---------------------------------------------------------------
# define ERR_NOSUCHNICK         "401" // "<nickname> :No such nick/channel"
# define ERR_NOSUCHCHANNEL      "403" // "<channel name> :No such channel"
# define ERR_CANNOTSENDTOCHAN   "404" // "<channel name> :Cannot send to channel"
# define ERR_TOOMANYCHANNELS    "405" // "<channel name> :You have joined too many channels"

# define ERR_NORECIPIENT        "411" // ":No recipient given (<command>)"
# define ERR_NOTEXTTOSEND       "412" // ":No text to send"

# define ERR_UNKNOWNCOMMAND     "421" // "<command> :Unknown command"
# define ERR_NOMOTD             "422" // ":MOTD File is missing"

# define ERR_NONICKNAMEGIVEN    "431" // ":No nickname given"
# define ERR_ERRONEUSNICKNAME   "432" // "<nick> :Erroneous nickname"
# define ERR_NICKNAMEINUSE      "433" // "<nick> :Nickname is already in use"

# define ERR_USERNOTINCHANNEL   "441" // "<nick> <channel> :They aren't on that channel"
# define ERR_NOTONCHANNEL       "442" // "<channel> :You're not on that channel"
# define ERR_USERONCHANNEL      "443" // "<user> <channel> :is already on channel"

# define ERR_NOTREGISTERED      "451" // ":You have not registered"

# define ERR_NEEDMOREPARAMS     "461" // "<command> :Not enough parameters"
# define ERR_ALREADYREGISTRED   "462" // ":Unauthorized command (already registered)"
# define ERR_PASSWDMISMATCH     "464" // ":Password incorrect"

# define ERR_CHANNELISFULL      "471" // "<channel> :Cannot join channel (+l)"
# define ERR_UNKNOWNMODE        "472" // "<char> :is unknown mode char to me"
# define ERR_INVITEONLYCHAN     "473" // "<channel> :Cannot join channel (+i)"
# define ERR_BANNEDFROMCHAN     "474" // "<channel> :Cannot join channel (+b)"
# define ERR_BADCHANNELKEY      "475" // "<channel> :Cannot join channel (+k)"

# define ERR_CHANOPRIVSNEEDED   "482" // "<channel> :You're not channel operator"

# define ERR_UMODEUNKNOWNFLAG   "501" // ":Unknown MODE flag"
# define ERR_USERSDONTMATCH     "502" // ":Cannot change mode for other users"


#endif