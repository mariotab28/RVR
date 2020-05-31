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

    virtual void update(sf::RenderWindow &window);

    /*virtual void render(sf::RenderWindow &window);
    virtual void update();
    virtual void handleInput(sf::Event &event);*/

    void to_bin();
    int from_bin(char *data);
};

#endif /* GEAR_H_ */