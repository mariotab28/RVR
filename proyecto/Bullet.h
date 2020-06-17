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
    // NO serializar
    int nBounces;
    int maxBounces;

public:
    Bullet(GameWorld *world);

    virtual void update(sf::RenderWindow &window, sf::Time &elapsedTime);

    bool checkBounces();
    void resetBounces();

    virtual void to_bin();
    virtual int from_bin(char *data);
};

#endif /* BULLET_H_ */
