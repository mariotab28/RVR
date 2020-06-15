#include "Gear.h"

#include <SFML/Graphics.hpp>
#include "GameWorld.h"
#include "Player.h"

Gear::Gear(GameWorld *world) : GameObject(world, 1)
{
    setId("Gear");
}

Gear::~Gear()
{
}

void Gear::update(sf::RenderWindow &window, sf::Time& elapsedTime)
{
    GameObject::update(window, elapsedTime);
    
    // check collision with a player--------------
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
            printf("gear recogido!\n");
            world->createGear(window);
        }
    }
}

void Gear::to_bin()
{
    GameObject::to_bin();


}

int Gear::from_bin(char *data)
{
    try
    {

        //printf("gear from_bin\n");
        GameObject::from_bin(data);

        return 0;
    }
    catch(const std::exception& e)
    {
        return -1;
    }
}
