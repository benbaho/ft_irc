#include "Server.hpp"

int Server::pass(std::string &str, Client &client)
{
    if (str.empty())
    {
        FD_SET(client.cliFd, &writeFds);
        client.incomingMessages.push_back("ERR_NEEDMOREPARAMS");
        return (0);
    }
    else if (client.auth == 1)
    {
        FD_SET(client.cliFd, &writeFds);
        client.incomingMessages.push_back("ERR_ALREADYREGISTRED");
        return (0);
    }
    else if (this->serverPass == str)
    {
        std::cout << "Auth Successfully!" << std::endl;
        client.auth = 1;
        FD_SET(client.cliFd, &writeFds);
        client.incomingMessages.push_back("Auth Succesfully!\n");
        return (1);
    }
    else
    {
        std::cout << "Auth Failed!" << std::endl;
        FD_SET(client.cliFd, &writeFds);
        client.incomingMessages.push_back("Auth Failed\n");
        return (0);
    }
}