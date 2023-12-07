// > ilk kişi girdiğinde oluşacak ve son kişi çıktığında kapanacak
// > İsminde;
//      '&' ya da '#' karakterlerinden oluşacak biz # kullanacağız
//      max 200 karakter
//      ASCII 7, herhangi bir boşluk ve virgül içermeyecek
// > Kullanıcının yeni bir Channel'oluşturmak için bir Channel'ın üyesi olması gerekiyor ???
// > Channel'ı oluşturan kişi o Channel'ın modu olur. Channel'a yeni katılacak olan kişinin isteğini Channel'ın modu kabul/red edebilir.
// > Birden fazla mod olabilir. ?? 
// > Bir kullanıcı aynı anda birden fazla kanala katılabilir. (En fazla 10 kanala katılması öneriliyor)


// KICK    - Eject a client from the channel
// MODE    - Change the channel's mode
// INVITE  - Invite a client to an invite-only channel (mode +i)
// TOPIC   - Change the channel topic in a mode +t channel

// A channel operator is identified by the '@' symbol next to their
//    nickname whenever it is associated with a channel (ie replies to the
//    NAMES, WHO and WHOIS commands).
#pragma once
#include "Client.hpp"
#include "Server.hpp"

class Client;

class Channel
{
    private:
        std::string                     key;

    public:
        std::string                     name;   //private a alınabilir
        std::string                     topic;
        std::vector<Client *>           users;
        std::vector<Client *>           operators;
        bool                            isInviteOnly;
    
        int                             compareKeys(const std::string &key);
        bool                            isOperator(const int &clientFd);
        void                            setKey(const std::string &key);
        int                             removeUser(const int &cliFd);

        std::vector<Client *>::iterator findUser(const int &cliFd);
        std::string                     getUserList();
        void                            updateUserList(fd_set &writeFds);
        void                            sendMessageAllUsers(fd_set &writeFds, const int &cliFd, const std::string &message);
};

typedef struct ChannelToken
{
    std::string name;
    std::string key;
}   ChannelToken;
