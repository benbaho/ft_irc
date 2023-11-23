
#include <vector>
#include <iostream>
#include <map>

#include "Client.hpp"

class Server
{
    private:
        typedef int (Server::*myFunc)(std::string&, Client &);
        int                                 serverFd;
        fd_set                              readFds;
        fd_set                              writeFds;

        const std::string                   serverPass;
    
    public:
        std::vector<Client>                 clients;

        Server(int port, const char *pass);
        void    createSocket(int port);
        void    serverLoop(void);
        
        int     cmpPass(const std::string pass);
        fd_set  getWriteFds(void);
        fd_set  getReadFds(void);
        
        //commands
        int     pass(std::string &str, Client &client);
        int     Nick(std::string &str, Client &client);
        int     user(std::string &str, Client &client);
        // void commandMap(std::string str);
        
        std::map<std::string, myFunc>       functionsMap;

};