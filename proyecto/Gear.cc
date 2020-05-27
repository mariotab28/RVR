#include "Gear.h"

#include <SFML/Graphics.hpp>
#include "GameWorld.h"

Gear::Gear(GameWorld *world) : GameObject(world, 1)
{
}

Gear::~Gear()
{
}

void Gear::update(sf::RenderWindow &window)
{
    GameObject::update(window);

    //check collision with a player
    // if(intersects())
    // world->destroy(this);
}

void Gear::to_bin()
{
}

int Gear::from_bin(char *data)
{
    return 0;
}
