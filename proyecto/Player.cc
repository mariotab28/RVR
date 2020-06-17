#include "Player.h"

#include <SFML/Graphics.hpp>
#include <math.h>
#include <string.h>

#include "GameWorld.h"
#include "Bullet.h"

Player::Player(GameWorld *world) : GameObject(world, 1)
{
    gun = new GameObject(world, 1);
    gun->setScale(0.4, 0.4);

    incAngle = 0;
    points = 0;
    nick = "";
    incSpeed = 80;
    incAngleSpeed = 110;
    reverse = false;
    cooldown = 0;
    time = 0;
}

Player::~Player()
{
    if (gun != nullptr)
        delete gun;
}

void Player::render(sf::RenderWindow &window)
{
    GameObject::render(window);
    gun->render(window);
}

void Player::update(sf::RenderWindow &window, float deltaTime)
{
    // update shoot cooldown
    if (time > 0)
        time -= deltaTime;

    // update rotation
    int factor = reverse ? -1 : 1;
    angle += incAngle * deltaTime;
    dirX = factor * cosf(angle * PI / 180);
    dirY = factor * sinf(angle * PI / 180);
    setRotation(angle);

    // collisions
    checkBulletCollision(window);
    checkWallCollision();

    // update pos
    x += dirX * speed * deltaTime;
    y += dirY * speed * deltaTime;
    setPosition(x, y);

    // update gun position and rotation
    gun->setPosition(x, y);
    gun->setRotation(gun->getRotation() + gunIncAngle * deltaTime);
}

void Player::processInput(BTMessage message)
{
    if (message.message.compare(0, 5, "Press") == 0)
    {
        // MOVEMENT INPUT
        if (message.message.compare(5, 1, "D") == 0)
            incAngle = incAngleSpeed;
        if (message.message.compare(5, 1, "A") == 0)
            incAngle = -incAngleSpeed;
        if (message.message.compare(5, 1, "W") == 0)
        {
            speed = incSpeed;
            if (reverse)
                reverse = false;
        }
        if (message.message.compare(5, 1, "S") == 0)
        {
            speed = incSpeed;
            if (!reverse)
                reverse = true;
        }
        // GUN ROTATION INPUT
        if (message.message.compare(5, 1, "Q") == 0)
            gunIncAngle = incAngleSpeed;
        if (message.message.compare(5, 1, "E") == 0)
            gunIncAngle = -incAngleSpeed;
    }

    if (message.message.compare(0, 5, "Relea") == 0)
    {
        if (message.message.compare(5, 1, "D") == 0)
            incAngle = 0;
        if (message.message.compare(5, 1, "A") == 0)
            incAngle = 0;
        if (message.message.compare(5, 1, "W") == 0)
            speed = 0;
        if (message.message.compare(5, 1, "S") == 0)
            speed = 0;
        if (message.message.compare(5, 1, "Q") == 0)
            gunIncAngle = 0;
        if (message.message.compare(5, 1, "E") == 0)
            gunIncAngle = 0;
    }

    // SHOOT INPUT
    if (message.message.compare(0, 5, "Mouse") == 0)
    {
        if (time <= 0)
        {
            shoot();
            time = cooldown;
        }
    }
}

void Player::checkBulletCollision(sf::RenderWindow& window)
{
    std::vector<GameObject *> bullets = world->getBullets();

    for (auto it = bullets.begin(); it != bullets.end(); ++it)
    {
        sf::Sprite *mySprite = getSprite();
        sf::Sprite *bulletSprite = (*it)->getSprite();
        if (mySprite != nullptr && bulletSprite != nullptr &&
            (*it)->isActive() &&
            mySprite->getGlobalBounds().intersects(bulletSprite->getGlobalBounds()) && (*it)->getId().compare(0, 6, "Bullet") == 0 && (*it)->getId().compare(6, 7, getId()) != 0)
        {
            setActive(false);
            (*it)->setActive(false);
            points = 0;
            world->createPlayer(nick, window);
        }
    }
}

