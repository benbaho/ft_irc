#include "Server.hpp"


// ERR_NEEDMOREPARAMS --> (Daha fazla parametre gerekiyor)	[+]
// ERR_NOTONCHANNEL --> (Kanalda değilsiniz)				[+]
// RPL_NOTOPIC --> (Kanalda başlık yoktur)					[+]
// RPL_TOPIC --> (Başlık mesajı verilir)					[+]
// ERR_CHANOPRIVSNEEDED --> (Kanal operatörü değilsiniz)	[+]

int Server::topic(std::string args, Client &client)
{
	std::stringstream               ss(args);
	std::string						tmp;
	std::list<Channel>::iterator  channelIt;

	ss >> tmp;
	
	if (tmp.empty())
	{
		client.newMessage(ERR_NEEDMOREPARAMS(std::string("TOPIC")), this->writeFds);
		return(0);
	}

 	channelIt = this->getChannelIterator(tmp);
	if (channelIt != this->channels.end())
	{
		if (channelIt->findUser(client.cliFd) == channelIt->users.end())
			client.newMessage(ERR_NOTONCHANNEL(channelIt->name), this->writeFds);
		else if (!ss.eof())
		{
			ss >> tmp;
			if (channelIt->isOperator(client.cliFd))
				channelIt->topic = tmp;
			else
				client.newMessage(ERR_CHANOPRIVSNEEDED(client.getNick(), channelIt->name), this->writeFds);
		}
		else if (channelIt->topic.empty() && ss.eof())
			client.newMessage(RPL_NOTOPIC(client.getNick(), channelIt->name), this->writeFds);
		else if (!channelIt->topic.empty() && ss.eof())
			client.newMessage(RPL_TOPIC(client.getNick(), channelIt->name, channelIt->topic), this->writeFds);
	}
	return (0);	
}