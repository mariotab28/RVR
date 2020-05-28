#include "Player.h"

#include <SFML/Graphics.hpp>

#include "GameWorld.h"
#include <math.h>

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

    mouseX = sf::Mouse::getPosition(window).x;
    mouseY = sf::Mouse::getPosition(window).y;
}

void Player::update(sf::RenderWindow &window)
{
    // BASE UPDATE-------------
    GameObject::update(window);

    angle += incAngle;

    // GUN UPDATE-------------
    gun->setPosition(x, y);

    gun->setRotation(atan2(mouseY - y, mouseX - x) * 180 / PI);

    // pos + velocity
}

void Player::handleInput(sf::Event &event)
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
        shoot();
    }
}

void Player::shoot()
{

    if (world != nullptr)
    {
        float gunAngle = gun->getRotation();

        world->createBullet(x + cosf(gunAngle * PI / 180) * 100,
                            y + sinf(gunAngle * PI / 180) * 100,
                            gunAngle);
    }
    /*
    
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
}

int Player::from_bin(char *data)
{
    return 0;
}
