#include "GameObject.h"

#include <SFML/Graphics.hpp>

#include <math.h>

GameObject::GameObject(GameWorld *world, int type)
    : Serializable(), world(world), type(type)
{
    switch (type)
    {
    case 0: // TEXT GAMEOBJECT
    {
        entity = new sf::Text();
        break;
    }
    case 1: // SPRITE GAMEOBJECT
    {
        entity = new sf::Sprite();
        break;
    }
    default:
    {
        printf("ERROR: invalid type of GO\n");
        break;
    }
    }

    angle = 0;
}

GameObject::~GameObject()
{
    if (entity != nullptr)
        delete entity;
}

void GameObject::setText(const std::string &text)
{
    if (type == 0)
        static_cast<sf::Text *>(entity)->setString(text);
    else
        printf("ERROR: trying to setText to a non-text GO\n");
}

void GameObject::setTexture(const sf::Texture &texture)
{
    if (type == 1)
        static_cast<sf::Sprite *>(entity)->setTexture(texture);
    else
        printf("ERROR: trying to setTexture to a non-sprite GO\n");
}

const sf::Texture *GameObject::getTexture()
{
    if (type == 1)
        return static_cast<sf::Sprite *>(entity)->getTexture();
    else
        printf("ERROR: trying to getTexture from a non-sprite GO\n");
    return nullptr;
}

void GameObject::setFont(const sf::Font &font)
{
    if (type == 0)
        static_cast<sf::Text *>(entity)->setFont(font);
    else
        printf("ERROR: trying to setFont to a non-text GO\n");
}

void GameObject::setPosition(float x, float y)
{
    this->x = x;
    this->y = y;
    entity->setPosition(x, y);
}

void GameObject::setRotation(float angle)
{
    this->angle = angle;
    entity->setRotation(angle);
}

float GameObject::getRotation()
{
    return angle;
}

void GameObject::setScale(float factorX, float factorY)
{
    entity->setScale(factorX, factorY);
}

void GameObject::setOrigin(float originX, float originY)
{
    entity->setOrigin(originX, originY);
}

void GameObject::setDir(float dirX, float dirY)
{
    this->dirX = dirX;
    this->dirY = dirY;
}
/*const std::pair<float, float> GameObject::getDir()
{
    return std::pair<float, float>(dirX, dirY);
}*/

void GameObject::setSpeed(float speed)
{
    this->speed = speed;
}

void GameObject::render(sf::RenderWindow &window)
{
    if (type == 0)
        window.draw(*static_cast<sf::Text *>(entity));
    else
        window.draw(*static_cast<sf::Sprite *>(entity));
}

void GameObject::update()
{
    // pos + velocity
    x += dirX * speed;
    y += dirY * speed;
    setPosition(x, y);

    dirX = cosf(angle * PI / 180);
    dirY = sinf(angle * PI / 180);
    setRotation(angle);
}

void GameObject::handleInput(sf::Event &event)
{
}

void GameObject::to_bin()
{
}

int GameObject::from_bin(char *data)
{
    return 0;
}