#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>

#include "Serializable.h"
#include "Socket.h"

namespace sf
{
    class RenderWindow;
    class Event;
    class Color;
}; // namespace sf

class GameWorld;

class BTMessage : public Serializable
{
public:
    size_t MESSAGE_SIZE = sizeof(char) * 8 + sizeof(uint8_t);

    enum MessageType
    {
        LOGIN = 0,
        OBJECT = 1,
        LOGOUT = 2
    };

    BTMessage(){};

    BTMessage(const std::string &n) : nick(n){};

    virtual void to_bin();

    virtual int from_bin(char *data);

    uint8_t type;
    std::string nick; // max. 8
};

// --------------------------------------------------------------------

class BTServer
{
public:
    BTServer(const char *s, const char *p) : socket(s, p)
    {
        socket.bind();
    };

    void start();

    void do_messages();
    void simulate();

private:
    // TODO: ELIMINAR VENTANA DEL SERVER!!!!
    sf::RenderWindow* window;
    sf::Color* bg;
    GameWorld* world;

    std::vector<Socket *> clients;
    Socket socket;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class BTClient
{
public:
    BTClient(const char *s, const char *p, const char *n) : socket(s, p),
                                                            nick(n){};

    void start();

    void login();
    void logout();

    void input_thread();
    void net_thread();
    //void render_thread();

private:
    sf::RenderWindow* window;
    sf::Color* bg;

    GameWorld* world;

    Socket socket;
    std::string nick;
};
