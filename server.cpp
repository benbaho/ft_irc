/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurmus <bdurmus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 18:43:17 by bdurmus           #+#    #+#             */
/*   Updated: 2023/11/15 20:37:41 by bdurmus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "server.hpp"
#include "client.hpp"
#include "sys/socket.h"
#include "unistd.h"
#include "netinet/in.h"
#include "sys/select.h"
#include "sys/types.h"
#include "vector"
#include "sstream"

Server::Server(int port, char *pass)
{
    createSocket(port);
    this->serverPass = pass;
    this->serverPass += "\n";
    serverLoop();
}

void Server::createSocket(int port)
{
    struct sockaddr_in addr;

    this->serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->serverFd == -1)
    {
        std::cout << "Socket Cannot Created!" << std::endl;
        exit(1);
    }

    int reuse = 1;
    if (setsockopt(this->serverFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        std::cerr << "Error setting SO_REUSEADDR option" << std::endl;
        close(this->serverFd);
        exit(1);
    }


    memset(&addr, 0, sizeof(addr));
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

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

void Server::serverLoop()
{
    int status;
    fd_set readFds;
    fd_set writeFds;
    int cliFd;
    std::vector<Client> clients;
    fd_set readFdsCopy;
    fd_set writeFdsCopy;
    char buffer[2048];
    int readed;

    FD_ZERO(&readFds);
    FD_ZERO(&writeFds);
    FD_SET(this->serverFd, &readFds);

    while (1)
    {
        readFdsCopy = readFds;
        writeFdsCopy = writeFds;
        status = select(500, &readFdsCopy, &writeFdsCopy, NULL, NULL);
        if (status == -1)
            exit(1);
        status = 1;
        if (FD_ISSET(this->serverFd, &readFdsCopy) && status)
        {
            Client tmp;
            cliFd = accept(this->serverFd, NULL, NULL);
            if (cliFd == -1)
                std::cout << "Client Error!" << std::endl;
            else
            {
                std::cout << "New Client Connected!" << std::endl;
                tmp.cliFd = cliFd;
                clients.push_back(tmp);
                FD_SET(tmp.cliFd, &readFds);
            }
            status = 0;
        }

        for (std::vector<Client>::iterator it = clients.begin(); it != clients.end() && status; it++)
        {
            if (FD_ISSET((*it).cliFd, &readFdsCopy))
            {
                readed = read((*it).cliFd, buffer, 2047);
                if (readed <= 0)
                {
                    std::cout << "Client Disconnected!" << std::endl;
                    FD_CLR((*it).cliFd, &readFds);
                    FD_CLR((*it).cliFd, &writeFds);
                    close((*it).cliFd);
                    clients.erase(it);
                }
                else
                {
                    buffer[readed] = 0;

                    if ((*it).auth == 0)
                    {
                        std::string tmp(buffer);
                        std::cout << "Auth Request: " << tmp << std::endl;
                        if (tmp.find("PASS ") != std::string::npos)
                        {
                            tmp = tmp.substr(5, tmp.length());
                            if (tmp == this->serverPass)
                            {
                                std::cout << "Auth Succesfully!" << std::endl;
                                (*it).auth = 1;
                                FD_SET((*it).cliFd, &writeFds);
                                (*it).incomingMessages.push_back("Auth Succesfully\n");
                            }
                            else
                            {
                                FD_SET((*it).cliFd, &writeFds);
                                (*it).incomingMessages.push_back("Auth Failed\n");
                            }
                        }
                    }
                    else
                    {
                        for (std::vector<Client>::iterator itt = clients.begin(); itt != clients.end(); itt++)
                        {
                            if ((*it).cliFd != (*itt).cliFd && (*itt).auth == 1)
                            {
                                (*itt).incomingMessages.push_back(buffer);
                                FD_SET((*itt).cliFd, &writeFds);
                            }
                        }
                    }
                }
                status = 0;
                break;
            }
        }


        for (std::vector<Client>::iterator it = clients.begin(); it != clients.end() && status; it++)
        {
            if (FD_ISSET((*it).cliFd, &writeFdsCopy))
            {
                readed = write((*it).cliFd, ((*it).incomingMessages.back()).c_str(), ((*it).incomingMessages.back()).length());
                if (readed < 0)
                {
                    std::cout << "Msg Cannot Send!" << std::endl;
                    FD_CLR((*it).cliFd, &readFds);
                    FD_CLR((*it).cliFd, &writeFds);
                    close((*it).cliFd);
                    clients.erase(it);
                }
                else
                {
                    (*it).incomingMessages.pop_back();
                    if ((*it).incomingMessages.empty())
                        FD_CLR((*it).cliFd, &writeFds);
                }
                    
                status = 0;
                break;
            }
        }


    }
}