#include "Bullet.h"

#include <SFML/Graphics.hpp>
#include "GameWorld.h"

Bullet::Bullet(GameWorld *world) : GameObject(world, 1)
{
}

Bullet::~Bullet()
{
}

void Bullet::update(sf::RenderWindow &window, sf::Time& elapsedTime)
{
    GameObject::update(window, elapsedTime);

    // if its out of bounds, its get destroyed
    if (x < 0 || x > window.getSize().x || y < 0 || y > window.getSize().y)
        setActive(false);//world->destroy(this);
}

void Bullet::to_bin()
{
    GameObject::to_bin();

}

int Bullet::from_bin(char *data)
{
    try
    {
        GameObject::from_bin(data);

        return 0;
    }
    catch(const std::exception& e)
    {
        return -1;
    }
}

