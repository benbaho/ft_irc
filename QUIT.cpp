#include "Server.hpp"

int Server::quit(std::string args, Client &client) 
{
   std::stringstream ss(args);
   std::string       tmp;

   ss >> tmp;
   if (tmp.empty())
      tmp = "Leaving";
   for (std::vector<Channel *>::iterator it = client.joinedChannels.begin(); it != client.joinedChannels.end(); it++)
      (*it)->sendMessageAllUsers(this->writeFds, client.cliFd,  QUIT(client.getPrefix(), tmp));
   FD_CLR(client.cliFd, &writeFds);
   FD_CLR(client.cliFd, &readFds);
   close(client.cliFd);
   this->clients.erase(this->getUserIterator(client.getNick()));

   return (1);
}