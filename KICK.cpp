#include "Server.hpp"

std::string vectorToString(std::vector<std::string> msg)
{
    std::string new_msg;
    unsigned long i = 2;
   
    if (msg.size() == 2)
        return ("No reason needed");
    while (i < msg.size()) 
    {
        if (i == 2)
            new_msg += msg[i].substr(1,msg[i].length());
        else
            new_msg += msg[i];
        if (i != msg.size() - 1) 
            new_msg += " ";
        i++;
    }
    return (new_msg);
} 


int Server::kick(std::string args, Client &client)
{
    std::stringstream ss(args); 
    std::vector<std::string>    tmp;
    std::string msg;

    
    while(getline(ss, msg, ' '))
        tmp.push_back(msg);
    if (tmp.size() > 1)
    {   
        std::list<Channel>::iterator channelIt = this->getChannelIterator((tmp.at(0)));    
        if (channelIt != this->channels.end())
        {
            std::vector<Client *>::iterator it = channelIt->findUser(tmp.at(1));         
            if (channelIt->isOperator(client.cliFd))
            {
                if (channelIt == this->channels.end())
                {
                    return (0);
                }
                if (it == (*channelIt).users.end())
                {   
                    client.newMessage(ERR_USERNOTINCHANNEL(client.getNick(),tmp.at(0),tmp.at(1)), this->writeFds);
                    return (0);
                }
                if (it != (*channelIt).users.end() && tmp.size() >= 2)
                {   
                    channelIt->sendMessageAllUsers(this->writeFds, client.cliFd, KICK(client.getPrefix(), tmp.at(0), tmp.at(1), vectorToString(tmp)));
                    client.newMessage(KICK(client.getNick(),channelIt->name, tmp.at(1), vectorToString(tmp)), this->writeFds);
                    if (channelIt->isInvitedUser(client.cliFd))
                        channelIt->removeInvited(client.cliFd); 
                    channelIt->removeUser((*it)->cliFd);
                    (*it)->removeJoinedChannel(channelIt->name);
                    return (1);
                }
            }
            else
                client.newMessage(ERR_CHANOPRIVSNEEDED(client.getNick(), channelIt->name), this->writeFds);
        }
        else
                client.newMessage(ERR_NOSUCHCHANNEL(tmp.at(0)), this->writeFds);
    }
    else
        client.newMessage(ERR_NEEDMOREPARAMS(std::string("KICK")), this->writeFds);
 
    return (0);
}