#include "Gear.h"

#include <SFML/Graphics.hpp>
#include "GameWorld.h"
#include "Player.h"

Gear::Gear(GameWorld *world) : GameObject(world, 1)
{
    setId("Gear");
}

void Gear::update(sf::RenderWindow &window, float deltaTime)
{
    GameObject::update(window, deltaTime);

    checkPlayerCollision(window);
}

void Gear::checkPlayerCollision(sf::RenderWindow &window)
{
    std::vector<GameObject *> players = world->getPlayers();

    for (auto it = players.begin(); it != players.end(); ++it)
    {
        sf::Sprite *mySprite = getSprite();
        sf::Sprite *playerSprite = (*it)->getSprite();
        if (mySprite != nullptr && playerSprite != nullptr &&
            (*it)->isActive() &&
            mySprite->getGlobalBounds().intersects(playerSprite->getGlobalBounds())
            && (*it)->getId().compare(0,6,"Player") == 0)
        {
            static_cast<Player*>(*it)->setPoints(static_cast<Player*>(*it)->getPoints() + 1);
            world->updateScoreTexts();
            setActive(false);
            world->createGear(window);
        }
    }
}
