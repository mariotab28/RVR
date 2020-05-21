#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include "Serializable.h"
#include <string>

namespace sf
{
    class Transformable;
    class RenderWindow;
    class Texture;
    class Font;
}; // namespace sf

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

    // info a serializar
    int type;
    char *id;

    int x, y;
    float angle;

public:
    GameObject(int type);
    virtual ~GameObject();

    virtual void render(sf::RenderWindow &window);
    virtual void update();
    virtual void handleInput(sf::RenderWindow &window);

    void setText(const std::string &text);
    void setTexture(const sf::Texture &texture);
    void setFont(const sf::Font &font);

    void setPosition(int x, int y);
    void setScale(float factorX, float factorY);
    void setOrigin(float originX, float originY);

    void to_bin();
    int from_bin(char *data);
};

#endif /* GAMEOBJECT_H_ */
