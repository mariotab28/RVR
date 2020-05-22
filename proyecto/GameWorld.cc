#include "GameWorld.h"

#include "GameObject.h"
#include <SFML/Graphics.hpp>

#include "GameObject.h"
#include "Player.h"
#include "Bullet.h"

GameWorld::GameWorld()
{
}

GameWorld::~GameWorld()
{
    for (auto go : gameObjects)
        delete go;
    for (auto tx : textures)
        delete tx;
    for (auto f : fonts)
        delete f;
}

void GameWorld::init()
{
    // TEST

    //event = new sf::Event();

    int highScore = 5;

    /*GameObject *tank1 = world.createSprite(0);
    tank1->setPosition(200, 200);
    tank1->setScale(0.5f, 0.5f);*/
    //tank1->setOrigin();

    // -----CREATE PLAYER------
    Player *player = new Player(this);
    player->setTexture(*textures[0]);
    player->setOrigin(player->getTexture()->getSize().x*0.5,
        player->getTexture()->getSize().y*0.5);
    player->setGunTexture(*textures[1]);
    gameObjects.push_back(player);

    player->setPosition(200, 200);
    player->setScale(0.5f, 0.5f);

    GameObject *text1 = createText(0, 20);
    text1->setPosition(20, 60);
    text1->setText("RANKING");

    GameObject *text2 = createText(0, 20);
    text2->setPosition(20, 20);
    text2->setText("HIGH SCORE: " + std::to_string(highScore));
}

GameObject *GameWorld::createSprite(int texture)
{
    GameObject *go = new GameObject(this, 1);
    go->setTexture(*textures[texture]);

    gameObjects.push_back(go);
    return go;
}

GameObject *GameWorld::createText(int font, int characterSize)
{
    GameObject *go = new GameObject(this, 0);
    go->setFont(*fonts[font]);
    //go->setCharacterSize(characterSize);
    gameObjects.push_back(go);
    return go;
}

bool GameWorld::loadTexture(const std::string &textureFilename)
{
    sf::Texture *texture = new sf::Texture();
    if (!texture->loadFromFile(textureFilename))
        return false;

    textures.push_back(texture);
    return true;
}

bool GameWorld::loadFont(const std::string &fontFilename)
{
    sf::Font *font = new sf::Font();
    if (!font->loadFromFile(fontFilename))
        return false;

    fonts.push_back(font);
    printf("font loaded\n");
    return true;
}

void GameWorld::createBullet(float posX, float posY, float angle)
{
    Bullet *b = new Bullet(this);
    b->setTexture(*textures[2]);
    b->setScale(0.8, 0.8);
    b->setPosition(posX, posY);
    b->setSpeed(2);
    b->setRotation(angle);
    b->setOrigin(b->getTexture()->getSize().x*0.5,
        b->getTexture()->getSize().y*0.5);

    gameObjects.push_back(b);
}

void GameWorld::render(sf::RenderWindow &window)
{
    for (auto go : gameObjects)
        go->render(window);
}

void GameWorld::update()
{
    for (auto go : gameObjects)
        go->update();
}

void GameWorld::handleInput(sf::RenderWindow &window)
{
    while (window.pollEvent(*event))
    {
        if (event->type == sf::Event::Closed)
            window.close();
        else //if (event.type == sf::Event::KeyPressed)
        {
            for (auto go : gameObjects)
                go->handleInput(*event);
        }
    }
}