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
    mouseX = 0;
    mouseY = 0;
    points = 0;
    nick = "";
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

void Player::update(sf::RenderWindow &window, sf::Time &elapsedTime)
{
    angle += incAngle * elapsedTime.asSeconds();

    // update cooldown
    if (time > 0)
        time -= elapsedTime.asSeconds();

    // GUN UPDATE-------------
    gun->setPosition(x, y);
    gun->setRotation(gun->getRotation() + gunIncAngle * elapsedTime.asSeconds());

    int factor = reverse ? -1 : 1;

    dirX = factor * cosf(angle * PI / 180);
    dirY = factor * sinf(angle * PI / 180);
    setRotation(angle);

    // check collision with a bullet--------------
    std::vector<GameObject *> bullets = world->getBullets();

    for (auto it = bullets.begin(); it != bullets.end(); ++it)
    {
        sf::Sprite *mySprite = getSprite();
        sf::Sprite *bulletSprite = (*it)->getSprite();
        if (mySprite != nullptr && bulletSprite != nullptr &&
            (*it)->isActive() &&
            mySprite->getGlobalBounds().intersects(bulletSprite->getGlobalBounds()) && (*it)->getId().compare(0, 6, "Bullet") == 0 && (*it)->getId().compare(6, 7, getId()) != 0)
        {
            printf("it->getid: %s getid: %s\n", (*it)->getId().c_str(), getId().c_str());

            printf("player destruido!\n");
            setActive(false);
            (*it)->setActive(false);
            points = 0;
            world->createPlayer(nick, window);
        }
    }

    // check collision with a wall--------------------
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
                speed = 0;
            else if ((collision == "right" && dirX < 0) || (collision == "left" && dirX > 0))
                speed = 0;
            else
                break;

            // normalize
            /*float magnitude = sqrt(dirX * dirX + dirY * dirY);
            dirX /= magnitude;
            dirY /= magnitude;

            angle = atan2(dirY, dirX) * 180 / PI;*/

            break;
        }
    }

    // BASE UPDATE-------------
    //GameObject::update(window, elapsedTime);

    x += dirX * speed * elapsedTime.asSeconds();
    y += dirY * speed * elapsedTime.asSeconds();
    setPosition(x, y);
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

    if (message.message.compare(0, 5, "Mouse") == 0)
    {
        if (time <= 0)
        {
            shoot();
            time = cooldown;
        }
    }

    // TODO: ESTO DA ERROR CUANDO SE CIERRA LA VENTANA!!
    //mouseX = message.mouseX;
    //mouseY = message.mouseY;
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

    int32_t auxSize = size();
    char *aux = (char *)malloc(auxSize);
    memcpy(aux, static_cast<void *>(data()), auxSize);

    alloc_data(MESSAGE_SIZE);
    memset(_data, 0, MESSAGE_SIZE);

    memcpy(_data, static_cast<void *>(aux), auxSize);
    _data += auxSize;

    // serializar points
    memcpy(_data, static_cast<void *>(&points), sizeof(uint8_t));
    _data += sizeof(uint8_t);

    memcpy(_data, static_cast<void *>(gun->data()), gun->size());
    _data += gun->size();

    // colocamos el puntero al inicio del fichero
    _data -= MESSAGE_SIZE;
}

int Player::from_bin(char *data)
{
    try
    {
        GameObject::from_bin(data);
        data += _size;

        // deserializamos points
        memcpy(static_cast<void *>(&points), data, sizeof(uint8_t));
        data += sizeof(uint8_t);
        _size += sizeof(uint8_t);

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