#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include "BTMessage.h"
#include <string>

namespace sf
{
    class Transformable;
    class RenderWindow;
    class Event;
    class Texture;
    class Font;
    class Sprite;
    class Time;
}; // namespace sf
class GameWorld;

class GameObject : public BTMessage
{
private:
    sf::Transformable *entity;

protected:
    const float PI = 3.14159265359;

    // info a serializar
    uint8_t goType;
    std::string id; // max 20

    float x, y;
    float angle;

    std::string text; // max 16

    bool active;
    float scaleX;
    float scaleY;

    // NO serializar
    float speed;
    float dirX, dirY;
    GameWorld *world;

public:
    GameObject(GameWorld *world, int goType);
    virtual ~GameObject();

    virtual void render(sf::RenderWindow &window);
    virtual void update(sf::RenderWindow &window, float deltaTime);
    virtual void handleInput(sf::Event &event, sf::RenderWindow &window);

    void setText(const std::string &text);
    void setTexture(const sf::Texture &texture);
    const sf::Texture *getTexture();
    void setFont(const sf::Font &font);
    void setId(const std::string& id);
    void setPosition(float x, float y);
    void setRotation(float angle);
    void setScale(float factorX, float factorY);
    void setOrigin(float originX, float originY);
    void setDir(float dirX, float dirY);
    void setSpeed(float speed);
    void setActive(bool active);

    float getX();
    float getY();
    std::string getId();
    float getRotation();
    sf::Sprite* getSprite();
    bool isActive();

    virtual void to_bin();
    virtual int from_bin(char *data);
};

#endif /* GAMEOBJECT_H_ */
