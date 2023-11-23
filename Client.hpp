#pragma once

#include <iostream>
#include <vector>

class Client
{       
    private:
        std::string                 pass;
        std::string                 nickname;
        std::string                 username;

    public:
        int                         cliFd;
        int                         auth;
        std::vector<std::string>    incomingMessages;
        std::string                 buffer;

        Client();
        ~Client();
        
        void setNickname(std::string s);
        std::string getNickname();
        
        void setUsername(std::string s);
        std::string getUsername();

};