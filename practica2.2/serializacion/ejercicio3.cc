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

    char* getName()
    {
        return name;
    }

    int16_t getPosX()
    {
        return pos_x;
    }

    int16_t getPosY()
    {
        return pos_y;
    }

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
        try
        {
            // leemos nombre
            memcpy(static_cast<void*>(&name), data, MAX_NAME*sizeof(char));
            data += MAX_NAME*sizeof(char);

            // leemos pos x
            memcpy(static_cast<void*>(&pos_x), data, sizeof(int16_t));
            data += sizeof(int16_t);

            // leemos pos y
            memcpy(static_cast<void*>(&pos_y), data,  sizeof(int16_t));
            data += sizeof(int16_t);

            return 0;
        }
        catch(const std::exception& e)
        {
            return -1;
        }
    }

private:
  static const size_t MAX_NAME = 20;

  int16_t pos_x;
  int16_t pos_y;
  
  char name[MAX_NAME];

};

int main(int argc, char **argv)
{
    // SERIALIZAR---
    Jugador one("player one", 12, 345);
    one.to_bin();

    // creamos fichero
    int f1;
    f1 = open("data", O_CREAT | O_RDWR);

    if(f1==-1)
    {
        printf("ERROR al crear fichero data\n");
        return -1;
    }

    // escribimos el objeto serializado en el fichero
    if(write(f1, one.data(), one.size()) == -1)
    {
        printf("ERROR al escribir en data\n");
        return -1;
    }
    
    // cerramos el fichero
    close(f1);

    // DESERIALIZAR---
    Jugador second("", 0, 0);

    // abrimos fichero
    int f2;
    f2 = open("data", O_RDONLY);

    // calculamos length del fichero a leer
    int length = lseek (f2, 0, SEEK_END);
    lseek (f2, 0, SEEK_SET);

    // leemos el objeto serializado en el fichero
    char * buff = (char *)malloc(length);
    if(read(f2, static_cast<void*>(buff), length) == -1)
    {
        printf("ERROR al leer de data\n");
        return -1;
    }

    if (second.from_bin(buff) == -1)
    {
        printf("ERROR al deserializar data\n");
        return -1;
    }

    // mostramos por pantalla el objeto
    printf("PLAYER SECOND\n");
    printf("NAME: %s\n", second.getName());
    printf("POSX: %d\n", second.getPosX());
    printf("POSY: %d\n", second.getPosY());

    // cerramos el fichero
    close(f2);

    return 0;
}
