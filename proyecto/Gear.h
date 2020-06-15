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
private:
    // info a serializar

public:
    Gear(GameWorld *world);
    virtual ~Gear();

    virtual void update(sf::RenderWindow &window, sf::Time& elapsedTime);

    /*virtual void render(sf::RenderWindow &window);
    virtual void update();
    virtual void handleInput(sf::Event &event);*/

    virtual void to_bin();
    virtual int from_bin(char *data);
};

#endif /* GEAR_H_ */
