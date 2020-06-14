#include "GameObject.h"

#include <SFML/Graphics.hpp>

#include <math.h>
#include <string.h>

// TODO: PONER EL NICK AL QUE PERTENECE EL GAMEOBJECT
GameObject::GameObject(GameWorld *world, int goType)
    : BTMessage(""), world(world), goType(goType)
{
    MESSAGE_SIZE += sizeof(uint8_t) + sizeof(char) * 20 + sizeof(float) * 3;

    id = "";
    x = 0;
    y = 0;
    angle = 0;
    speed = 0;
    dirX = 0;
    dirY = 0;

    switch (goType)
    {
    case 0: // TEXT GAMEOBJECT
    {
        entity = new sf::Text();
        break;
    }
    case 1: // SPRITE GAMEOBJECT
    {
        entity = new sf::Sprite();
        break;
    }
    default:
    {
        printf("ERROR: invalid type of GO\n");
        break;
    }
    }
}

GameObject::~GameObject()
{
    if (entity != nullptr)
        delete entity;
}

void GameObject::setText(const std::string &text)
{
    if (goType == 0)
        static_cast<sf::Text *>(entity)->setString(text);
    else
        printf("ERROR: trying to setText to a non-text GO\n");
}

void GameObject::setTexture(const sf::Texture &texture)
{
    if (goType == 1)
        static_cast<sf::Sprite *>(entity)->setTexture(texture);
    else
        printf("ERROR: trying to setTexture to a non-sprite GO\n");
}

const sf::Texture *GameObject::getTexture()
{
    if (goType == 1)
        return static_cast<sf::Sprite *>(entity)->getTexture();
    else
        printf("ERROR: trying to getTexture from a non-sprite GO\n");
    return nullptr;
}

void GameObject::setFont(const sf::Font &font)
{
    if (goType == 0)
        static_cast<sf::Text *>(entity)->setFont(font);
    else
        printf("ERROR: trying to setFont to a non-text GO\n");
}

void GameObject::setPosition(float x, float y)
{
    this->x = x;
    this->y = y;
    entity->setPosition(x, y);
}

void GameObject::setRotation(float angle)
{
    this->angle = angle;
    entity->setRotation(angle);
}

void GameObject::setScale(float factorX, float factorY)
{
    entity->setScale(factorX, factorY);
}

void GameObject::setOrigin(float originX, float originY)
{
    entity->setOrigin(originX, originY);
}

void GameObject::setDir(float dirX, float dirY)
{
    this->dirX = dirX;
    this->dirY = dirY;
}

void GameObject::setId(const std::string &id)
{
    this->id = id;
}

/*const std::pair<float, float> GameObject::getDir()
{
    return std::pair<float, float>(dirX, dirY);
}*/

float GameObject::getX()
{
    return x;
}
float GameObject::getY()
{
    return y;
}

float GameObject::getRotation()
{
    return angle;
}

void GameObject::setSpeed(float speed)
{
    this->speed = speed;
}

std::string GameObject::getId()
{
    return id;
}

sf::Sprite *GameObject::getSprite()
{
    if (goType == 0)
    {
        //printf("ERROR: trying to getSprite from a non-sprite GO\n");
        return nullptr;
    }

    return static_cast<sf::Sprite *>(entity);
}

void GameObject::render(sf::RenderWindow &window)
{
    //std::cout << "goType: " << std::to_string(goType) << "\n";
    if (goType == 0)
        window.draw(*static_cast<sf::Text *>(entity));
    else
    {
        window.draw(*static_cast<sf::Sprite *>(entity));
        //window.draw(*static_cast<sf::Sprite *>(entity)->getGlobalBounds());
    }
}

void GameObject::update(sf::RenderWindow &window)
{
    // pos + velocity
    x += dirX * speed;
    y += dirY * speed;
    setPosition(x, y);

    dirX = cosf(angle * PI / 180);
    dirY = sinf(angle * PI / 180);
    setRotation(angle);
}

void GameObject::handleInput(sf::Event &event)
{
}

void GameObject::to_bin()
{
    //BTMessage::to_bin();

    MESSAGE_SIZE = sizeof(uint8_t) + 20 * sizeof(char) + 3 * sizeof(float);

    alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);

    // serializar goType
    memcpy(_data, static_cast<void *>(&goType), sizeof(uint8_t));
    _data += sizeof(uint8_t);

    // serializar id
    memcpy(_data, static_cast<void *>((char *)id.c_str()), 20 * sizeof(char));
    _data += 20 * sizeof(char);

    //printf("size id: %d\n", id.size());

    // serializar x
    memcpy(_data, static_cast<void *>(&x), sizeof(float));
    _data += sizeof(float);

    // serializar y
    memcpy(_data, static_cast<void *>(&y), sizeof(float));
    _data += sizeof(float);

    // serializar angle
    memcpy(_data, static_cast<void *>(&angle), sizeof(float));
    _data += sizeof(float);

    // colocamos el puntero al inicio del fichero
    _data -= MESSAGE_SIZE;
}

int GameObject::from_bin(char *data)
{
    try
    {
        _size = 0;
        // deserializamos goType
        memcpy(static_cast<void *>(&goType), data, sizeof(uint8_t));
        data += sizeof(uint8_t);
        _size += sizeof(uint8_t);
        //printf("goType: %d\n", goType);

        // deserializamos id
        char auxId[20];
        memcpy(static_cast<void *>(&auxId), data, 20 * sizeof(char));
        data += 20 * sizeof(char);
        _size += 20 * sizeof(char);
        auxId[19] = '\0';
        id = auxId;
        
        //printf("id: %s\n", id.c_str());

        // deserializamos x
        memcpy(static_cast<void *>(&x), data, sizeof(float));
        data += sizeof(float);
        _size += sizeof(float);

        // deserializamos y
        memcpy(static_cast<void *>(&y), data, sizeof(float));
        data += sizeof(float);
        _size += sizeof(float);

        setPosition(x, y);

        // deserializamos angle
        memcpy(static_cast<void *>(&angle), data, sizeof(float));
        data += sizeof(float);
        _size += sizeof(float);

        setRotation(angle);

        //data -= MESSAGE_SIZE;

        return 0;
    }
    catch (const std::exception &e)
    {
        return -1;
    }
}