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
    class Time;
}; // namespace sf

class GameWorld : public BTMessage
{
private:
    int PLAYERS_SIZE;
    int GEARS_SIZE;
    int BULLETS_SIZE;
    int WALLS_SIZE;

    int initialGears;
    int leftX;

    sf::Event event;
    bool result;

    // resources
    std::vector<sf::Texture *> textures;
    std::vector<sf::Font *> fonts;

    std::vector<GameObject *> gameObjects;

    // pools
    std::vector<GameObject *> players;
    std::vector<GameObject *> gears;
    std::vector<GameObject *> bullets;
    std::vector<GameObject *> walls;
    std::vector<GameObject *> playerTexts;
    std::vector<GameObject *> scoreTexts;

    GameObject *highscoreText;

    int highScore;
    std::string highScoreNick;

public:
    GameWorld();
    ~GameWorld();

    void init(sf::RenderWindow &window, int level);
    void createObjects();

    int createPlayer(const std::string &nick, sf::RenderWindow &window);
    void createBullet(float posX, float posY, float angle, std::string ownerId);
    void createWall(const std::pair<float, float> &pos, const std::pair<float, float> &scale);
    void createGear(const sf::RenderWindow &window);

    void removePlayer(int i);

    GameObject *getObjectFromPool(const std::vector<GameObject *> &pool);

    std::vector<GameObject *> getGameObjects();
    std::vector<GameObject *> getPlayers();
    std::vector<GameObject *> getGears();
    std::vector<GameObject *> getBullets();
    std::vector<GameObject *> getWalls();

    bool loadTexture(const std::string &textureFilename);
    bool loadFont(const std::string &fontFilename);

    void updateScoreTexts();
    void updatePlayerTexts();

    void render(sf::RenderWindow &window);
    void update(sf::RenderWindow &window, float deltaTime);
    bool handleInput(sf::RenderWindow &window, BTMessage &message);

    void processInput(BTMessage message);
    void appendMessage(BTMessage &message, const char *c);

    virtual void to_bin();
    virtual int from_bin(char *data);
};

#endif /* GAMEWORLD_H_ */
