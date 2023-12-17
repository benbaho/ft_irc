#include "Server.hpp"

// Parameters: <username> <hostname> <servername> <realname>

// Numeric Replies:

// ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED

std::vector<std::string> convertUserMsgVec(const std::string &args)
{
    std::stringstream           ss(args);
    std::vector<std::string>    userMsgVec;
    std::string                 tmp;
    std::string                 tmp2;

    tmp2 = args.substr(args.find(':') + 1, args.length());
    for (int i = 0; !ss.eof() && i <= 3; i++)
    {
        ss >> tmp;
        if (!tmp.empty() && tmp[0] == ':')
        {
            userMsgVec.push_back(tmp2);
            break;
        }
        userMsgVec.push_back(tmp);
    }
    return (userMsgVec);
}

int Server::user(std::string args, Client &client)
{
    (void)client;
    std::vector<std::string> userMsgVec = convertUserMsgVec(args);

    if (userMsgVec.size() < 4)
    {
        client.newMessage(ERR_NEEDMOREPARAMS(std::string("USER")), this->writeFds);
        return (0);
    }
    else if (!client.isRegistered())
    {
        client.newMessage(ERR_ALREADYREGISTRED(client.getNick()), this->writeFds);
        return (0);
    }
    client.setUserInfo(userMsgVec);
    if (!(client.getNick().empty()))
        this->info(args, client);
    return (1);
}
