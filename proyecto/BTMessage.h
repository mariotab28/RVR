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
    size_t MESSAGE_SIZE = sizeof(char) * 8 + sizeof(uint8_t);

    enum MessageType
    {
        LOGIN = 0,
        OBJECT = 1,
        LOGOUT = 2
    };

    BTMessage(){};

    BTMessage(const std::string &n) : nick(n){};

    virtual void to_bin();

    virtual int from_bin(char *data);

    uint8_t type;
    std::string nick; // max. 8
};

#endif /* BTMESSAGE_H_ */