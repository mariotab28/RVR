#include "Serializable.h"

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

class Jugador: public Serializable
{
public:
    Jugador(const char * _n, int16_t _x, int16_t _y):pos_x(_x),pos_y(_y)
    {
        strncpy(name, _n, MAX_NAME);
    };

    virtual ~Jugador(){};

    void to_bin()
    {
        // calculamos size total de data a serializar
        int32_t totalSize = MAX_NAME*sizeof(char) + sizeof(int16_t) + sizeof(int16_t);

        // reservamos memoria
        alloc_data(totalSize);

        // escribimos nombre
        memcpy(_data, static_cast<void*>(&name), MAX_NAME*sizeof(char));
        _data += MAX_NAME*sizeof(char);

        // escribimos pos x
        memcpy(_data, static_cast<void*>(&pos_x), sizeof(int16_t));
        _data += sizeof(int16_t);

        // escribimos pos y
        memcpy(_data, static_cast<void*>(&pos_y), sizeof(int16_t));
        _data += sizeof(int16_t);

        // colocamos el puntero al inicio del fichero
        _data -= totalSize;
    }

    int from_bin(char * data)
    {
        //
        return 0;
    }

private:
  static const size_t MAX_NAME = 20;

  int16_t pos_x;
  int16_t pos_y;
  
  char name[MAX_NAME];

};

int main(int argc, char **argv)
{
    // instanciamos objeto
    Jugador one("player one", 12, 345);

    // serializamos objeto
    one.to_bin();

    // creamos fichero
    int f;
    f = open("data", O_CREAT | O_RDWR);

    if(f==-1)
    {
        printf("ERROR al crear fichero data\n");
        return -1;
    }

    // escribimos el objeto serializado en el fichero
    int bytes = write(f, one.data(), one.size());

    if(bytes == -1)
    {
        printf("ERROR al escribir en data\n");
    }

    // cerramos el fichero
    close(f);

    return 0;
}
