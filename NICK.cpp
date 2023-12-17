#include "Server.hpp"

int Server::nick(std::string args, Client &client)
{
    std::stringstream               ss(args);
    ss >> args;

    std::list<Client>::iterator   it = this->getUserIterator(args);

    if (args.empty())
    {
        client.newMessage(ERR_NONICKNAMEGIVEN, writeFds);
        return (0);
    }
    else if (it != clients.end() && client.auth)
    {
        client.newMessage(ERR_NICKNAMEINUSE(args), writeFds);
        return (0);
    }
    else if (!client.setNick(args, writeFds))
    {
        client.newMessage(ERR_ERRONEUSNICKNAME(args), writeFds);
        return (0);
    }
    if (!(client.getUsername().empty()))
        this->info(args, client);
    return (1);
}
