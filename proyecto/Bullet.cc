#include "Bullet.h"

#include <SFML/Graphics.hpp>
#include "GameWorld.h"

Bullet::Bullet(GameWorld *world) : GameObject(world, 1)
{
}

Bullet::~Bullet()
{
}

void Bullet::update(sf::RenderWindow &window)
{
    GameObject::update(window);

    // if its out of bounds, its get destroyed
    if (x < 0 || x > window.getSize().x || y < 0 || y > window.getSize().y)
        world->destroy(this);
}

void Bullet::to_bin()
{
}

int Bullet::from_bin(char *data)
{
    return 0;
}
