#ifndef GAMEWORLD_H_
#define GAMEWORLD_H_

#include <vector>

class GameObject;
namespace sf
{
    class RenderWindow;
};

class GameWorld
{
private:
    std::vector<GameObject *> gameObjects;

public:
    GameWorld();
    ~GameWorld();

    void createGameObject();

    void render(sf::RenderWindow &window);
    void update();
    void handleInput();
};

#endif /* GAMEWORLD_H_ */
