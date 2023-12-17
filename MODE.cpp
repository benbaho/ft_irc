#include "Server.hpp"
#include "Channel.hpp"

// ERR_NEEDMOREPARAMS --> (Daha fazla parametre gerekiyor)  [+]
// RPL_CHANNELMODEIS --> (Kanal modları hakkında bilgi)     [ ]
// ERR_CHANOPRIVSNEEDED --> (Kanal operatörü değilsiniz)    [+]
// ERR_NOSUCHNICK --> (Böyle bir nick bulunmuyor)           [ ]
// ERR_NOTONCHANNEL --> (Kanalda değilsiniz)                [+]
// ERR_KEYSET --> (Kanalda zaten bir şifre var)             [ ]
// RPL_BANLIST --> (Ban listesi)                            [ ]
// RPL_ENDOFBANLIST --> (Ban listesinin sonu)               [ ]
// ERR_UNKNOWNMODE --> (Bilinmeyen mode)                    [+]
// ERR_NOSUCHCHANNEL --> (Böyle bir kanal bulunmuyor)       [+]
// ERR_USERSDONTMATCH --> (Nick uyuşmazlığı)                [ ]
// RPL_UMODEIS --> (Nick modları hakkında bilgi(own))       [ ]
// ERR_UMODEUNKNOWNFLAG --> (Bilinmeyen nick modu)          [ ]

// o - operatör yetkisi verme/alma;                                         [+]
// p - özel kanal;                                                          [+]
// s - gizli kanal;                                                         [+]
// i - davetsel moddaki kanal;                                              [+]
// t - sadece operatörlerin başlık değiştirebildiği kanal;                  [+]
// n - kanalda bulunmayanların mesaj gönderemediği kanal;                   [+]
// m - sadece operatörlerin konuşabildiği kanal;                            [+]
// l - kanala limit koyma/limit konmuş kanal;                               [+]
// b - bir mask'i ban'lama/ban'ı kaldırma;                                  [+]
// v - +m moddaki kanalda bir kullanıcıya konuşma izni (voice) verme/alma;  [+]
// k - kanala şifre koyma/kaldırma/şifre konmuş kanal.                      [+]

int     Channel::modeO(std::string args, bool type)
{
    std::vector<Client *>::iterator user = this->findUser(args);

    if (user != this->users.end())
    {
        if (type && !this->isOperator((*user)->cliFd))
        {
            this->operators.push_back(*user);
            this->updatedModesBuff += "+o " + (*user)->getNick();
        }
        else if (!type && this->isOperator((*user)->cliFd))
        {
            this->removeOperator((*user)->cliFd);
            this->updatedModesBuff += "-o " + (*user)->getNick();

        }
    }
    return (1);
}

int     Channel::modeT(std::string args, bool type)
{
    (void)args;
    if (type)
    {
        this->isTopicOperatorOnly = true;
        this->updatedModesBuff += "+t";
    }
    else
    {
        this->isTopicOperatorOnly = false;
        this->updatedModesBuff += "-t";
    }
    return (1);
}

int     Channel::modeL(std::string args, bool type)
{
    if ((args.empty() && type) || (!args.empty() && args.find_first_not_of('-') != 0))
        return (0);
    if (type)
    {
        std::stringstream   ss(args);
        size_t              limit;

        ss >> limit;
        this->userLimit = limit;
        ss << limit;
        this->updatedModesBuff += "+l " + ss.str();
    }
    else
    {
        this->userLimit = 0;
        this->updatedModesBuff += "-l ";
    }
    return(1);
}

int     Channel::modeI(std::string args, bool type)
{
    (void)args;
    if (type)
    {
        this->isInviteOnly = true;
        this->updatedModesBuff += "+i ";
    }
    else
    {
        this->isInviteOnly = false;
        this->updatedModesBuff += "-i";
    }
    return (1);
}

int     Channel::modeK(std::string args, bool type)
{
    if (type)
    {
        this->key = args;
        this->updatedModesBuff += "+k " + args;
    }
    else
    {
        this->key.clear();
        this->updatedModesBuff += "-k";
    }
    return(1);
}

int     Channel::modeN(std::string args, bool type)
{
    (void)args;
    if (type)
    {
        this->isMessageOnlyChUsers = true;
        this->updatedModesBuff += "+n";
    }
    else
    {
        this->isMessageOnlyChUsers = false;
        this->updatedModesBuff += "-n";
    }
    return (1);
}

int     Channel::modeM(std::string args, bool type)
{
    (void)args;
    if (type)
    {
        this->isMessageOnlyOperators = true;
        this->updatedModesBuff += "+m";
    }
    else
    {
        this->isMessageOnlyOperators = false;
        this->updatedModesBuff += "-m";
    }
    return (1);
}

int     Channel::modeV(std::string args, bool type)
{
    std::vector<Client *>::iterator user = this->findUser(args);

    if (user != this->users.end())
    {
        if (type && !this->isVoicePermittedUser((*user)->cliFd))
        {
            this->voicePermittedUsers.push_back(*user);
            this->updatedModesBuff += "+v " + (*user)->getNick();
        }
        else if (!type && this->isVoicePermittedUser((*user)->cliFd))
        {
            this->removeVoicePermittedUser((*user)->cliFd);
            this->updatedModesBuff += "-v " + (*user)->getNick();

        }
    }
    return (1);
}

int     Channel::modeP(std::string args, bool type)
{
    (void)args;
    if (type)
    {
        this->isPrivate = true;
        this->updatedModesBuff += "+p";
    }
    else
    {
        this->isPrivate = false;
        this->updatedModesBuff += "-p";
    }
    return (1);
}

int     Channel::modeS(std::string args, bool type)
{
    (void)args;   
    if (type)
    {
        this->isSecret = true;
        this->updatedModesBuff += "+s";
    }
    else
    {
        this->isSecret = false;
        this->updatedModesBuff += "-s";
    }
    return (1);
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
        if (ss.eof())
        {
            client.newMessage(RPL_CHANNELMODEIS(client.getNick(), channelIt->name, channelIt->getChannelModes()), this->writeFds);
            return(1);
        }
        if (channelIt->findUser(client.cliFd) != channelIt->users.end())
        {
            if (channelIt->isOperator(client.cliFd))
            {
                ss >> tmp;

                for (std::string::iterator  it = tmp.begin(); it != tmp.end(); it++)
                    if (*it == '+' || *it == '-')
                    {
                        std::string user;
                        ss >> user;
                        if ((channelIt->modeFunctionsMap)[*(it + 1)])
                        {
                            ((*channelIt).*(channelIt->modeFunctionsMap)[*(it + 1)])(user, *it == '+' ? 1 : 0);
                            channelIt->sendMessageAllUsers(this->writeFds, WITH_MSG_SENDER, 
                                                            MODE(client.getPrefix(),
                                                            channelIt->name, channelIt->updatedModesBuff));
                            channelIt->updatedModesBuff.clear();
                        }
                        else
                            client.newMessage(ERR_UNKNOWNMODE(client.getNick(), *(it + 1)), this->writeFds);
                    }
                return (1);
            }
            else
            {
                client.newMessage(ERR_CHANOPRIVSNEEDED(client.getNick(), channelIt->name), this->writeFds);
                return (0);
            }

        }
        else
        {
            client.newMessage(ERR_NOTONCHANNEL(client.getNick(),channelIt->name), this->writeFds);
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