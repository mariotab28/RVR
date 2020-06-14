#ifndef GAMEWORLD_H_
#define GAMEWORLD_H_

#include <vector>
#include <string>
#include "BTMessage.h"
#include <SFML/Graphics.hpp>

class GameObject;
class Player;
namespace sf
{
    class RenderWindow;
    class Texture;
    class Font;
    class Event;
}; // namespace sf

class GameWorld : public BTMessage
{
private:
    sf::Event event;

    std::vector<GameObject *> gameObjects;

    std::vector<sf::Texture *> textures;
    std::vector<sf::Font *> fonts;

    // test
    //Player *player2;
    //std::string text;
    GameObject* text1;
    GameObject *playerText;

    int highScore;

public:
    GameWorld();
    ~GameWorld();

    void init();
    void clientInit();

    GameObject *createSprite(int texture);
    GameObject *createText(int font, int characterSize);

    std::vector<GameObject *> getGameObjects();

    bool loadTexture(const std::string &textureFilename);
    bool loadFont(const std::string &fontFilename);

    void createBullet(float posX, float posY, float angle, std::string ownerId);
    void destroy(GameObject* go);

    void updateScores();

    void createGear(const sf::RenderWindow& window);

    void render(sf::RenderWindow &window);
    void update(sf::RenderWindow &window);
    void handleInput(sf::RenderWindow &window);

    virtual void to_bin();
    virtual int from_bin(char * data);
};

#endif /* GAMEWORLD_H_ */
