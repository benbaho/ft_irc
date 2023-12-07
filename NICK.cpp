#include "Server.hpp"

int Server::nick(std::string args, Client &client)
{
    std::stringstream               ss(args);
    ss >> args;

    std::list<Client>::iterator   it = this->getUserIterator(args);

    if (args.empty())
        client.newMessage(ERR_NONICKNAMEGIVEN, writeFds);
    else if (it != clients.end() && client.auth)
        client.newMessage(ERR_NICKNAMEINUSE(args), writeFds);
    else if (!client.setNick(args))
        client.newMessage(ERR_ERRONEUSNICKNAME(args), writeFds);
    return (1);
}
