#include <string>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include "Serializable.h"
#include "Socket.h"
#include "BT.h"

#include <SFML/Graphics.hpp>
#include <X11/Xlib.h>  

void _server_thread(BTServer* es)
{
    es->do_messages();
}

int main(int argc, char **argv)
{
    XInitThreads();

    sigset_t waitset;
    int      sig;

    BTServer es(argv[1], argv[2]);

    es.start();

    // launch the server thread
    sf::Thread thread(&_server_thread, &es);
    thread.launch();

    es.simulate();
    //es.do_messages();

    sigemptyset(&waitset);
    sigaddset(&waitset, SIGQUIT);

    sigwait(&waitset, &sig);

    return 0;
}
