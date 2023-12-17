#include "Channel.hpp"

Channel::Channel()
{
    this->userLimit = 0;
    this->isInviteOnly = false;
    this->isTopicOperatorOnly = false;
    this->isMessageOnlyOperators = false;
    this->isMessageOnlyChUsers = false;
    this->isPrivate = false;
    this->isSecret = false;
    
    this->modeFunctionsMap['o'] = &Channel::modeO;
    this->modeFunctionsMap['t'] = &Channel::modeT;
    this->modeFunctionsMap['l'] = &Channel::modeL;
    this->modeFunctionsMap['i'] = &Channel::modeI;
    this->modeFunctionsMap['k'] = &Channel::modeK;
    this->modeFunctionsMap['n'] = &Channel::modeN;
    this->modeFunctionsMap['m'] = &Channel::modeM;
    this->modeFunctionsMap['v'] = &Channel::modeV;
    this->modeFunctionsMap['p'] = &Channel::modeP;
    this->modeFunctionsMap['s'] = &Channel::modeS;
}


std::string Channel::getChannelSize(){
    std::stringstream ss;
    
    ss << this->users.size();
    
    return (ss.str());
}

std::string Channel::getChannelModes(void)
{
    std::string channelModes;
    std::string channelModeArgs;
    std::stringstream ss(this->userLimit);

    if (!this->key.empty())
    {
        channelModes += "k";
        channelModeArgs += this->key + " ";
    }
    if (this->userLimit != 0)
    {
        channelModes += "l";
        channelModeArgs += ss.str();
        channelModeArgs += " ";
    }
    if (this->isInviteOnly)
        channelModes += "i";
    
    channelModes += " " + channelModeArgs;
    return (channelModes);
}

bool    Channel::isOperator(const int &clientFd)
{
    for (std::vector<Client *>::iterator   it = this->operators.begin(); it != this->operators.end(); it++)
        if ((*it)->cliFd == clientFd)
            return (1);
    return (0);
}

bool    Channel::isInvitedUser(const int &clientFd)
{
    for (std::vector<Client *>::iterator   it = this->invitedUsers.begin(); it != this->invitedUsers.end(); it++)
        if ((*it)->cliFd == clientFd)
            return (1);
    return (0);
}

bool    Channel::isVoicePermittedUser(const int &clientFd)
{
    for (std::vector<Client *>::iterator   it = this->voicePermittedUsers.begin(); it != this->voicePermittedUsers.end(); it++)
        if ((*it)->cliFd == clientFd)
            return (1);
    return (0);
}

int     Channel::removeUser(const int &clientFd)
{
    this->removeInvited(clientFd);
    this->removeOperator(clientFd);
    this->removeVoicePermittedUser(clientFd);
    for (std::vector<Client *>::iterator   it = this->users.begin(); it != this->users.end(); it++)
    {
        if ((*it)->cliFd == clientFd)
        {
            this->users.erase(it);
            return (1);
        }
    }
    return (0);
}

int         Channel::removeInvited(const int &clientFd)
{
    for (std::vector<Client *>::iterator it = this->invitedUsers.begin(); it != this->invitedUsers.end(); it++)
    {
        if ((*it)->cliFd == clientFd)
        {
            this->invitedUsers.erase(it);    
            return (1); 
        }
    }
    return (0);
}


int         Channel::removeOperator(const int &clientFd)
{
    for (std::vector<Client *>::iterator it = this->operators.begin(); it != this->operators.end(); it++)
    {
        if ((*it)->cliFd == clientFd)
        {         
            this->operators.erase(it);
            return (1);
        }
    } 
    return (0);
}

int         Channel::removeVoicePermittedUser(const int &clientFd)
{
    for (std::vector<Client *>::iterator it = this->voicePermittedUsers.begin(); it != this->voicePermittedUsers.end(); it++)
    {
        if ((*it)->cliFd == clientFd)
        {         
            this->voicePermittedUsers.erase(it);
            return (1);
        }
    } 
    return (0);
}

std::string Channel::getUserList()
{
    std::stringstream   ss;

    for (std::vector<Client *>::iterator   it = this->users.begin(); it != this->users.end(); it++)
    {
        ss << (this->isOperator((*it)->cliFd) ? "@" + (*it)->getNick() : (*it)->getNick());
        if (it != this->users.end() - 1)
            ss << " ";
    }
    return (ss.str());
}

void        Channel::updateUserList(fd_set &writeFds)
{
    for (std::vector<Client *>::iterator   it = this->users.begin(); it != this->users.end(); it++)
    {
       (*it)->newMessage(RPL_NAMREPLY((*it)->getNick(), this->name, this->getUserList()), writeFds);
       (*it)->newMessage(RPL_ENDOFNAMES((*it)->getNick(), this->name) , writeFds);
    }
}

void        Channel::sendMessageAllUsers(fd_set &writeFds, const int &cliFd, const std::string &message)
{
    for (std::vector<Client *>::iterator   it = this->users.begin(); it != this->users.end(); it++)
        if (cliFd == -1 || (*it)->cliFd != cliFd)
            (*it)->newMessage(message, writeFds);
}

std::vector<Client *>::iterator   Channel::findUser(const int &cliFd)
{
    for (std::vector<Client *>::iterator    it = this->users.begin(); it != this->users.end(); it++)
    {
        if ((*it)->cliFd == cliFd)
            return (it);
    }
    return (this->users.end());
}

std::vector<Client *>::iterator   Channel::findUser(const std::string &nick)
{
    for (std::vector<Client *>::iterator    it = this->users.begin(); it != this->users.end(); it++)
    {
        if ((*it)->getNick() == nick)
            return (it);
    }
    return (this->users.end());
}

int Channel::compareKeys(const std::string &key)
{
    if (this->key == key)
        return(1);
    return(0);
}

void    Channel::setKey(const std::string &key)
{
    this->key = key;
}