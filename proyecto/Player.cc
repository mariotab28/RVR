#include "GameObject.h"

#include <SFML/Graphics.hpp>

GameObject::GameObject(int type)
    : Serializable(), type(type)
{
    switch (type)
    {
    case 0: // TEXT GAMEOBJECT
    {
        entity = new sf::Text();
        //sf::Text *text = static_cast<sf::Text *>(entity);

        /*text->setFont(*font);
        text->setCharacterSize(20);
        text->setString("RANKING");*/

        printf("text created\n");

        break;
    }
    case 1: // SPRITE GAMEOBJECT
    {
        entity = new sf::Sprite();

        /*sf::Sprite *sprite = static_cast<sf::Sprite *>(entity);
        sprite->setTexture(*texture);
        sprite->setPosition(x, y);*/

        break;
    }
    default:
    {
        printf("ERROR: invalid type of GO\n");
        break;
    }
    }
}

GameObject::~GameObject()
{
    if (entity != nullptr)
        delete entity;
    /*if (texture != nullptr)
        delete texture;
    if (font != nullptr)
        delete font;*/
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

void GameObject::setFont(const sf::Font &font)
{
    if (type == 0)
        static_cast<sf::Text *>(entity)->setFont(font);
    else
        printf("ERROR: trying to setFont to a non-text GO\n");
}

void GameObject::setPosition(int x, int y)
{
    this->x = x;
    this->y = y;
    entity->setPosition(x, y);
}

void GameObject::setScale(float factorX, float factorY)
{
    entity->setScale(factorX, factorY);
}

void GameObject::setOrigin(float originX, float originY)
{
    entity->setOrigin(originX, originY);
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
}

void GameObject::handleInput(sf::RenderWindow &window)
{
}

void GameObject::to_bin()
{
}

int GameObject::from_bin(char *data)
{
    return 0;
}