#include "Server.hpp"

// ERR_NORECIPIENT          [+]             
// ERR_NOTEXTTOSEND         [+]
// ERR_CANNOTSENDTOCHAN     [+]            
// ERR_NOTOPLEVEL           [ ] -
// ERR_WILDTOPLEVEL         [ ] -     
// ERR_TOOMANYTARGETS       [ ] -
// ERR_NOSUCHNICK           [+]
// RPL_AWAY                 [ ] -

std::vector<std::string>    createReceiverVector(const std::string &receiver)
{
    std::stringstream           ss(receiver);
    std::vector<std::string>    receiverVector;
    std::string                 tmp;

    while(getline(ss, tmp, ','))
        receiverVector.push_back(tmp);
    return(receiverVector);
}
// host mask yapılacak
int Server::privmsg(std::string args, Client &client)
{
    std::stringstream           ss(args);
    std::vector<std::string>    receiverVector;
    std::string                 msg;

    ss >> msg;

    receiverVector = createReceiverVector(msg);

    if (ss.eof())
        msg.clear();
    else
        msg = &ss.str()[1 + ss.tellg()];
    if (receiverVector.empty())
    {
        client.newMessage(ERR_NORECIPIENT(client.getNick(),std::string("PRIVMSG")), this->writeFds);
        return (0);
    }
    if (msg.empty())
    {
        client.newMessage(ERR_NOTEXTTOSEND, this->writeFds);
        return (0);
    }
    for (std::vector<std::string>::iterator it = receiverVector.begin(); it != receiverVector.end(); it++)
    {
        if ((*it)[0] == '#' || (*it)[0] == '&')
        {
            std::list<Channel>::iterator  channelIt = this->getChannelIterator(*it);

            if (channelIt != channels.end()
                && (channelIt->isMessageOnlyChUsers == false || channelIt->findUser(client.cliFd) != channelIt->users.end())
                && (channelIt->isMessageOnlyOperators == false || channelIt->isOperator(client.cliFd) || channelIt->isVoicePermittedUser(client.cliFd)))
                channelIt->sendMessageAllUsers(this->writeFds, client.cliFd, PRIVMSG(client.getPrefix(), *it, msg));
            else
                client.newMessage(ERR_CANNOTSENDTOCHAN(client.getNick(), (*it)), this->writeFds);
        }
        else
        {
            std::list<Client>::iterator   userIt = this->getUserIterator(*it);

            if (userIt == this->clients.end())
            {
                client.newMessage(ERR_NOSUCHNICK(*it), this->writeFds);
                return (0);
            }
            userIt->newMessage(PRIVMSG(client.getPrefix(), *it, msg), this->writeFds);
        }
    }
    return(1);
}