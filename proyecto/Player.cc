#include "Player.h"

#include <SFML/Graphics.hpp>
#include <math.h>
#include <string.h>

#include "GameWorld.h"
#include "Bullet.h"

Player::Player(GameWorld *world) : GameObject(world, 1)
{
    gun = new GameObject(world, 1);
    gun->setScale(0.5, 0.5);

    incAngle = 0;
    mouseX = 0;
    mouseY = 0;
    points = 0;
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

void Player::update(sf::RenderWindow &window)
{
    // BASE UPDATE-------------
    GameObject::update(window);

    angle += incAngle;

    // GUN UPDATE-------------
    gun->setPosition(x, y);
    gun->setRotation(atan2(mouseY - y, mouseX - x) * 180 / PI);

    // check collision with a bullet--------------
    /*std::vector<GameObject *> gameObjects = world->getGameObjects();

    for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
    {
        sf::Sprite *mySprite = getSprite();
        sf::Sprite *bulletSprite = (*it)->getSprite();
        if (mySprite != nullptr && bulletSprite != nullptr &&
            mySprite->getGlobalBounds().intersects(bulletSprite->getGlobalBounds()) && (*it)->getId().compare(0, 6, "Bullet") == 0 && (*it)->getId().compare(6, 7, getId()) != 0)
        {
            printf("it->getid: %s getid: %s\n", (*it)->getId().c_str(), getId().c_str());

            printf("player destruido!\n");
            world->destroy(this);
        }
    }*/
}

void Player::handleInput(sf::Event &event, sf::RenderWindow &window)
{
    if (event.type == sf::Event::KeyPressed)
    {
        // MOVEMENT INPUT
        if (event.key.code == sf::Keyboard::D)
            incAngle = 0.5;
        if (event.key.code == sf::Keyboard::A)
            incAngle = -0.5;
        if (event.key.code == sf::Keyboard::W)
            speed = 0.3;
        if (event.key.code == sf::Keyboard::S)
            speed = -0.3;

        /*if (event.key.code == sf::Keyboard::Space)
            shoot();*/
    }

    if (event.type == sf::Event::KeyReleased)
    {
        if (event.key.code == sf::Keyboard::D)
            incAngle = 0;
        if (event.key.code == sf::Keyboard::A)
            incAngle = 0;
        if (event.key.code == sf::Keyboard::W)
            speed = 0;
        if (event.key.code == sf::Keyboard::S)
            speed = 0;
    }

    if (event.type == sf::Event::MouseButtonPressed)
    {
        printf("shoot!\n");
        shoot();
    }
    
    // TODO: ESTO DA ERROR CUANDO SE CIERRA LA VENTANA!!
    mouseX = sf::Mouse::getPosition(window).x;
    mouseY = sf::Mouse::getPosition(window).y;
}

void Player::shoot()
{
    /*if (world != nullptr)
    {
        float gunAngle = gun->getRotation();

        world->createBullet(x + cosf(gunAngle * PI / 180) * 100,
                            y + sinf(gunAngle * PI / 180) * 100,
                            gunAngle, getId());
    }*/
    /*
    
    // TODO: ???
    // coger del pool de balas
    // si no hay ninguna desactivada, se crea una nueva
    // EL VECTOR DE BALAS TIENE QUE TENERLO EL GAMEWORLD
    
    Bullet* b = new Bullet();
    bullets.push_back();
    */
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