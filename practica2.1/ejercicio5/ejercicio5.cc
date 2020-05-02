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
    hints.ai_socktype = SOCK_STREAM;

    int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

    if ( rc != 0 )
    {
        std::cerr << "getaddrinfo: " << gai_strerror(rc) << std::endl;
        return -1;
    }

    // res contiene la representación como sockaddr de dirección + puerto

    int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(sd == -1)
        printf("Error al crear el socket.\n");


    freeaddrinfo(res);

      // ----------------------------------------------------------------------//
     //         ENVIAR MENSAJE AL SERVIDOR                                    //
    // ----------------------------------------------------------------------//
    char buffer[80];
    memset(&buffer, '\0', 80);

    ssize_t bytes;

    if(connect(sd, res->ai_addr, res->ai_addrlen) != 0)
        printf("Error al conectar.\n");

    while(strcmp(buffer, "Q") != 0)
    {
        std::cin >> buffer;
        if(strcmp(buffer, "Q") != 0)
        {
            send(sd, (void *) buffer, sizeof(char)*79, 0);
        
            // ----------------------------------------------------------------------//
            //         RECIBIR MENSAJE DEL SERVIDOR                                  //
            // ----------------------------------------------------------------------//
            bytes = recv(sd, (void *) buffer, 79 * sizeof(char), 0);
            
            // Escribo lo que me envía el servidor
            buffer[bytes] = '\0';
            printf("%s\n", buffer);
        }
        
    }

    return 0;
}