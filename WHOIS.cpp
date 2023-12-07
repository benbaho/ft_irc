#include "Server.hpp"

//ERR_NONICKNAMEGIVEN [+] --> (Herhangi bir nick belirlenmedi veya uygun şartta verilmedi)
//RPL_WHOISUSER [+]--> (Whois sorgusu kullanıcı verisi cevabı)
//RPL_WHOISACCOUNT [+] --> (Whois sorgusu kanal verisi cevabı)
//RPL_NOSUCHNICK [+] --> (Nick bulunamadı)
//RPL_WHOISSERVER [+]--> (Whois sorgusu sunucu verisi cevabı)

int     Server::whois(std::string args, Client &client)
{
    std::stringstream ss(args);
    std::string tmp;
    int i = 0;
    
    while (ss >> tmp)
        i++;

    if (i == 0 || i > 2)
    {
        client.newMessage(ERR_NONICKNAMEGIVEN, this->writeFds);
        return (0);
    }
    else 
    {
        std::list<Client>::iterator it = this->getUserIterator(args.substr(0, args.find(' ')));
        if ((*it).compareNick(args.substr(0, args.find(' '))))
        {
            client.newMessage(RPL_WHOISUSER(client.getNick(), (*it).getNick(), (*it).getUsername(), (*it).getHostname(), (*it).getRealname()), this->writeFds);
            client.newMessage(RPL_WHOISSERVER(client.getNick(), (*it).getNick(), (*it).getHostname(),  std::string("alper")),  this->writeFds);
            client.newMessage(RPL_WHOISACCOUNT(client.getNick(), (*it).getNick(), (*it).getUsername()), this->writeFds);
            return (1);
        }
        else
        {
            client.newMessage(ERR_NOSUCHNICK(args.substr(0, args.find(' '))), this->writeFds);
            return (0);
        }
    }
}