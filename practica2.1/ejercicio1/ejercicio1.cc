#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#include <iostream>

/*
argv[0] ---> nombre del programa
argv[1] ---> primer argumento (char *)

./addrinfo www.ucm.es 80
argv[0] = "./addrinfo"
argv[1] = "www.ucm.es"
argv[2] = "80"
|
|
V
res->ai_addr ---> (socket + bind)
|
|
V
host (numeric)

./addrinfo 127.0.0.1 80
./addrinfo www.ucm.es http
*/
int main(int argc, char **argv)
{
    struct addrinfo hints;
    struct addrinfo * res;

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_UNSPEC;

    int rc = getaddrinfo(argv[1], NULL, &hints, &res);

    if ( rc != 0 )
    {
        std::cerr << "Error: " << gai_strerror(rc) << std::endl;
        return -1;
    }

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    while( res != nullptr)
    {
        getnameinfo(res->ai_addr, res->ai_addrlen, host, NI_MAXHOST, service, 
            NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
        std::cout <<  host << " " << res->ai_family << " " << res->ai_socktype << std::endl;
        res = res->ai_next;
    }
        

    freeaddrinfo(res);

    return 0;
}