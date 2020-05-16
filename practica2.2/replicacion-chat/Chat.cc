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

}

void ChatClient::input_thread()
{
    while (true)
    {
        // Leer stdin con std::getline
        // Enviar al servidor usando socket
    }
}

void ChatClient::net_thread()
{
    while(true)
    {
        //Recibir Mensajes de red
        //Mostrar en pantalla el mensaje de la forma "nick: mensaje"
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