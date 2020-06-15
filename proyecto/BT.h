#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Socket.h"

namespace sf
{
    class RenderWindow;
    class Event;
    class Color;
    class Clock;
    class Time;
}; // namespace sf

class GameWorld;

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

    float time;
    //float deltaTime;
    sf::Clock* clock;
    sf::Time elapsedTime;

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

    void wait();

private:
    sf::RenderWindow* window;
    sf::Color* bg;

    GameWorld* world;

    Socket socket;
    std::string nick;

    int index;

    float time;
    float deltaTime;
    sf::Clock* clock;
    sf::Time elapsedTime;
};
