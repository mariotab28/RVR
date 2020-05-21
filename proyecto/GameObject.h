#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include "Serializable.h"

namespace sf
{
    class Drawable;
    class RenderWindow;
};

class GameObject : public Serializable
{
private:
    sf::Drawable *entity;

    /*enum TYPE
    {
        TEXT,
        SPRITE
    };*/

    // info a serializar
    int type;
    char *id;

    int x, y;
    float angle;

public:
    GameObject(int type, int x, int y);
    virtual ~GameObject();

    void render(sf::RenderWindow &window);
    virtual void update();

    void to_bin();
    int from_bin(char *data);
};

#endif /* GAMEOBJECT_H_ */
