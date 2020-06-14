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

    // 8 players, 1 gear, 50 bullets

    int PLAYERS_SIZE = 8;
    int GEARS_SIZE = 1;
    int BULLETS_SIZE = 50;

    sf::Event event;

    std::vector<GameObject *> gameObjects;

    std::vector<GameObject *> players;
    std::vector<GameObject *> gears;
    std::vector<GameObject *> bullets;

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
    void createObjects();

    void createPlayer(int i, const std::string& nick);

    GameObject* getObjectFromPool(const std::vector<GameObject*>& pool);

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
    bool handleInput(sf::RenderWindow &window, BTMessage& message);

    void processInput(BTMessage message);

    virtual void to_bin();
    virtual int from_bin(char * data);
};

#endif /* GAMEWORLD_H_ */
