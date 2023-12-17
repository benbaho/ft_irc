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


#define WITH_MSG_SENDER -1


class Server;
class Client;

        typedef int (Channel::*modeFunction)(std::string args, bool type);
class Channel
{
    private:
        std::string                     key;

    public:
        std::string                     name;   //private a alınabilir
        std::string                     topic;
        std::vector<Client *>           users;
        std::vector<Client *>           operators;
        std::vector<Client *>           invitedUsers;
        std::vector<Client *>           voicePermittedUsers;
        std::map<char, modeFunction>    modeFunctionsMap;

        std::string                     updatedModesBuff;
        size_t                          userLimit;
        bool                            isInviteOnly;
        bool                            isTopicOperatorOnly;
        bool                            isMessageOnlyChUsers;
        bool                            isMessageOnlyOperators;
        bool                            isPrivate;
        bool                            isSecret;
        
        Channel();
    
        int                             sizeOfChannel();
        int                             compareKeys(const std::string &key);
        bool                            isOperator(const int &clientFd);
        bool                            isInvitedUser(const int &clientFd);
        bool                            isVoicePermittedUser(const int &clientFd);
        void                            setKey(const std::string &key);
        int                             removeUser(const int &cliFd);
        int                             removeOperator(const int &cliFd);
        int                             removeInvited(const int &cliFd);
        int                             removeVoicePermittedUser(const int &clientFd);

        std::vector<Client *>::iterator findUser(const int &cliFd);
        std::vector<Client *>::iterator findUser(const std::string &nick);
        std::string                     getUserList();
        std::string                     getChannelSize();
        void                            updateUserList(fd_set &writeFds);
        void                            sendMessageAllUsers(fd_set &writeFds, const int &cliFd, const std::string &message);

        std::string                     getChannelModes(void);

        int                             modeL(std::string args, bool type);
        int                             modeO(std::string args, bool type);
        int                             modeT(std::string args, bool type);
        int                             modeI(std::string args, bool type);
        int                             modeK(std::string args, bool type);
        int                             modeN(std::string args, bool type);
        int                             modeM(std::string args, bool type);
        int                             modeV(std::string args, bool type);
        int                             modeP(std::string args, bool type);
        int                             modeS(std::string args, bool type);
};

typedef struct ChannelToken
{
    std::string name;
    std::string key;
}   ChannelToken;
