#include "Server.hpp"

int Server::info(std::string args, Client &client)
{
    (void)args;
    std::string msg;
    
    msg += "--------------------------------------------------------------------------\n";
    msg += "--------------------------------WELCOME-----------------------------------\n";
    msg += "--------------------------------------------------------------------------\n";
    msg += "-----------------------------TEAM_RUSH_IRC--------------------------------\n";
    msg += "--------------------------------------------------------------------------\n";
    msg += "--This IRC project made by ademirci and bdurmus with love and patient <3--\n";
    msg += "--------------------------------------------------------------------------\n";
    msg += "---------------------------Coded with select()----------------------------\n",
    msg += "--------------------------------------------------------------------------\n";
    msg += "------------------------Based on RFC version 1459-------------------------\n";
    msg += "--------------------------------------------------------------------------";


    client.newMessage(RPL_INFO(client.getHostname(),client.getNick(), msg), this->writeFds);
    client.newMessage(RPL_ENDOFINFO(client.getHostname(),client.getNick()), this->writeFds);

    return (1);
}
