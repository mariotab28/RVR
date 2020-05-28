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

    int points;

    // info a serializar

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

    void to_bin();
    int from_bin(char *data);
};

#endif /* PLAYER_H_ */
