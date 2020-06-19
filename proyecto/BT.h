#include <string>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <queue>
#include <mutex>

#include <SFML/Graphics.hpp>

#include "BTMessage.h"
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

static float FPS = 30;              // 30 Hz
static float tickRate = 1000 / FPS; // 33.33 ms

class BTServer
{
public:
    BTServer(const char *s, const char *p, const char *l) : socket(s, p)
    {
        socket.bind();
        if (l != nullptr)
        {
            std::stringstream ss(l);
            ss >> level;
        }
        else
            printf("ATENCION: Ningún nivel elegido!\nEscribir 0,1,2 como 3º arg para elegir el nivel\n");
    };

    void start();
    void do_messages();
    void simulate();

private:
    std::mutex m;
    
    sf::RenderWindow *window;
    sf::Color *bg;
    GameWorld *world;
    
    sf::Clock *clock;
    sf::Time elapsedTime;
    int level;

    std::vector<Socket *> clients;
    Socket socket;
    std::vector<sf::Thread *> clientThreads;

    std::queue<BTMessage> inputMessages;
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
    sf::RenderWindow *window;
    sf::Color *bg;

    GameWorld *world;

    Socket socket;
    std::string nick;

    int index;

    sf::Clock *clock;
    sf::Time elapsedTime;
};
