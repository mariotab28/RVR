#ifndef GAMEWORLD_H_
#define GAMEWORLD_H_

#include <vector>
#include <string>

class GameObject;
class Player;
namespace sf
{
    class RenderWindow;
    class Texture;
    class Font;
    class Event;
}; // namespace sf

class GameWorld
{
private:
    sf::Event *event;

    std::vector<GameObject *> gameObjects;

    std::vector<sf::Texture *> textures;
    std::vector<sf::Font *> fonts;

    // test
    Player *player;
    Player *player2;
    std::string text;
    GameObject *playerText;

public:
    GameWorld();
    ~GameWorld();

    void init();

    GameObject *createSprite(int texture);
    GameObject *createText(int font, int characterSize);

    std::vector<GameObject *> getGameObjects();

    bool loadTexture(const std::string &textureFilename);
    bool loadFont(const std::string &fontFilename);

    void createBullet(float posX, float posY, float angle, std::string ownerId);
    void destroy(GameObject* go);

    void updateScores();

    void render(sf::RenderWindow &window);
    void update(sf::RenderWindow &window);
    void handleInput(sf::RenderWindow &window);
};

#endif /* GAMEWORLD_H_ */
