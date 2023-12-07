#include "Server.hpp"

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>


void        printDec(std::string str);
std::string stringTrim(char *buff);


// Server methods
std::list<Channel>::iterator  Server::getChannelIterator(const std::string &channelName)
{
   for (std::list<Channel>::iterator it = this->channels.begin(); it != this->channels.end(); it++)
   {
        if (it->name == channelName)
            return (it);
   }
   return (this->channels.end());
}

std::list<Client>::iterator   Server::getUserIterator(const std::string &nick)
{
    for (std::list<Client>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
    {
        if (it->compareNick(nick))
            return(it);
    }
    return (this->clients.end());
}

int     Server::addNewClient(fd_set &readFdsTmp)
{
    if (FD_ISSET(this->serverFd, &readFdsTmp))
    {
        Client              tmp;
        // struct  sockaddr    newClientInfo;
        // socklen_t           len;

        tmp.cliFd = accept(this->serverFd, NULL, NULL);
        // tmp.cliFd = accept(this->serverFd, &newClientInfo, &len);
        if (tmp.cliFd == -1)
            std::cout << "Client Error!" << std::endl;
        else
        {
            // char clientIP[INET_ADDRSTRLEN];

            std::cout << "New Client Connected!" << std::endl;
            this->clients.push_back(tmp);
            FD_SET(tmp.cliFd, &(this->readFds));
            // if (newClientInfo.sa_family == AF_INET)
            // {
            //     struct sockaddr_in *clientAddr = (struct sockaddr_in *)&newClientInfo;
            //     inet_ntop(AF_INET, &(clientAddr->sin_addr), clientIP, INET_ADDRSTRLEN);
            //     std::cout << clientIP << std::endl;
            // }
        }
        return (1);
    }
    return (0);
}

//constructor

Server::Server(int port, const char *pass) : serverPass(pass)
{
    //init commands
    this->functionsMap["PASS"] = &Server::pass;
    this->functionsMap["NICK"] = &Server::nick;
    this->functionsMap["USER"] = &Server::user;
    this->functionsMap["QUIT"] = &Server::quit;
    this->functionsMap["JOIN"] = &Server::join;
    this->functionsMap["PRIVMSG"] = &Server::privmsg;
    this->functionsMap["PART"] = &Server::part;
    this->functionsMap["TOPIC"] = &Server::topic;
    this->functionsMap["WHOIS"] = &Server::whois;
    this->functionsMap["MODE"] = &Server::mode;


    //other
    createSocket(port);
    FD_ZERO(&this->readFds);
    FD_ZERO(&this->writeFds);
    FD_SET(this->serverFd, &this->readFds);
    serverLoop();
}

int     Server::readFromClients(fd_set  &readFdsTmp)
{
    char    buffer[2048];//niye 2048 ?
    int     readByte;

    for (std::list<Client>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
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
                    break;
                if (buffer[readByte - 1] != '\n')
                {
                    it->buffer += buffer;
                    break;
                }
                std::stringstream  line(it->buffer + buffer);
                std::string        tmp;

                it->buffer.clear();
                printDec(line.str());
                while(getline(line, tmp, '\n'))
                {
                    std::stringstream   tokenStream(tmp);
                    std::string         command;
                    std::string         args;

                    tokenStream >> command;
                    args = stringTrim(&(tokenStream.str()[1 + tokenStream.tellg()]));
                    if (command == args)
                        args.clear();
                    if ((functionsMap[command]))
                    {
                        (this->*functionsMap[command])(args, *it);
                    }
                }
                // for (std::vector<Client>::iterator itt = clients.begin(); itt != clients.end(); itt++)
                // {
                //     if ((*it).cliFd != (*itt).cliFd && (*itt).auth == 1)
                //     {
                //         (*itt).incomingMessages.push_back(buffer);
                //         FD_SET((*itt).cliFd, &(this->writeFds));
                //     }
                // }
            }
            return (1);
        }
    }
    return (0);
}

int     Server::writeClients(fd_set &writeFdsTmp)
{
    int writeByte;

    for (std::list<Client>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
    {
        if (FD_ISSET(it->cliFd, &writeFdsTmp))
        {
            writeByte = write(it->cliFd, (it->incomingMessages.front()).c_str(), (it->incomingMessages.front()).length());
            if (writeByte < 0)
            {
                std::cout << "Msg Cannot Send!" << std::endl;
                FD_CLR(it->cliFd, &(this->readFds));
                FD_CLR(it->cliFd, &(this->writeFds));
                close(it->cliFd);
                this->clients.erase(it);
            }
            else
            {
                it->incomingMessages.pop();
                if (it->incomingMessages.empty())
                    FD_CLR(it->cliFd, &(this->writeFds));
            }
            return (1);
        }
    }
    return (0);
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
    if (listen(this->serverFd, 512) == -1)//neden 512
    {
        std::cout << "Listen Error!" << std::endl;
        close(this->serverFd);
        exit(1);
    }
}

void    Server::serverLoop(void)
{
    fd_set  readFdsTmp;
    fd_set  writeFdsTmp;

    while(1)
    {
        readFdsTmp = this->readFds;
        writeFdsTmp = this->writeFds;
        
        if (select(500, &readFdsTmp, &writeFdsTmp, NULL, NULL) == -1)//neden 500
            exit(1);
        if (addNewClient(readFdsTmp) || readFromClients(readFdsTmp) || writeClients(writeFdsTmp))
            continue;
    }
}