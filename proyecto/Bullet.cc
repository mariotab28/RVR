#include "Bullet.h"

#include <SFML/Graphics.hpp>
#include <math.h>
#include "GameWorld.h"

Bullet::Bullet(GameWorld *world) : GameObject(world, 1)
{
    maxBounces = 3;
    resetBounces();
}

void Bullet::update(sf::RenderWindow &window, sf::Time &elapsedTime)
{
    GameObject::update(window, elapsedTime);

    // check collision with wall and bounce
    std::vector<GameObject *> walls = world->getWalls();

    for (auto it = walls.begin(); it != walls.end(); ++it)
    {
        sf::Sprite *mySprite = getSprite();
        sf::Sprite *wallSprite = (*it)->getSprite();
        if (mySprite != nullptr && wallSprite != nullptr &&
            (*it)->isActive() &&
            mySprite->getGlobalBounds().intersects(wallSprite->getGlobalBounds()) && (*it)->getId().compare(0, 4, "Wall") == 0)
        {
            // calculate distance
            std::pair<float, float> dir = {getX() - (*it)->getX(), getY() - (*it)->getY()};

            float bulletWidth = mySprite->getTexture()->getSize().x * mySprite->getScale().x;
            float bulletHeight = mySprite->getTexture()->getSize().y * mySprite->getScale().y;
            float wallWidth = wallSprite->getTexture()->getSize().x * wallSprite->getScale().x;
            float wallHeight = wallSprite->getTexture()->getSize().y * wallSprite->getScale().y;

            float width = (bulletWidth + wallWidth) / 2;
            float height = (bulletHeight + wallHeight) / 2;
            float crossWidth = width * dir.second;
            float crossHeight = height * dir.first;
            std::string collision = "none";

            if (abs(dir.first) <= width && abs(dir.second) <= height)
            {
                if (crossWidth > crossHeight)
                    collision = (crossWidth > (-crossHeight)) ? "bottom" : "left";
                else
                    collision = (crossWidth > -(crossHeight)) ? "right" : "top";
            }

            if ((collision == "top" && dirY > 0) || (collision == "bottom" && dirY < 0))
            {
                dirY *= -1;
                if (checkBounces())
                    return;
            }
            else if ((collision == "right" && dirX < 0) || (collision == "left" && dirX > 0))
            {
                dirX *= -1;
                if (checkBounces())
                    return;
            }
            else
                break;

            // normalize
            float magnitude = sqrt(dirX * dirX + dirY * dirY);
            dirX /= magnitude;
            dirY /= magnitude;

            angle = atan2(dirY, dirX) * 180 / PI;

            break;
        }
    }

    // if its out of bounds, its get destroyed
    if (x < 0 || x > window.getSize().x || y < 0 || y > window.getSize().y)
        setActive(false);
}

bool Bullet::checkBounces()
{
    nBounces--;
    if (nBounces <= 0)
    {
        setActive(false);
        return true;
    }

    return false;
}

void Bullet::resetBounces()
{
    nBounces = maxBounces;
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
    catch (const std::exception &e)
    {
        return -1;
    }
}
