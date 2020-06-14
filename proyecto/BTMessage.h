#ifndef BTMESSAGE_H_
#define BTMESSAGE_H_

#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>

#include "Serializable.h"
#include "Socket.h"

class BTMessage : public Serializable
{
public:
    size_t MESSAGE_SIZE = 8*sizeof(char)  + 2*sizeof(uint8_t) + 2*sizeof(float)
    + 12*sizeof(char);

    enum MessageType
    {
        LOGIN = 0,
        INPUT = 1,
        LOGOUT = 2,
        ACCEPT = 3
    };

    BTMessage(){};

    BTMessage(const std::string &n) : nick(n){};

    virtual void to_bin();
    virtual int from_bin(char *data);

    uint8_t type;
    std::string nick; // max. 8

    // TODO: ESTA INFO DE INPUT TIENE QUE IR EN OTRA CLASE!!!
    uint8_t index;
    std::string message; // max. 12
    float mouseX;
    float mouseY;
};

#endif /* BTMESSAGE_H_ */