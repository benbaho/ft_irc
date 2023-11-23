#include "Server.hpp"


int Server::Nick(std::string &str, Client &client){
    if (str.empty()){
        FD_SET(client.cliFd, &writeFds);
        client.incomingMessages.push_back("ERR_NONICKNAMEGIVEN\n");
        return (0);
    }
    for (std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end() && (*it).auth; it++ ){
        if ((*it).getNickname() == str){
            FD_SET((*it).cliFd, &writeFds);
            (*it).incomingMessages.push_back("ERR_ERRONEUSNICKNAME\n");
            return (0);
        }
    }
    for (std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end() && (*it).auth == 0; it++ ){
        if ((*it).getNickname() == str){
            FD_SET((*it).cliFd, &writeFds);
            (*it).incomingMessages.push_back("ERR_NICKCOLLISION\n");
            return (0);
        }
    }
    if (client.auth == 1)
        client.setNickname(str);
    return (1);
}