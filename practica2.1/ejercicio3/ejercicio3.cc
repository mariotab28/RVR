// AUTORES:
// Arturo García Cárdenas
// Mario Tabasco Vargas

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

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

    // ---------------------------------------------------------------------- //
    // INICIALIZACIÓN SOCKET & BIND //
    // ---------------------------------------------------------------------- //

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

    if ( rc != 0 )
    {
        std::cerr << "getaddrinfo: " << gai_strerror(rc) << std::endl;
        return -1;
    }

    // res contiene la representación como sockaddr de dirección + puerto

    int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);


    freeaddrinfo(res);

      // ----------------------------------------------------------------------//
     //         ENVIAR MENSAJE AL SERVIDOR                                    //
    // ----------------------------------------------------------------------//
    char buffer[80];
    memset(&buffer, '\0', 80);
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    struct sockaddr client_addr;
    socklen_t client_len = sizeof(struct sockaddr);

    ssize_t bytes;

    sendto(sd, argv[3], strlen(argv[3]) + 1, 0, res->ai_addr, res->ai_addrlen);
    
      // ----------------------------------------------------------------------//
     //         RECIBIR MENSAJE DEL SERVIDOR                                  //
    // ----------------------------------------------------------------------//
    bytes = recvfrom(sd, buffer, 79 * sizeof(char), 0, res->ai_addr, &res->ai_addrlen);
    
    // Escribo lo que me envía el servidor
    printf("%s\n", buffer);


    return 0;
}