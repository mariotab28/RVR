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
    virtual ~Bullet();

    virtual void update(sf::RenderWindow &window, sf::Time &elapsedTime);

    /*virtual void render(sf::RenderWindow &window);
    virtual void update();
    virtual void handleInput(sf::Event &event);*/

    bool checkBounces();
    void resetBounces();

    virtual void to_bin();
    virtual int from_bin(char *data);
};

#endif /* BULLET_H_ */
