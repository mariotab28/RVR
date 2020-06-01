#include "BT.h"

void BTServer::do_messages()
{
    while (true)
    {
        //Recibir Mensajes en y en función del tipo de mensaje
        // - LOGIN: Añadir al vector clients
        // - LOGOUT: Eliminar del vector clients
        // - MESSAGE: Reenviar el mensaje a todos los clientes (menos el emisor)

        Socket* client;
        BTMessage message;

        if(socket.recv(message,client) == -1)
        {
            std::cout << "ERROR recv -1\n";
        }

        switch (message.type)
        {
            case BTMessage::LOGIN:
            {
                std::cout << "LOGIN " << *client<< "\n";
                // añadir el client al vector clients
                clients.push_back(client);
                break;
            }
            case BTMessage::LOGOUT:
            {
                std::cout << "LOGOUT " << *client<< "\n";
                // buscar si existe el socket client en el vector clients
                for(auto it = clients.begin(); it != clients.end(); ++it)
                {
                    if(*(*it) == *client)
                    {
                        delete *it;
                        clients.erase(it);
                        break;
                    }
                }

                break;
            }
            case BTMessage::MESSAGE:
            {
                // comprobar que el client este logeado (i.e. este en el vector
                // clients)
                std::cout << "MESSAGE " << *client<< "\n";
                for (auto it = clients.begin(); it != clients.end(); ++it)
                {
                    if(!(*(*it) == *client))
                    {
                        std::cout << "ENVIANDO A " << *(*it)<< "\n";
                        socket.send(message, *(*it));
                    }
                }
                
                break;
            }
            default:
                break;
        }

        std::cout << "CLIENTS CONNECTED: " << clients.size() << "\n";
        std::cout << "TYPE: " << (int) message.type << " NICK: "<<  message.nick  << " MESSAGE: " << message.message << "\n";
    }
}

// --------------------------------------------------------------------

void BTClient::login()
{
    std::string msg;

    BTMessage em(nick, msg);
    em.type = BTMessage::LOGIN;

    socket.send(em, socket);
}

void BTClient::logout()
{
    std::string msg;

    BTMessage em(nick, msg);
    em.type = BTMessage::LOGOUT;

    socket.send(em, socket);
}

void BTClient::input_thread()
{
    while (true)
    {
        // Leer stdin con std::getline
        // Enviar al servidor usando socket
        std::string msg;
        std::getline(std::cin, msg);

        // Si el cliente pulsa 'q' cierra su conexión
        if(msg == "q")
            break;

        BTMessage em(nick, msg);
        em.type = BTMessage::MESSAGE;

        socket.send(em, socket);
    }

    logout();
}

void BTClient::net_thread()
{
    while(true)
    {
        //Recibir Mensajes de red
        //Mostrar en pantalla el mensaje de la forma "nick: mensaje"
        BTMessage msg;

        socket.recv(msg);

        std::cout << msg.nick.c_str() << ": " << msg.message.c_str() << "\n";
    }
}