#include "Server.hpp"

// ERR_NEEDMOREPARAMS --> (Daha fazla parametre gerekiyor)  [+]
// ERR_NOSUCHNICK --> (Böyle bir nick bulunmuyor)           [ ]
// ERR_NOTONCHANNEL --> (Kanalda değilsiniz)                [ ]
// ERR_USERONCHANNEL --> (Kullanıcı kanalda)                [ ]
// ERR_CHANOPRIVSNEEDED --> (Kanal operatörü değilsiniz)    [ ]
// RPL_INVITING --> (Kullanıcı başarıyla davet ediliyor)    [ ]
// RPL_AWAY --> (Kullanıcı meşgul durumda)                  [ ]

int Server::invite(std::string args, Client &client)
{
    std::stringstream   ss(args);
    std::string         tmp;


    if (!args.empty())
    {
        ss >> tmp;
        std::list<Client>::iterator user = this->getUserIterator(tmp);
        
        if (user != this->clients.end())
        {
            ss >> tmp;
            std::list<Channel>::iterator channelIt = this->getChannelIterator(tmp);

            if (channelIt != this->channels.end())
            {
                if (channelIt->findUser(client.cliFd) != channelIt->users.end())
                {
                    if (channelIt->findUser(tmp) == channelIt->users.end())
                    {
                        if (channelIt->isOperator(client.cliFd))
                        {
                            if (!channelIt->isInvitedUser(user->cliFd))
                                channelIt->invitedUsers.push_back(&(*user));
                            (*user).newMessage(RPL_INVITE(client.getNick(), channelIt->name), this->writeFds);
                            client.newMessage(RPL_INVITING((*user).getNick(),channelIt->name), this->writeFds);
                            return (1);
                        }
                        else
                            client.newMessage(ERR_CHANOPRIVSNEEDED(client.getNick(), channelIt->name), this->writeFds);
                    }
                    else
                        client.newMessage(ERR_USERONCHANNEL(client.getNick(), tmp, channelIt->name), this->writeFds);
                }
                else
                    client.newMessage(ERR_NOTONCHANNEL(client.getNick(), channelIt->name), this->writeFds);
            }
            else
                client.newMessage(ERR_NOSUCHCHANNEL(tmp), this->writeFds);
        }
        else
            client.newMessage(ERR_NOSUCHNICK(tmp), this->writeFds);
    }
    else
        client.newMessage(ERR_NEEDMOREPARAMS(std::string("INVITE")), this->writeFds);
    return (0);
    // std::stringstream ss(args); 
    // std::vector<std::string>    tmp;
    // std::string msg;
    
    // while(getline(ss, msg, ' '))
    //     tmp.push_back(msg);
    


    // std::list<Channel>::iterator channelIt = this->getChannelIterator((tmp.at(1)));
    // std::list<Client>::iterator it  = this->getUserIterator(tmp.at(0));      
    // std::vector<Client *>::iterator cIt = channelIt->findUser(tmp.at(0));

    // if (tmp.size() < 2)
    // {
    //    client.newMessage(ERR_NEEDMOREPARAMS(std::string("INVITE")), this->writeFds);
    //    return (0);
    // }
    // if (channelIt == this->channels.end()) 
    // {
    //     client.newMessage(ERR_NOSUCHCHANNEL(tmp.at(1)), this->writeFds);
    //     return (0);
    // }
    // if (it == this->clients.end())
    // {
    //     client.newMessage(ERR_NOSUCHNICK(tmp.at(0)), this->writeFds);
    //     return (0);
    // }
    // if (cIt == channelIt->users.end())
    // {
    //     if (channelIt->isOperator(client.cliFd))
    //     {
    //         if (!(channelIt->isInvitedUser((*it).cliFd)))
    //             channelIt->invitedUsers.push_back(&(*it));
    //         (*it).newMessage(RPL_INVITE(client.getNick(), channelIt->name), this->writeFds);
    //         client.newMessage(RPL_INVITING((*it).getNick(),channelIt->name), this->writeFds);
    //         return (1);
    //     }
    // }
    // if (cIt != channelIt->users.end())
    // {
    //     client.newMessage(ERR_USERONCHANNEL(channelIt->name,it->getNick(),channelIt->name), this->writeFds);
    //     return (0);
    // }
    // else
    //     client.newMessage(ERR_CHANOPRIVSNEEDED(client.getNick(), channelIt->name), this->writeFds);
    // return (0);
 
}

