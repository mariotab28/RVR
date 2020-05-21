#include "GameObject.h"

#include <SFML/Graphics.hpp>

GameObject::GameObject(int type, int x, int y)
    : Serializable(), type(type), x(x), y(y)
{
    switch (type)
    {
    case 0:
    {
        /* code */
        break;
    }
    case 1:
    {
        sf::Texture texture;
        if (!texture.loadFromFile("Assets/tankBase.png"))
            printf("error loading texture\n");

        printf("create sprite\n");

        entity = new sf::Sprite();

        sf::Sprite *sprite = static_cast<sf::Sprite *>(entity);
        sprite->setTexture(texture);
        sprite->setPosition(x, y);

        break;
    }
    default:
    {
        break;
    }
    }
}

GameObject::~GameObject()
{
    delete entity;
}

void GameObject::render(sf::RenderWindow &window)
{
    window.draw(*entity);
}

void GameObject::update()
{
    // pos + velocity
}

void GameObject::to_bin()
{
}

int GameObject::from_bin(char *data)
{
    return 0;
}