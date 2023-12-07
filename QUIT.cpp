#include "Server.hpp"

int Server::quit(std::string args, Client &client) // check fd leaks
{
   std::stringstream ss(args);
   std::string       tmp;

   ss >> tmp;
   if (tmp.empty())
      tmp = "Leaving";
   
   for (std::vector<Channel *>::iterator it = client.joinedChannels.begin(); it != client.joinedChannels.end(); it++)
   {
      (*it)->sendMessageAllUsers(this->writeFds, client.cliFd,  RPL_QUIT(client.getNick(), client.getUsername(), tmp));
      //this->part(tmp +" " +(*it)->name, client); will be fixed
   }
   FD_CLR(client.cliFd, &writeFds);
   FD_CLR(client.cliFd, &readFds);
   close(client.cliFd);
   this->clients.erase(this->getUserIterator(client.getNick()));
   return (1);
}