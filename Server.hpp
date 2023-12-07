#pragma once

#include <vector>
#include <iostream>
#include <map>
#include <sstream>
#include <list>

#include "Client.hpp"
#include "Channel.hpp"


// SERVER PASS Sonradan değiştirilemez

//    <target>     ::= <to> [ "," <target> ]
//    <to>         ::= <channel> | <user> '@' <servername> | <nick> | <mask>
//    <channel>    ::= ('#' | '&') <chstring>
//    <servername> ::= <host>
//    <host>       ::= see RFC 952 [DNS:4] for details on allowed hostnames
//    <nick>       ::= <letter> { <letter> | <number> | <special> }
//    <mask>       ::= ('#' | '$') <chstring>
//    <chstring>   ::= <any 8bit code except SPACE, BELL, NUL, CR, LF and
//                      comma (',')>

//    Other parameter syntaxes are:

//    <user>       ::= <nonwhite> { <nonwhite> }
//    <letter>     ::= 'a' ... 'z' | 'A' ... 'Z'
//    <number>     ::= '0' ... '9'
//    <special>    ::= '-' | '[' | ']' | '\' | '`' | '^' | '{' | '}'

#define ERR_NOSUCHCHANNEL(channelName)			": 403 " + channelName + " :No such channel\r\n"
#define ERR_NEEDMOREPARAMS(command)				": 461 " + command + " :Not enough parameters\r\n"
#define ERR_ALREADYREGISTRED					": 462 :You may not reregister\r\n"
#define ERR_NONICKNAMEGIVEN						": 431 :No nickname given\r\n"
#define ERR_ERRONEUSNICKNAME(nick)				": 432 " + nick + " :Erroneus nickname\r\n"
#define ERR_NICKNAMEINUSE(nick)					": 433 " + nick + " :Nickname is already in use\r\n"
#define ERR_BADCHANNELKEY(channelName)			": 475 " + channelName + " :Cannot join channel (+k)\r\n"
#define ERR_NORECIPIENT(command)				": 411 :No recipient given (" + command + ")\r\n"
#define ERR_NOTEXTTOSEND						": 412 : No text to send\r\n"
#define ERR_NOSUCHNICK(nick)					": 401 " + nick + " :No such nick/channel\r\n"
#define ERR_NOTONCHANNEL(channelName)			": 442 " + channelName + ":You're not on that channel\r\n"
#define ERR_CHANOPRIVSNEEDED(nick, channel)		": 482 " + nick + " " + channel + " :You're not channel operator\r\n"

#define RPL_NAMREPLY(nick, channel, userList)	": 353 " + nick + " = " + channel + " :" + userList + "\r\n"
#define RPL_ENDOFNAMES(nick,channel)			": 366 " + nick + " " + channel + " :End of /NAMES list\r\n"
#define RPL_NOTOPIC(nick, channel)				": 331 " + nick + " " + channel + " :No topic is set\r\n"   
#define RPL_TOPIC(nick, channel, topic)			": 332 " + nick + " " + channel + " " + topic + "\r\n"      

#define RPL_WHOISUSER(target, nick, user, host, realname) 			": 311 " + target + " " + nick + " " + user + " " + host + " * :" + realname + "\r\n"
#define RPL_WHOISSERVER(target, nick, server, serverinfo)			": 312 " + target + " " + nick + " " + server + " :" + serverinfo + "\r\n"
#define RPL_WHOISACCOUNT(target, nick, account)						": 330 " + target + " " + nick + " " + account + " :is authed as"+ "\r\n"


// my RPL_QUIT
#define RPL_QUIT(nick, user, message)					":" + nick + "!" + user + "@localhost" + " QUIT :Quit: " + message + "\r\n"
#define RPL_PART()

class Server
{
	private:
		typedef int (Server::*myFunc)(std::string, Client &);
		int                                 serverFd;
		fd_set                              readFds;
		fd_set                              writeFds;

		const std::string                   serverPass;

		int     							addNewClient(fd_set &readFdsTmp);
		int     							readFromClients(fd_set &readFdsTmp);
		int     							writeClients(fd_set &writeFdsTmp);
	
	public:
		std::list<Client>					clients;
		std::list<Channel>					channels;

		Server(int port, const char *pass);
		void                                createSocket(int port);
		void                                serverLoop(void);

		std::list<Channel>::iterator      	getChannelIterator(const std::string &channelName);
		std::list<Client>::iterator       	getUserIterator(const std::string &userName);

		//commands
		int     							pass(std::string args, Client &client);
		int     							nick(std::string args, Client &client);
		int     							user(std::string args, Client &client);
		int     							quit(std::string args, Client &client);
		int     							join(std::string args, Client &client);
		int     							privmsg(std::string args, Client &client);
		int     							topic(std::string args, Client &client);
		int     							part(std::string args, Client &client);
		int     							whois(std::string args, Client &client);
		int									mode(std::string args, Client &client);

		
		std::map<std::string, myFunc>       functionsMap;

};