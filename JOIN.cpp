#include "Server.hpp"
#include "Channel.hpp"
#include <sstream>


// ERR_NEEDMOREPARAMS --> (Daha fazla parametre gerekiyor)      [+]
// ERR_BANNEDFROMCHAN --> (Kanalda banlısınız)                  [ ]
// ERR_INVITEONLYCHAN --> (Sadece davet edilenler girebilir)    [ ]
// ERR_BADCHANNELKEY --> (Kanal şifresi yanlış girildi)         [ ]
// ERR_CHANNELISFULL --> (Kanalın kullanıcı limiti dolu)        [ ]
// ERR_BADCHANMASK --> (Kanal ismi yanlış girildi)              [ ]
// ERR_NOSUCHCHANNEL --> (Böyle bir kanal bulunmuyor)           [ ]
// ERR_TOOMANYCHANNELS --> (Çok fazla kanalda bulunuyorsunuz)   [ ]
// RPL_TOPIC --> (Başlık mesajı verilir)                        [ ]

// İkinci Channel a girildiğinde user list güncellenmiyor !!!


std::vector<ChannelToken>    convertChannelTokens(std::string args)
{
    std::vector<ChannelToken>   tokens;
    std::stringstream           ss(args);
    std::stringstream           ss2;
    std::string                 tmp[2];

    ss >> tmp[0];
    ss >> tmp[1];
    ss.clear();
    ss.str(tmp[0]);
    ss2.str(tmp[1]);

    while(getline(ss, tmp[0], ','))
    {
        ChannelToken    token;
        
        getline(ss2, tmp[1], ',');
        token.name = tmp[0];
        token.key = tmp[1];
        tokens.push_back(token);
    }
    return (tokens);
}

int Server::join(std::string args, Client &client)
{
    if (args.empty())
    {
        client.newMessage(ERR_NEEDMOREPARAMS(std::string("JOIN")), this->writeFds);
        return (0);
    }
    else
    {
        std::vector<ChannelToken>   argTokens = convertChannelTokens(args);

        for (std::vector<ChannelToken>::iterator it = argTokens.begin(); it != argTokens.end(); it++)
        {
            if (it->name[0] == '#' || it->name[0] == '&')
            {
                Channel                         newChannel;
                std::list<Channel>::iterator  channelIt = this->getChannelIterator(it->name);

                if (channelIt == this->channels.end())
                {
                    newChannel.name = it->name;
                    newChannel.setKey(it->key);
                    newChannel.users.push_back(&client);
                    this->channels.push_back(newChannel);
                    client.joinedChannels.push_back(&(*this->channels.begin()));
                    client.newMessage(client.getPrefix("JOIN", it->name), this->writeFds);
                    (*this->channels.begin()).operators.push_back(&client);
                    (*this->channels.begin()).updateUserList(this->writeFds);
                }
                else
                {
                    if (channelIt->compareKeys(it->key))
                    { 
                        channelIt->users.push_back(&client);
                        client.newMessage(client.getPrefix("JOIN", it->name), this->writeFds);
                        client.joinedChannels.push_back(&(*this->channels.begin()));
                        channelIt->updateUserList(this->writeFds);
                        if (!channelIt->topic.empty())
                            client.newMessage(RPL_TOPIC(client.getNick(), channelIt->name, channelIt->topic), this->writeFds);

                    }
                    else
                        client.newMessage(ERR_BADCHANNELKEY(channelIt->name), this->writeFds);
                }
            }
            else
                client.newMessage(ERR_NOSUCHCHANNEL(it->name), this->writeFds);
        }
    }
    return (1);
}