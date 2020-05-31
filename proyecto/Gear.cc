#include "Gear.h"

#include <SFML/Graphics.hpp>
#include "GameWorld.h"
#include "Player.h"

Gear::Gear(GameWorld *world) : GameObject(world, 1)
{
}

Gear::~Gear()
{
}

void Gear::update(sf::RenderWindow &window)
{
    GameObject::update(window);

    // check collision with a player--------------
    std::vector<GameObject *> gameObjects = world->getGameObjects();

    for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
    {
        sf::Sprite *mySprite = getSprite();
        sf::Sprite *playerSprite = (*it)->getSprite();
        if (mySprite != nullptr && playerSprite != nullptr &&
            mySprite->getGlobalBounds().intersects(playerSprite->getGlobalBounds())
            && (*it)->getId().compare(0,6,"Player") == 0)
        {
            static_cast<Player*>(*it)->setPoints(static_cast<Player*>(*it)->getPoints() + 1);
            world->updateScores();
            printf("gear recogido!\n");
            world->destroy(this);
        }
    }
}

void Gear::to_bin()
{
}

int Gear::from_bin(char *data)
{
    return 0;
}
