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
    // 8 players, 1 gear, 50 bullets, 10 walls, 8 playertexts, 8 scoretexts
    int PLAYERS_SIZE = 8;
    int GEARS_SIZE = 2;
    int BULLETS_SIZE = 50;
    int WALLS_SIZE = 10;

    int initialGears = 2;

    int leftX = 200;

    sf::Event event;

    std::vector<GameObject *> gameObjects;

    std::vector<GameObject *> players;
    std::vector<GameObject *> gears;
    std::vector<GameObject *> bullets;
    std::vector<GameObject *> walls;
    std::vector<GameObject *> playerTexts;
    std::vector<GameObject *> scoreTexts;

    GameObject *highscoreText;

    std::vector<sf::Texture *> textures;
    std::vector<sf::Font *> fonts;

    // test
    //Player *player2;
    //std::string text;

    int highScore;
    std::string highScoreNick;

public:
    GameWorld();
    ~GameWorld();

    void init(sf::RenderWindow& window, int level);
    void createObjects();

    int createPlayer(const std::string& nick, sf::RenderWindow& window);
    void removePlayer(int i);

    void createWall(const std::pair<float, float>& pos, const std::pair<float, float>& scale);

    GameObject* getObjectFromPool(const std::vector<GameObject*>& pool);

    std::vector<GameObject *> getGameObjects();
    std::vector<GameObject *> getPlayers();
    std::vector<GameObject *> getGears();
    std::vector<GameObject *> getBullets();
    std::vector<GameObject *> getWalls();

    bool loadTexture(const std::string &textureFilename);
    bool loadFont(const std::string &fontFilename);

    void createBullet(float posX, float posY, float angle, std::string ownerId);

    void updateScoreTexts();
    void updatePlayerTexts();

    void createGear(const sf::RenderWindow& window);

    void render(sf::RenderWindow &window);
    void update(sf::RenderWindow &window, sf::Time& elapsedTime);
    bool handleInput(sf::RenderWindow &window, BTMessage& message);

    void processInput(BTMessage message);

    virtual void to_bin();
    virtual int from_bin(char * data);
};

#endif /* GAMEWORLD_H_ */
