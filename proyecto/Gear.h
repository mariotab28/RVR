#ifndef GEAR_H_
#define GEAR_H_

#include "GameObject.h"

namespace sf
{
    class RenderWindow;
    class Event;
}; // namespace sf

class Gear : public GameObject
{
public:
    Gear(GameWorld *world);

    virtual void update(sf::RenderWindow &window, sf::Time& elapsedTime);

    void checkPlayerCollision(sf::RenderWindow &window);
};

#endif /* GEAR_H_ */
