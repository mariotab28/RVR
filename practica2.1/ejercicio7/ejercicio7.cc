/*

Código TCP multithread (accept-fork)


*/


#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
#include <thread>
#include <vector>

/*
./tcp 0.0.0.0 7777

class ConexionTCP
{
public:


private:


};
*/

class MyThread
{
private:
    int sd;
    int id;
public:
    MyThread(int sd, int id);
    ~MyThread();

    void receive()
    {
        // ---------------------------------------------------------------------- //
        // GESTION DE LA CONEXION CLIENTE //
        // ---------------------------------------------------------------------- //
        ssize_t bytes;
        do
        {
            char buffer[80];

            bytes = recv(sd, (void *) buffer, sizeof(char)*79, 0);
            if ( bytes <= 0 )
            {
                std::cout << "Thread ID: " << id << " -> Conexión terminada" << std::endl;
            }

            send(sd, (void *) buffer, bytes, 0);
        } while(bytes > 0);

    }
    
};

MyThread::MyThread(int sd, int id) : sd(sd), id(id)
{
}

MyThread::~MyThread()
{
    std::cout << "Thread destruido" << std::endl;
}


int main(int argc, char **argv)
{
    struct addrinfo hints;
    struct addrinfo * res;

    // ---------------------------------------------------------------------- //
    // INICIALIZACIÓN SOCKET & BIND //
    // ---------------------------------------------------------------------- //

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

    if ( rc != 0 )
    {
        std::cerr << "getaddrinfo: " << gai_strerror(rc) << std::endl;
        return -1;
    }

    // res contiene la representación como sockaddr de dirección + puerto

    int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if ( bind(sd, res->ai_addr, res->ai_addrlen) != 0 )
    {
        std::cerr << "bind: " << std::endl;
        return -1;
    }

    freeaddrinfo(res);

    // ---------------------------------------------------------------------- //
    // PUBLICAR EL SERVIDOR (LISTEN) //
    // ---------------------------------------------------------------------- //
    listen(sd, 16);

    // ---------------------------------------------------------------------- //
    // GESTION DE LAS CONEXIONES AL SERVIDOR //
    // ---------------------------------------------------------------------- //
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    int th_id = 0;

    std::vector<std::thread> pool_thread;

    while(true)
    {
        struct sockaddr client_addr;
        socklen_t client_len = sizeof(struct sockaddr);

        // Thread ppal se bloqua en accept
        int sd_client = accept(sd, &client_addr, &client_len);

        getnameinfo(&client_addr, client_len, host, NI_MAXHOST, service,
        NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

        std::cout << "CONEXION DESDE IP: " << host << " PUERTO: " << service
            << std::endl << "Creación de thread: " << th_id << std::endl;

        //Crear thread para tratar cada conexion
        MyThread* thread = new MyThread(sd_client, th_id++);
        std::thread conexionthr(&MyThread::receive, thread);

        conexionthr.detach();
    }

    return 0;
}