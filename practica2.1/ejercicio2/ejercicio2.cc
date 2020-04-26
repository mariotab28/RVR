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

    if ( bind(sd, res->ai_addr, res->ai_addrlen) != 0 )
    {
        std::cerr << "bind: " << std::endl;
        return -1;
    }

    freeaddrinfo(res);

    // ---------------------------------------------------------------------- //
    // RECEPCIÓN MENSAJE DE CLIENTE //
    // ---------------------------------------------------------------------- //
    char buffer[80];
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    struct sockaddr client_addr;
    socklen_t client_len = sizeof(struct sockaddr);

    ssize_t bytes;
    while(buffer[0] != 'q' || bytes != 2)
    {
        memset(&buffer, '\0', 80);
        bytes = recvfrom(sd, buffer, 79 * sizeof(char), 0, &client_addr,
        &client_len);
        
        if ( bytes == -1)
        {
            std::cerr << "recvfrom: " << std::endl;
            return -1;
        }

        getnameinfo(&client_addr, client_len, host, NI_MAXHOST, service,
            NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

        std::cout << (int)bytes <<  " bytes de " << host << ":" << service
            << std::endl;


        char bufferOut[30];
        memset(&bufferOut, '\0', 30);
        time_t myTime = time(NULL);
        tm* timeData = localtime(&myTime);

        if(buffer[0] == 't' && bytes == 2) // Hora
        {
            strftime(bufferOut, 29, "%r", timeData);
            // RESPUESTA AL CLIENTE //
            sendto(sd, bufferOut, strlen(bufferOut), 0, &client_addr, client_len);
        }
        else if(buffer[0] == 'd' && bytes == 2) // Fecha
        {
            strftime(bufferOut, 29, "%F", timeData);
            // RESPUESTA AL CLIENTE //
            sendto(sd, bufferOut, strlen(bufferOut), 0, &client_addr, client_len);
        }
        else if(buffer[0] == 'q' && bytes == 2)
        {
            // RESPUESTA AL CLIENTE //
            sendto(sd, "", 1, 0, &client_addr, client_len);
        }
        else 
        {
            printf("Comando no soportado %s\n", buffer);
            // RESPUESTA AL CLIENTE //
            sendto(sd, "", 1, 0, &client_addr, client_len);
        }
    }

    return 0;
}