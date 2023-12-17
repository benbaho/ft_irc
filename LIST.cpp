#include "Server.hpp"

int Server::list(std::string args, Client &client)
{
    (void)args;
    
    client.newMessage(RPL_LISTSTART(client.getNick()), this->writeFds);
    for (std::list<Channel>::iterator it = this->channels.begin(); it != this->channels.end(); it++)
    {
        if (!((*it).isSecret))
        {
            if (!((*it).isPrivate))
                client.newMessage(RPL_LIST(client.getNick(), (*it).name, (*it).getChannelSize(), (*it).topic), this->writeFds);
            else
                client.newMessage(RPL_LIST(client.getNick(), (*it).name, (*it).getChannelSize(), ""), this->writeFds);
        }
    }
    client.newMessage(RPL_LISTEND(client.getNick()), this->writeFds);
    return (1);
}