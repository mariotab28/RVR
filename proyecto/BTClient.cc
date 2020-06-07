#include <string>
#include <unistd.h>
#include <string.h>
#include "BT.h"
#include <SFML/Graphics.hpp>
#include <X11/Xlib.h>  

void _client_thread(BTClient* ec)
{
    ec->net_thread();
}

int main(int argc, char **argv)
{
    XInitThreads();

    BTClient ec(argv[1], argv[2], argv[3]);

    // create window, load resources
    ec.start();

    // net thread
    /*pthread_attr_t attr;
    pthread_t id;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(&id, &attr, _client_thread, static_cast<void *>(&ec));*/

    // launch the net thread
    sf::Thread thread(&_client_thread, &ec);
    thread.launch();

    ec.login();

    ec.input_thread();
}

