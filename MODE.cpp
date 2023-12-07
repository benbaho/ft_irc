#include "Server.hpp"

// ERR_NEEDMOREPARAMS --> (Daha fazla parametre gerekiyor)  [+]
// RPL_CHANNELMODEIS --> (Kanal modları hakkında bilgi)     [ ]
// ERR_CHANOPRIVSNEEDED --> (Kanal operatörü değilsiniz)    [ ]
// ERR_NOSUCHNICK --> (Böyle bir nick bulunmuyor)           [+]
// ERR_NOTONCHANNEL --> (Kanalda değilsiniz)                [+]
// ERR_KEYSET --> (Kanalda zaten bir şifre var)             [ ]
// RPL_BANLIST --> (Ban listesi)                            [ ]
// RPL_ENDOFBANLIST --> (Ban listesinin sonu)               [ ]
// ERR_UNKNOWNMODE --> (Bilinmeyen mode)                    [ ]
// ERR_NOSUCHCHANNEL --> (Böyle bir kanal bulunmuyor)       [+]
// ERR_USERSDONTMATCH --> (Nick uyuşmazlığı)                [ ]
// RPL_UMODEIS --> (Nick modları hakkında bilgi(own))       [ ]
// ERR_UMODEUNKNOWNFLAG --> (Bilinmeyen nick modu)          [ ]

//CHANNEL İsimlerindeki # i kontrol et

void    modeExecuter(std::string args, Client &client)
{
    std::stringstream   ss(args);
    size_t              pos;
    (void)client;

    pos = args.find_first_of("+o");


    if (pos != std::string::npos)
    {
        std::string tmp;
        ss.seekg(pos + 2);

        ss >> tmp;
        std::cout << tmp << std::endl;
    }
}

int Server::mode(std::string args, Client &client)
{
    std::stringstream   ss(args);
    std::string         tmp;


    if (args.empty())
    {
        client.newMessage(ERR_NEEDMOREPARAMS(std::string("MODE")), this->writeFds);
        return (0);
    }

    ss >> tmp;
    std::list<Channel>::iterator    channelIt = this->getChannelIterator(tmp);

    if (channelIt != this->channels.end())
    {
        if (channelIt->findUser(client.cliFd) != channelIt->users.end())
        {
            modeExecuter(&ss.str()[ss.tellg()], client);
            return (1);
        }
        else
        {
            client.newMessage(ERR_NOTONCHANNEL(channelIt->name), this->writeFds);
            return (0);
        }
    }
    else
    {
        client.newMessage(ERR_NOSUCHCHANNEL(tmp), this->writeFds);
        return (0);
    }



    return (0);
}