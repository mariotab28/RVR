#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include "Serializable.h"
#include <string>

namespace sf
{
    class Transformable;
    class RenderWindow;
    class Event;
    class Texture;
    class Font;
}; // namespace sf
class GameWorld;

class GameObject : public Serializable
{
private:
    sf::Transformable *entity;

    //sf::Texture *texture;
    //sf::Font *font;

    /*enum TYPE
    {
        TEXT,
        SPRITE
    };*/

protected:
    const float PI = 3.14159265359;

    // info a serializar
    int type;
    char *id;

    float x, y;
    float angle;

    // serializar?
    float speed;
    float dirX, dirY;

    // NO serializar
    GameWorld *world;

public:
    GameObject(GameWorld *world, int type);
    virtual ~GameObject();

    virtual void render(sf::RenderWindow &window);
    virtual void update();
    virtual void handleInput(sf::Event &event);

    void setText(const std::string &text);
    void setTexture(const sf::Texture &texture);
    const sf::Texture *getTexture();
    void setFont(const sf::Font &font);

    void setPosition(float x, float y);
    void setRotation(float angle);
    float getRotation();
    void setScale(float factorX, float factorY);
    void setOrigin(float originX, float originY);
    void setDir(float dirX, float dirY);
    //const std::pair<float, float> getDir();
    void setSpeed(float speed);

    void to_bin();
    int from_bin(char *data);
};

#endif /* GAMEOBJECT_H_ */
