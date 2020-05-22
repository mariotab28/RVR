#ifndef BULLET_H_
#define BULLET_H_

#include "GameObject.h"

namespace sf
{
    class RenderWindow;
    class Event;
}; // namespace sf

class Bullet : public GameObject
{
private:

    // info a serializar

public:
    Bullet(GameWorld* world);
    virtual ~Bullet();

    /*virtual void render(sf::RenderWindow &window);
    virtual void update();
    virtual void handleInput(sf::Event &event);*/

    void to_bin();
    int from_bin(char *data);
};

#endif /* BULLET_H_ */
