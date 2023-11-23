#include "Server.hpp"

int Server::user(std::string &str, Client &client){ // username var mı yok mu
    if (str.empty()){
        FD_SET(client.cliFd, &writeFds);
        client.incomingMessages.push_back("ERR_ALREADYREGISTRED\n");
        return (0);
    }
    if (client.auth == 0){
        client.setUsername(str);
        return (1);
    }
    return (0);
}