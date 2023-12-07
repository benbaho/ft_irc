#pragma once

#include <iostream>
#include <vector>
#include <unistd.h>
#include <queue>
#include <sstream>

#include "Utils.hpp"

class Channel;

class Client
{       
    private:
        std::string                 pass;
        std::string                 nick;
        std::string                 userInfo;
        std::string                 userName;
        std::string                 hostName;
        std::string                 serverName;
        std::string                 realName;

    public:
        int                         cliFd;
        int                         auth;
        std::queue<std::string>     incomingMessages;
        std::vector<Channel *>      joinedChannels;
        std::string                 buffer;

        Client();
        ~Client();
        
        bool        compareNick(const std::string &username);
        void        newMessage(const std::string &message, fd_set &writeFds);
        int         setNick(std::string &username);

        bool        isRegistered();
        void        setUserInfo(std::vector<std::string> userInfoVec);
        
        void        removeJoinedChannel(const std::string &name);


        
        std::string getNick();
        std::string getUsername();
        std::string getHostname();
        std::string getRealname();
        std::string getServername();
        std::string getUserinfo();

        std::string getPrefix(std::string command, std::string args);
};