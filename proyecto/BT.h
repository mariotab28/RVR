#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>

#include "Serializable.h"
#include "Socket.h"

class BTMessage: public Serializable
{
public:
    static const size_t MESSAGE_SIZE = sizeof(char) * 88 + sizeof(uint8_t);

    enum MessageType
    {
        LOGIN   = 0,
        OBJECT = 1,
        LOGOUT  = 2
    };

    BTMessage(){};

    BTMessage(const std::string& n, const std::string& m):nick(n),message(m){};

    void to_bin(){};

    int from_bin(char * bobj){};

    uint8_t type;

    std::string nick;
    std::string message;
};

class BTServer
{
public:
    BTServer(const char * s, const char * p): socket(s, p)
    {
        socket.bind();
    };

    void do_messages();

private:
    std::vector<Socket *> clients;
    Socket socket;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class BTClient
{
public:
    BTClient(const char * s, const char * p, const char * n):socket(s, p),
        nick(n){};

    void login();
    void logout();

    void input_thread();
    void net_thread();

private:
    Socket socket;
    std::string nick;
};

