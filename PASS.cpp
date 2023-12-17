#include "Server.hpp"

int Server::pass(std::string args, Client &client)
{
    std::stringstream   ss(args);

    ss >> args;

    if (args.empty())
    {
        client.newMessage(ERR_NEEDMOREPARAMS(std::string("PASS")), this->writeFds);
        return (0);
    }
    else if (client.auth)
    {
        client.newMessage(ERR_ALREADYREGISTRED(client.getNick()), this->writeFds);
        return (0);
    }
    else if (this->serverPass != args)
    {
        std::cout << "Auth Failed!" << std::endl;
        client.newMessage("Auth Failed\r\n", this->writeFds);
        return (0);
    }
    std::cout << "Auth Successfully!" << std::endl;
    client.auth = 1;
    return (1); 

}