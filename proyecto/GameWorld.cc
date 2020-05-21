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
}

void GameWorld::createGameObject()
{
    GameObject* go = new GameObject(1, 100, 100);

    gameObjects.push_back(go);
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

void GameWorld::handleInput()
{
}