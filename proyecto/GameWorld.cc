#include "GameWorld.h"

#include "GameObject.h"
#include <SFML/Graphics.hpp>

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

GameObject *GameWorld::createSprite(int texture)
{
    GameObject *go = new GameObject(1);
    go->setTexture(*textures[texture]);

    gameObjects.push_back(go);
    return go;
}

GameObject *GameWorld::createText(int font, int characterSize)
{
    GameObject *go = new GameObject(0);
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

/*void GameWorld::createGameObject()
{
    GameObject *go = new GameObject(0, 100, 100);

    gameObjects.push_back(go);
}*/

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
    for (auto go : gameObjects)
        go->handleInput(window);
}