#include "Client.hpp"
#include "Channel.hpp"

Client::Client()
{
    this->auth = 0;
}

Client::~Client()
{
    for (std::vector<Channel *>::iterator it = this->joinedChannels.begin(); it != this->joinedChannels.end(); it++)
        (*it)->removeUser(this->cliFd);
}

bool        Client::compareNick(const std::string &nick)
{
    std::string lowerNick = strLower(nick);

    if (strLower(this->nick) == strLower(nick))
        return (1);
    return (0);
}

void        Client::newMessage(const std::string &message, fd_set &writeFds)
{
    FD_SET(this->cliFd, &writeFds);
    this->incomingMessages.push(message);
}

std::string Client::getUserinfo(){
    return this->userInfo;
}

std::string Client::getHostname(){
    return this->hostName;
}

std::string Client::getRealname(){
    return this->realName;
}

std::string Client::getServername(){
    return this->serverName;
}

std::string Client::getUsername(){
    return this->userName;
}

std::string Client::getNick()
{
    return this->nick;
}

std::string Client::getPrefix(std::string command, std::string args)
{
    std::string prefix(":" + this->nick + "!" + this->userName + "@" + this->hostName + " " + command + " " + args + "\r\n");

    // std::cout << prefix << std::endl;
    
    return (prefix);
}

// void Client::setNickname(std::string s){
//     this->nickname = s;
// }

int Client::setNick(std::string &nick)
{
    for (std::string::iterator it = nick.begin(); it != nick.end(); it++)
        if ((*it != '-' && *it != '[' && *it != ']' && *it != '\\' && *it != '`' && *it != '{' && *it != '}' && *it != '|') && !isalnum(*it))
            return (0);
    if (this->auth)
        this->nick = nick;
    return (1);
}

void    Client::setUserInfo(std::vector<std::string> userInfoVec)
{  
    this->userName = userInfoVec[0];
    this->hostName = userInfoVec[1];
    this->serverName = userInfoVec[2];
    this->realName = userInfoVec[3];
}
    
void    Client::removeJoinedChannel(const std::string &name)
{
    for (std::vector<Channel *>::iterator   channelIt = this->joinedChannels.begin(); channelIt != this->joinedChannels.end(); channelIt++)
    {
        if ((*channelIt)->name == name)
        {
            this->joinedChannels.erase((channelIt));
            break;
        }
    }
}

bool    Client::isRegistered()
{
    if (this->userName.length() != 0 || this->hostName.length() != 0
        || this->serverName.length() != 0 || this->realName.length() != 0)
        return (0);
    return (1);
}


