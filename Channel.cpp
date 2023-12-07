#include "Channel.hpp"

bool    Channel::isOperator(const int &clientFd)
{
    for (std::vector<Client *>::iterator   it = this->operators.begin(); it != this->operators.end(); it++)
        if ((*it)->cliFd == clientFd)
            return (1);
    return (0);
}

int         Channel::removeUser(const int &clientFd)
{
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
        if ((*it)->cliFd != cliFd)
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