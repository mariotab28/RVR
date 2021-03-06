#include "BTMessage.h"

void BTMessage::to_bin()
{
    alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);

    // serializar message type
    memcpy(_data, static_cast<void *>(&type), sizeof(uint8_t));
    _data += sizeof(uint8_t);

    // serializar nick
    memcpy(_data, static_cast<void *>((char *)nick.c_str()), 8 * sizeof(char));
    _data += 8 * sizeof(char);

    // INPUT INFO-------------------------

    // serializar index
    memcpy(_data, static_cast<void *>(&index), sizeof(uint8_t));
    _data += sizeof(uint8_t);

    // serializar message
    memcpy(_data, static_cast<void *>((char *)message.c_str()), 12 * sizeof(char));
    _data += 12 * sizeof(char);

    // colocamos el puntero al inicio del _data
    _data -= MESSAGE_SIZE;
}

int BTMessage::from_bin(char *data)
{
    try
    {
        // deserializamos type
        memcpy(static_cast<void *>(&type), data, sizeof(uint8_t));
        data += sizeof(uint8_t);

        // deserializamos nick
        char auxNick[8];
        memcpy(static_cast<void *>(&auxNick), data, 8 * sizeof(char));
        data += 8 * sizeof(char);
        auxNick[7] = '\0';
        nick = auxNick;

        // INPUT INFO-------------------------

        // deserializamos type
        memcpy(static_cast<void *>(&index), data, sizeof(uint8_t));
        data += sizeof(uint8_t);

        // deserializamos message
        char auxMessage[12];
        memcpy(static_cast<void *>(&auxMessage), data, 12 * sizeof(char));
        data += 12 * sizeof(char);
        auxMessage[11] = '\0';
        message = auxMessage;

        // colocamos el puntero al inicio del _data
        //data -= MESSAGE_SIZE;

        return 0;
    }
    catch (const std::exception &e)
    {
        return -1;
    }
}