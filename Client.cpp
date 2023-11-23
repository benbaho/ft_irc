#include "Client.hpp"

Client::Client() {
    this->auth = 0;
}

void Client::setNickname(std::string s){
    this->nickname = s;
}
std::string Client::getNickname(){
    return this->nickname;
}

void Client::setUsername(std::string s){
    this->username = s;
}

std::string Client::getUsername(){
    return this->username;
}

Client::~Client(){}
