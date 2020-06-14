#ifndef PLAYER_H_
#define PLAYER_H_

#include "GameObject.h"

namespace sf
{
    class RenderWindow;
    class Event;
}; // namespace sf

class Player : public GameObject
{
private:
    GameObject *gun;

    float incAngle;
    float mouseX, mouseY;

    // info a serializar
    uint8_t points;

public:
    Player(GameWorld* world);
    virtual ~Player();

    virtual void render(sf::RenderWindow &window);
    virtual void update(sf::RenderWindow &window);
    virtual void handleInput(sf::Event &event);

    void setGunTexture(sf::Texture &texture);
    void shoot();

    void setPoints(int points);
    int getPoints();

    virtual void to_bin();
    virtual int from_bin(char *data);
};

#endif /* PLAYER_H_ */
