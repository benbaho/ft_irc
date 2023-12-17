#include "Server.hpp"

int	Server::notice(std::string args, Client &client)
{
    std::stringstream ss(args); 
    std::vector<std::string>    tmp;
    std::string msg;

    
    while(getline(ss, msg, ' '))
        tmp.push_back(msg);
    if (tmp.size() == 0)
    {
        client.newMessage(ERR_NORECIPIENT(client.getNick(),std::string("NOTICE")), this->writeFds);
        return (0);
    }
    else if (tmp.size() == 1)
    {
        client.newMessage(ERR_NOTEXTTOSEND, this->writeFds);
        return (0);
    }
    else if (tmp.size() >= 2)
    {   
        msg = args.substr(args.find(':') + 1, args.length());
        if ((tmp.at(0)[0] == '#') || (tmp.at(0)[0] == '&'))
        {
            std::list<Channel>::iterator  channelIt = this->getChannelIterator(tmp.at(0));
            if (channelIt != channels.end())
            {
                (*channelIt).sendMessageAllUsers(this->writeFds, WITH_MSG_SENDER, NOTICE(client.getNick(),(*channelIt).name, msg));
                return (1);
            }
        }
        else
        {   
            std::list<Client>::iterator it = this->getUserIterator(tmp.at(0));
            if (it != this->clients.end())
            {
                (*it).newMessage(NOTICE(client.getNick(),(*it).getNick(), msg), this->writeFds);
                return (1);
            }
        }
    }
    return (0);
}