#include "GameWorld.h"

#include "GameObject.h"
#include <SFML/Graphics.hpp>

#include "GameObject.h"
#include "Player.h"
#include "Bullet.h"
#include "Gear.h"

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
    // leer el highscore de un archivo!!!!
    int highScore = 5;

    // TEST GEARS-------

    Gear *g = new Gear(this);
    g->setTexture(*textures[3]);
    g->setScale(0.8, 0.8);
    g->setPosition(500, 500);
    g->setOrigin(g->getTexture()->getSize().x * 0.5,
                 g->getTexture()->getSize().y * 0.5);

    gameObjects.push_back(g);

    Gear *g2 = new Gear(this);
    g2->setTexture(*textures[3]);
    g2->setScale(0.8, 0.8);
    g2->setPosition(200, 500);
    g2->setOrigin(g2->getTexture()->getSize().x * 0.5,
                  g2->getTexture()->getSize().y * 0.5);

    gameObjects.push_back(g2);

    // -----CREATE PLAYER------
    player = new Player(this);
    player->setTexture(*textures[0]);
    player->setOrigin(player->getTexture()->getSize().x * 0.5,
                      player->getTexture()->getSize().y * 0.5);
    player->setGunTexture(*textures[1]);
    gameObjects.push_back(player);

    player->setPosition(200, 200);
    player->setScale(0.5f, 0.5f);
    player->setId("Player1");

    // PLAYER 2 TO TEST DEATH-----

    player2 = new Player(this);
    player2->setTexture(*textures[0]);
    player2->setOrigin(player2->getTexture()->getSize().x * 0.5,
                       player2->getTexture()->getSize().y * 0.5);
    player2->setGunTexture(*textures[1]);
    gameObjects.push_back(player2);

    player2->setPosition(500, 200);
    player2->setScale(0.5f, 0.5f);
    player2->setId("Player2");

    // ------CREATE TEXTS------

    GameObject *text1 = createText(0, 20);
    text1->setPosition(20, 60);
    text1->setText("RANKING");

    GameObject *text2 = createText(0, 20);
    text2->setPosition(20, 20);
    text2->setText("HIGH SCORE: " + std::to_string(highScore));

    text = "P1 POINTS: " + std::to_string(player->getPoints());

    playerText = createText(0, 20);
    playerText->setPosition(20, 80);
    playerText->setText(text);
}

std::vector<GameObject *> GameWorld::getGameObjects()
{
    return gameObjects;
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

void GameWorld::createBullet(float posX, float posY, float angle, std::string ownerId)
{
    Bullet *b = new Bullet(this);
    b->setTexture(*textures[2]);
    b->setScale(0.8, 0.8);
    b->setPosition(posX, posY);
    b->setSpeed(2);
    b->setRotation(angle);
    b->setOrigin(b->getTexture()->getSize().x * 0.5,
                 b->getTexture()->getSize().y * 0.5);
    b->setId("Bullet" + ownerId);
    //printf("Bullet%s\n", ownerId.c_str());

    gameObjects.push_back(b);
}

void GameWorld::destroy(GameObject *go)
{
    //find gameObject and destroys it
    auto it = std::find(gameObjects.begin(), gameObjects.end(), go);
    if (it != gameObjects.end())
    {
        delete *it;
        gameObjects.erase(it);
    }
    else
    {
        printf("ERROR: trying to destroy a GO that doesn't exist\n");
    }
}

void GameWorld::updateScores()
{
    text = "P1 POINTS: " + std::to_string(player->getPoints());
    playerText->setText(text);
}

void GameWorld::createGear(const sf::RenderWindow &window)
{
    Gear *g = new Gear(this);
    g->setTexture(*textures[3]);
    g->setScale(0.8, 0.8);
    g->setOrigin(g->getTexture()->getSize().x * 0.5,
                 g->getTexture()->getSize().y * 0.5);

    g->setPosition(rand() % window.getSize().x, rand() % window.getSize().y);

    // TODO: SI EL GEAR SE CREA EN UNA POSICION
    // DONDE YA HAY ALGO SE VUELVE A HACER EL RANDOM
    /*sf::Sprite *mySprite = getSprite();
    sf::Sprite *otherSprite = (*it)->getSprite();
    if(mySprite->getGlobalBounds().intersects(otherSprite->getGlobalBounds())*/

    gameObjects.push_back(g);
}

void GameWorld::render(sf::RenderWindow &window)
{
    for (auto go : gameObjects)
        go->render(window);
}

void GameWorld::update(sf::RenderWindow &window)
{
    for (auto go : gameObjects)
        go->update(window);
}

void GameWorld::handleInput(sf::RenderWindow &window)
{
    while (window.pollEvent(*event))
    {
        if (event->type == sf::Event::Closed)
            window.close();
        else
        {
            for (auto go : gameObjects)
                go->handleInput(*event);
        }
    }
}