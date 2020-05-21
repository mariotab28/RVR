#ifndef GAMEWORLD_H_
#define GAMEWORLD_H_

#include <vector>
#include <string>

class GameObject;
namespace sf
{
    class RenderWindow;
    class Texture;
    class Font;
};

class GameWorld
{
private:
    std::vector<GameObject *> gameObjects;

    std::vector<sf::Texture*> textures;
    std::vector<sf::Font*> fonts;

public:
    GameWorld();
    ~GameWorld();

    GameObject* createSprite(int texture);
    GameObject* createText(int font, int characterSize);

    bool loadTexture(const std::string& textureFilename);
    bool loadFont(const std::string& fontFilename);

    void render(sf::RenderWindow &window);
    void update();
    void handleInput(sf::RenderWindow &window);
};

#endif /* GAMEWORLD_H_ */
