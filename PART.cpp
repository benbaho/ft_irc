#include "Server.hpp"

// ERR_NEEDMOREPARAMS   [+] --> (Daha fazla parametre gerekiyor)
// ERR_NOSUCHCHANNEL    [+] --> (Böyle bir kanal bulunmuyor)
// ERR_NOTONCHANNEL     [+] --> (Kanalda değilsiniz)

std::vector<std::string>    createChannelVector(const std::string &channel)
{
    std::stringstream           ss(channel);
    std::vector<std::string>    channelVector;
    std::string                 tmp;

    while(getline(ss, tmp, ' '))
        channelVector.push_back(tmp);
    return(channelVector);
}


int Server::part(std::string args, Client &client) 
{
    std::stringstream               ss(args);
    std::string                     tmp;

    ss >> tmp;
    std::vector<std::string>        channelVector = createChannelVector(tmp);
    if (channelVector.empty())
    {
        client.newMessage(ERR_NEEDMOREPARAMS(std::string("PART")), writeFds);
        return (0);
    }
    for (std::vector<std::string>::iterator it = channelVector.begin(); it != channelVector.end(); it++)
    {
        std::list<Channel>::iterator  channelIt = this->getChannelIterator(*it);
        if (channelIt == channels.end())
        {
            client.newMessage(ERR_NOSUCHCHANNEL(*it), writeFds);
            return (0);
        }
        if (channelIt->isInvitedUser(client.cliFd))
            channelIt->removeInvited(client.cliFd);
        channelIt->sendMessageAllUsers(this->writeFds, WITH_MSG_SENDER, PART(client.getPrefix(),channelIt->name));
        if (!channelIt->removeUser(client.cliFd))
        {
            client.newMessage(ERR_NOTONCHANNEL(client.getNick(),*it), writeFds);
            return(0);
        }
       
        client.removeJoinedChannel(channelIt->name);
        if (channelIt->users.empty())
            this->channels.erase(channelIt);
    }
    return (1);
}