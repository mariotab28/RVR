#include <string>
#include <unistd.h>
#include <string.h>
#include "BT.h"

extern "C" void * _client_thread(void *arg)
{
    BTClient * server = static_cast<BTClient *>(arg);

    server->net_thread();

    return 0;
}

extern "C" void * _client_thread2(void *arg)
{
    BTClient * server = static_cast<BTClient *>(arg);

    server->render_thread();

    return 0;
}

int main(int argc, char **argv)
{
    BTClient ec(argv[1], argv[2], argv[3]);

    ec.start();
    
    // net thread
    pthread_attr_t attr;
    pthread_t id;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(&id, &attr, _client_thread, static_cast<void *>(&ec));

    // render thread
    pthread_attr_t attr2;
    pthread_t id2;

    pthread_attr_init(&attr2);
    pthread_attr_setdetachstate(&attr2, PTHREAD_CREATE_DETACHED);

    pthread_create(&id2, &attr2, _client_thread2, static_cast<void *>(&ec));

    ec.login();

    ec.input_thread();
}

