#include "Server.hpp"

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>
#include <sstream>

void    printDec(std::string str);

Server::Server(int port, const char *pass) : serverPass(pass)
{
    //init commands
    this->functionsMap["PASS"] = &Server::pass;
    this->functionsMap["NICK"] = &Server::Nick;
    this->functionsMap["USER"] = &Server::user;

    //other
    createSocket(port);
    FD_ZERO(&this->readFds);
    FD_ZERO(&this->writeFds);
    FD_SET(this->serverFd, &this->readFds);
    serverLoop();
}

void    Server::createSocket(int port)
{
    struct  sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    this->serverFd = socket(AF_INET, SOCK_STREAM, 0);
    int reuse = 1;

    if (this->serverFd == -1)
    {
        std::cout << "Socket Cannot Created!" << std::endl;
        exit(1);
    }
    if (setsockopt(this->serverFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
        std::cerr << "Error setting SO_REUSEADDR option" << std::endl;
        close(this->serverFd);
        exit(1);
    }

    if (bind(this->serverFd, (sockaddr *)&addr, sizeof(addr)) == -1)
    {
        std::cout << "Bind Error!" << std::endl;
        close(this->serverFd);
        exit(1);
    }

    if (listen(this->serverFd, 512) == -1)
    {
        std::cout << "Listen Error!" << std::endl;
        close(this->serverFd);
        exit(1);
    }
}

void    Server::serverLoop(void)
{
    int     status;
    int     cliFd;

    fd_set  readFdsTmp;
    fd_set  writeFdsTmp;

    char    buffer[2048];
    int     readByte;

    while(1)
    {
        readFdsTmp = this->readFds;
        writeFdsTmp = this->writeFds;
        
        status = select(500, &readFdsTmp, &writeFdsTmp, NULL, NULL);
        if (status == -1)
            exit(1);
        status = 1;
        if (FD_ISSET(this->serverFd, &readFdsTmp) && status)
        {
            Client tmp;

            tmp.cliFd = accept(this->serverFd, NULL, NULL);
            if (tmp.cliFd == -1)
                std::cout << "Client Error!" << std::endl;
            else
            {
                std::cout << "New Client Connected!" << std::endl;
                clients.push_back(tmp);
                FD_SET(tmp.cliFd, &(this->readFds));
            }
            status = 0;
            continue;
        }

        for (std::vector<Client>::iterator it = clients.begin(); it != clients.end() && status; it++)
        {
            if (FD_ISSET(it->cliFd, &readFdsTmp))
            {
                readByte = read(it->cliFd, buffer, 2047);
                if (readByte <= 0)
                {
                    std::cout << "Client Disconnected!" << std::endl;
                    FD_CLR(it->cliFd, &(this->readFds));
                    FD_CLR(it->cliFd, &(this->writeFds));
                    close(it->cliFd);
                    clients.erase(it);
                }
                else
                {
                    buffer[readByte] = 0;

                    if (buffer[0] == '\n')
                    {
                        status = 1;
                        break;
                    }
                    if (buffer[readByte - 1] != '\n')
                    {
                        it->buffer += buffer;
                        status = 1;
                        break;
                    }

                    // std::cout << buffer << "--" << std::endl;
                    std::stringstream  line(buffer);
                    std::string        tmp(it->buffer + buffer);

                    it->buffer.clear();
                    printDec(tmp);
                    while(getline(line, tmp, '\n'))
                    {
                        std::stringstream  word(tmp);
                        std::string        command;
                        std::string        a;

                        word >> command;
                        tmp.clear();
                        word >> tmp;
                        if ((functionsMap[command]))
                            (this->*functionsMap[command])(tmp, *it);
                    }
                    for (std::vector<Client>::iterator itt = clients.begin(); itt != clients.end(); itt++)
                    {
                        if ((*it).cliFd != (*itt).cliFd && (*itt).auth == 1)
                        {
                            (*itt).incomingMessages.push_back(buffer);
                            FD_SET((*itt).cliFd, &(this->writeFds));
                        }
                    }
                    // std::cout << it->getNickname() << std::endl;
                }
                status = 0;
                break;
            }
        }
        for (std::vector<Client>::iterator it = clients.begin(); it != clients.end() && status; it++)
        {
            if (FD_ISSET((*it).cliFd, &writeFdsTmp))
            {
                readByte = write((*it).cliFd, ((*it).incomingMessages.back()).c_str(), ((*it).incomingMessages.back()).length());
                if (readByte < 0)
                {
                    std::cout << "Msg Cannot Send!" << std::endl;
                    FD_CLR((*it).cliFd, &(this->readFds));
                    FD_CLR((*it).cliFd, &(this->writeFds));
                    close((*it).cliFd);
                    clients.erase(it);
                }
                else
                {
                    (*it).incomingMessages.pop_back();
                    if ((*it).incomingMessages.empty())
                        FD_CLR((*it).cliFd, &(this->writeFds));
                }
                    
                status = 0;
                break;
            }
        }


    }
}