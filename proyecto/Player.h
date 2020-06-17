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
    float gunIncAngle;

    // info a serializar
    uint8_t points;

    // NO serializar
    float incSpeed;
    float incAngleSpeed;
    std::string nick;
    bool reverse;
    float cooldown;
    float time;

public:
    Player(GameWorld *world);
    virtual ~Player();

    virtual void render(sf::RenderWindow &window);
    virtual void update(sf::RenderWindow &window, sf::Time& elapsedTime);

    void processInput(BTMessage message);

    void checkBulletCollision(sf::RenderWindow& window);
    void checkWallCollision();

    void setGunTexture(sf::Texture &texture);
    void shoot();

    void setPoints(int points);
    int getPoints();
    void setNick(const std::string& nick);
    std::string getNick();

    virtual void to_bin();
    virtual int from_bin(char *data);
};

#endif /* PLAYER_H_ */
