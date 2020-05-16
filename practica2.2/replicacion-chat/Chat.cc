#include "Chat.h"

void ChatMessage::to_bin()
{
    alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);

    type = 1;

    //Serializar los campos type, nick y message en el buffer _data

    // serializar type
    memcpy(_data, static_cast<void*>(&type), sizeof(uint8_t));
    _data += sizeof(uint8_t);

    // serializar nick
    memcpy(_data, static_cast<void*>((char*)nick.c_str()), 8*sizeof(char));
    _data += 8*sizeof(char);

    // serializar mensaje
    memcpy(_data, static_cast<void*>((char*)message.c_str()), 80*sizeof(char));
    _data += 80*sizeof(char);

    // colocamos el puntero al inicio del fichero
    _data -= MESSAGE_SIZE;
}

int ChatMessage::from_bin(char * bobj)
{
    try
    {
        /*alloc_data(MESSAGE_SIZE);

        memcpy(static_cast<void *>(_data), bobj, MESSAGE_SIZE);*/

        //Reconstruir la clase usando el buffer _data

        // deserializamos type
        memcpy(static_cast<void*>(&type), bobj, sizeof(uint8_t));
        bobj += sizeof(uint8_t);

        // deserializamos nick
        //nick = std::string("", 8);
        char auxNick[8];
        memcpy(static_cast<void*>(&auxNick), bobj, 8*sizeof(char));
        bobj += 8*sizeof(char);
        nick = auxNick;

        // deserializamos message
        char auxMessage[80];
        memcpy(static_cast<void*>(&auxMessage), bobj, 80*sizeof(char));
        bobj += 80*sizeof(char);
        message = auxMessage;

        /*std::cout << nick << std::endl;
        std::cout << message << std::endl;
        printf("TYPE: %d\n", type);*/


        return 0;
    }
    catch(const std::exception& e)
    {
        return -1;
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatServer::do_messages()
{
    while (true)
    {
        //Recibir Mensajes en y en función del tipo de mensaje
        // - LOGIN: Añadir al vector clients
        // - LOGOUT: Eliminar del vector clients
        // - MESSAGE: Reenviar el mensaje a todos los clientes (menos el emisor)

        /*for (int i = 0; i < clients; i++)
        {
            socket.recv(, clients[i]);
        }*/
        
        Socket* client;
        ChatMessage message;

        if(socket.recv(message,client) == -1)
        {
            printf("recv -1\n");
        }

        switch (message.type)
        {
        case ChatMessage::LOGIN:
        {
        
            // añadir el client al vector clients
            printf("LOGIN: %s\n", message.nick.c_str());
            clients.push_back(client);
            break;
        }
        case ChatMessage::LOGOUT:
        {
            // buscar si existe el socket client en el vector clients
            printf("LOGOUT: %s\n", message.nick.c_str());
            int i=0;
            while(i<clients.size() && !(clients[i] == client))
                i++;

            // si lo encuentra, lo borra
            if(i <clients.size())
            {
                printf("BORRADO: %s\n", message.nick.c_str());
                clients.erase(clients.begin() + i);
            }
            
            break;
        }
        case ChatMessage::MESSAGE:
        {
            // comprobar que el client este logeado (i.e. este en el vector
            // clients)
            printf("MESSAGE: %s\n", message.nick.c_str());
            int i=0;
            while(i<clients.size() && !(clients[i] == client))
                i++;

            // si lo está hacer send del message a todos los
            // clients menos al client
            if(i <clients.size())
            {
                printf("ENVIADO: %s\n", message.nick.c_str());
                for (int j = 0; j < clients.size(); j++)
                {
                    if(j!=i)
                    {
                        
                        socket.send(message, *clients[j]);
                    }
                }
            }
            
            break;
        }
        default:
            break;
        }
    }
}

void ChatClient::login()
{
    std::string msg;

    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGIN;

    socket.send(em, socket);
}

void ChatClient::logout()
{
    std::string msg;

    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGOUT;

    socket.send(em, socket);
}

void ChatClient::input_thread()
{
    while (true)
    {
        // Leer stdin con std::getline
        // Enviar al servidor usando socket
        std::string msg;
        std::getline(std::cin, msg);

        ChatMessage em(nick, msg);
        em.type = ChatMessage::MESSAGE;

        socket.send(em, socket);
    }

    logout();
}

void ChatClient::net_thread()
{
    while(true)
    {
        //Recibir Mensajes de red
        //Mostrar en pantalla el mensaje de la forma "nick: mensaje"
        //Socket* client;
        ChatMessage msg;

        socket.recv(msg);

        printf("%s: %s\n", msg.nick.c_str(), msg.message.c_str());
    }
}


/*int main(int argc, char **argv)
{
    // SERIALIZAR---
    ChatMessage one("art", "buenas tardes");
    one.to_bin();

    // DESERIALIZAR---
    ChatMessage second("","");

    second.from_bin(one.data());

    return 0;
}*/