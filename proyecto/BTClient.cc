#include <string>
#include <unistd.h>
#include <string.h>
#include "BT.h"
#include <SFML/Graphics.hpp>
#include <X11/Xlib.h>

void _client_thread(BTClient *ec)
{
    ec->net_thread();
}

int main(int argc, char **argv)
{
    XInitThreads();

    BTClient ec(argv[1], argv[2], argv[3]);
    ec.login();

    ec.wait();
    ec.start();

    // launch the net thread
    sf::Thread thread(&_client_thread, &ec);
    thread.launch();

    ec.input_thread();
}