void Player::checkWallCollision()
{
    std::vector<GameObject *> walls = world->getWalls();

    for (auto it = walls.begin(); it != walls.end(); ++it)
    {
        sf::Sprite *mySprite = getSprite();
        sf::Sprite *wallSprite = (*it)->getSprite();
        if (mySprite != nullptr && wallSprite != nullptr &&
            (*it)->isActive() &&
            mySprite->getGlobalBounds().intersects(wallSprite->getGlobalBounds()) && (*it)->getId().compare(0, 4, "Wall") == 0)
        {
            // calculate difference vector
            std::pair<float, float> dir = {getX() - (*it)->getX(), getY() - (*it)->getY()};

            // calculate widths and heights
            float playerWidth = mySprite->getTexture()->getSize().x * mySprite->getScale().x;
            float playerHeight = mySprite->getTexture()->getSize().y * mySprite->getScale().y;
            float wallWidth = wallSprite->getTexture()->getSize().x * wallSprite->getScale().x;
            float wallHeight = wallSprite->getTexture()->getSize().y * wallSprite->getScale().y;

            float width = (playerWidth + wallWidth) / 2;
            float height = (playerHeight + wallHeight) / 2;
            float crossWidth = width * dir.second;
            float crossHeight = height * dir.first;

            // calculate which side of the wall the player is colliding
            std::string collision = "none";
            if (abs(dir.first) <= width && abs(dir.second) <= height)
            {
                if (crossWidth > crossHeight)
                    collision = (crossWidth > (-crossHeight)) ? "bottom" : "left";
                else
                    collision = (crossWidth > -(crossHeight)) ? "right" : "top";
            }

            // stop the player if its colliding
            if ((collision == "top" && dirY > 0) || (collision == "bottom" && dirY < 0))
                speed = 0;
            else if ((collision == "right" && dirX < 0) || (collision == "left" && dirX > 0))
                speed = 0;

            break;
        }
    }
}

void Player::shoot()
{
    if (world != nullptr)
    {
        float gunAngle = gun->getRotation();
        world->createBullet(x + cosf(gunAngle * PI / 180) * 70,
                            y + sinf(gunAngle * PI / 180) * 70,
                            gunAngle, getId());
    }
}

void Player::setGunTexture(sf::Texture &texture)
{
    gun->setTexture(texture);
    gun->setOrigin(gun->getTexture()->getSize().x * 0.3,
                   gun->getTexture()->getSize().y * 0.5);
}

void Player::setPoints(int points)
{
    this->points = points;
}

int Player::getPoints()
{
    return points;
}

void Player::setNick(const std::string &nick)
{
    this->nick = nick;
}

std::string Player::getNick()
{
    return nick;
}

void Player::to_bin()
{
    GameObject::to_bin();
    gun->to_bin();

    MESSAGE_SIZE += gun->size() + sizeof(uint8_t);

    // save data
    int32_t auxSize = size();
    char *aux = (char *)malloc(auxSize);
    memcpy(aux, static_cast<void *>(data()), auxSize);

    // add new data
    alloc_data(MESSAGE_SIZE);
    memset(_data, 0, MESSAGE_SIZE);

    memcpy(_data, static_cast<void *>(aux), auxSize);
    _data += auxSize;

    // serialize points
    memcpy(_data, static_cast<void *>(&points), sizeof(uint8_t));
    _data += sizeof(uint8_t);

    // serialize gun
    memcpy(_data, static_cast<void *>(gun->data()), gun->size());
    _data += gun->size();

    _data -= MESSAGE_SIZE;
}

int Player::from_bin(char *data)
{
    try
    {
        GameObject::from_bin(data);
        data += _size;

        // deserialize points
        memcpy(static_cast<void *>(&points), data, sizeof(uint8_t));
        data += sizeof(uint8_t);
        _size += sizeof(uint8_t);

        // deserialize gun
        gun->from_bin(data);
        data += gun->size();
        _size += gun->size();

        return 0;
    }
    catch (const std::exception &e)
    {
        return -1;
    }
}