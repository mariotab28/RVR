#include "GameWorld.h"

#include "GameObject.h"
#include <SFML/Graphics.hpp>

#include <fstream>

#include "GameObject.h"
#include "Player.h"
#include "Bullet.h"
#include "Gear.h"

// sort function
static bool sortInv(const std::pair<int, int> &a,
                    const std::pair<int, int> &b)
{
    return (a.first > b.first);
}

//---------------------------------------------------

GameWorld::GameWorld() : BTMessage("")
{
    PLAYERS_SIZE = 8;
    GEARS_SIZE = 2;
    BULLETS_SIZE = 50;
    WALLS_SIZE = 10;

    initialGears = 2;
    leftX = 200;
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

void GameWorld::createObjects()
{
    for (int i = 0; i < WALLS_SIZE; i++)
    {
        GameObject *wall = new GameObject(this, 1);
        gameObjects.push_back(wall);
        walls.push_back(wall);

        wall->setTexture(*textures[4]);
        wall->setOrigin(wall->getTexture()->getSize().x * 0.5,
                        wall->getTexture()->getSize().y * 0.5);
        wall->setId("Wall");
    }
    for (int i = 0; i < GEARS_SIZE; i++)
    {
        Gear *gear = new Gear(this);
        gameObjects.push_back(gear);
        gears.push_back(gear);

        gear->setTexture(*textures[3]);
        gear->setOrigin(gear->getTexture()->getSize().x * 0.5,
                        gear->getTexture()->getSize().y * 0.5);
        gear->setScale(0.4, 0.4);
    }
    for (int i = 0; i < BULLETS_SIZE; i++)
    {
        Bullet *bullet = new Bullet(this);
        gameObjects.push_back(bullet);
        bullets.push_back(bullet);

        bullet->setTexture(*textures[2]);
        bullet->setOrigin(bullet->getTexture()->getSize().x * 0.5,
                          bullet->getTexture()->getSize().y * 0.5);
        bullet->setScale(0.7, 0.5);
    }
    for (int i = 0; i < PLAYERS_SIZE; i++)
    {
        // players
        Player *player = new Player(this);
        gameObjects.push_back(player);
        players.push_back(player);

        player->setTexture(*textures[0]);
        player->setOrigin(player->getTexture()->getSize().x * 0.5,
                          player->getTexture()->getSize().y * 0.5);
        player->setGunTexture(*textures[1]);
        player->setScale(0.4f, 0.4f);

        // scoreTexts
        GameObject *scoreText = new GameObject(this, 0);
        gameObjects.push_back(scoreText);
        scoreTexts.push_back(scoreText);

        scoreText->setFont(*fonts[0]);

        // playerTexts
        GameObject *playerText = new GameObject(this, 0);
        gameObjects.push_back(playerText);
        playerTexts.push_back(playerText);

        playerText->setFont(*fonts[0]);
    }

    // ranking text
    GameObject *rankingText = new GameObject(this, 0);
    gameObjects.push_back(rankingText);

    rankingText->setFont(*fonts[0]);
    rankingText->setActive(true);
    rankingText->setText("RANKING");
    rankingText->setPosition(15, 90);

    // highscore texts
    GameObject *highscoreTitleText = new GameObject(this, 0);
    gameObjects.push_back(highscoreTitleText);

    highscoreTitleText->setFont(*fonts[0]);
    highscoreTitleText->setActive(true);
    highscoreTitleText->setText("HIGHSCORE");
    highscoreTitleText->setPosition(10, 5);

    highscoreText = new GameObject(this, 0);
    gameObjects.push_back(highscoreText);

    highscoreText->setFont(*fonts[0]);
    highscoreText->setActive(true);
    highscoreText->setPosition(12, 32);
}

void GameWorld::init(sf::RenderWindow &window, int level)
{
    // read highscore data from the file "data.txt"
    std::ifstream file;
    file.open("data.txt");

    if (file.is_open())
    {
        file >> highScoreNick >> highScore;
        file.close();
    }

    highscoreText->setText(highScoreNick + ": " + std::to_string(highScore));

    // create map bounds with 4 walls
    std::vector<std::pair<float, float>> boundsWalls = {{100, window.getSize().y / 2},
                                                        {window.getSize().x / 2, 0},
                                                        {window.getSize().x, window.getSize().y / 2},
                                                        {window.getSize().x / 2, window.getSize().y}};

    for (int i = 0; i < 4; i++)
    {
        GameObject *w = getObjectFromPool(walls);
        w->setPosition(boundsWalls[i].first, boundsWalls[i].second);
        if (i == 0)
            w->setScale(leftX, window.getSize().y);
        else if (i % 2 != 0)
            w->setScale(window.getSize().x, 50);
        else
            w->setScale(50, window.getSize().y);
        w->setActive(true);
    }

    // create the level chosen
    switch (level)
    {
    case 0:
    {
        createWall({400, 150}, {50, 300});
        createWall({600, 450}, {50, 300});
        break;
    }
    case 1:
    {
        createWall({320, 220}, {50, 200});
        createWall({580, 420}, {200, 50});
        createWall({580, 220}, {100, 100});
        break;
    }
    case 2:
    {
        createWall({500, 300}, {300, 100});
        createWall({500, 300}, {100, 300});
        break;
    }
    default:
        break;
    }

    // create initial gears
    for (int i = 0; i < initialGears; i++)
        createGear(window);
}

int GameWorld::createPlayer(const std::string &nick, sf::RenderWindow &window)
{
    int i = 0;

    while (i < players.size() && players[i]->isActive())
        i++;

    if (i < players.size())
    {
        Player *p = static_cast<Player *>(getObjectFromPool(players));

        if (p != nullptr)
        {
            bool generate = false;
            do
            {
                p->setPosition((rand() % (window.getSize().x - leftX + 1)) + leftX, rand() % window.getSize().y);
                generate = false;

                // check collision with a player--------------
                std::vector<GameObject *> players = getPlayers();

                for (auto it = players.begin(); it != players.end(); ++it)
                {
                    sf::Sprite *mySprite = p->getSprite();
                    sf::Sprite *playerSprite = (*it)->getSprite();
                    if (mySprite != nullptr && playerSprite != nullptr &&
                        (*it)->isActive() &&
                        mySprite->getGlobalBounds().intersects(playerSprite->getGlobalBounds()) &&
                        (*it)->getId().compare(0, 6, "Player") == 0)
                    {
                        generate = true;
                    }
                }

                // check collision with a wall--------------
                std::vector<GameObject *> walls = getWalls();

                for (auto it = walls.begin(); it != walls.end(); ++it)
                {
                    sf::Sprite *mySprite = p->getSprite();
                    sf::Sprite *wallSprite = (*it)->getSprite();
                    if (mySprite != nullptr && wallSprite != nullptr &&
                        (*it)->isActive() &&
                        mySprite->getGlobalBounds().intersects(wallSprite->getGlobalBounds()) &&
                        (*it)->getId().compare(0, 4, "Wall") == 0)
                    {
                        generate = true;
                    }
                }

                // check collision with a gear--------------
                std::vector<GameObject *> gears = getGears();

                for (auto it = gears.begin(); it != gears.end(); ++it)
                {
                    sf::Sprite *mySprite = p->getSprite();
                    sf::Sprite *gearSprite = (*it)->getSprite();
                    if (mySprite != nullptr && gearSprite != nullptr &&
                        (*it)->isActive() &&
                        mySprite->getGlobalBounds().intersects(gearSprite->getGlobalBounds()) &&
                        (*it)->getId().compare(0, 4, "Gear") == 0)
                    {
                        generate = true;
                    }
                }
            } while (generate);

            p->setActive(true);
            p->setId("Player" + std::to_string(i));
            p->setNick(nick);

            // player text
            playerTexts[i]->setActive(true);
            playerTexts[i]->setText("P" + std::to_string(i + 1));

            // score text
            scoreTexts[i]->setActive(true);
            updateScoreTexts();
        }
    }
    else
    {
        printf("ERROR: there are no deactivated players\n");
        i = -1;
    }

    return i;
}

void GameWorld::removePlayer(int i)
{
    if (i < players.size() && players[i] != nullptr && players[i]->isActive())
    {
        players[i]->setActive(false);
        static_cast<Player *>(players[i])->setPoints(0);
        playerTexts[i]->setActive(false);
        scoreTexts[i]->setActive(false);

        updateScoreTexts();
    }
    else
        printf("ERROR: trying to remove a invalid player\n");
}

void GameWorld::createWall(const std::pair<float, float> &pos, const std::pair<float, float> &scale)
{
    GameObject *w = getObjectFromPool(walls);
    w->setScale(scale.first, scale.second);
    w->setPosition(pos.first, pos.second);
    w->setActive(true);
}

GameObject *GameWorld::getObjectFromPool(const std::vector<GameObject *> &pool)
{
    GameObject *go = nullptr;
    int i = 0;
    while (i < pool.size() && pool[i]->isActive())
        i++;

    if (i < pool.size())
        go = pool[i];

    return go;
}

std::vector<GameObject *> GameWorld::getGameObjects()
{
    return gameObjects;
}

std::vector<GameObject *> GameWorld::getPlayers()
{
    return players;
}

std::vector<GameObject *> GameWorld::getGears()
{
    return gears;
}

std::vector<GameObject *> GameWorld::getBullets()
{
    return bullets;
}

std::vector<GameObject *> GameWorld::getWalls()
{
    return walls;
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
    return true;
}

void GameWorld::createBullet(float posX, float posY, float angle, std::string ownerId)
{
    Bullet *b = static_cast<Bullet *>(getObjectFromPool(bullets));

    if (b != nullptr)
    {
        b->setPosition(posX, posY);
        b->setSpeed(300);
        b->setRotation(angle);
        b->setId("Bullet" + ownerId);
        b->setActive(true);
        b->resetBounces();
    }
}

void GameWorld::updateScoreTexts()
{
    std::vector<std::pair<int, int>> aux;

    for (int i = 0; i < players.size(); i++)
    {
        if (players[i]->isActive())
        {
            aux.push_back({static_cast<Player *>(players[i])->getPoints(), i});
            scoreTexts[i]->setText("P" + std::to_string(i + 1) + " " + static_cast<Player *>(players[i])->getNick() +
                                   " " + std::to_string(static_cast<Player *>(players[i])->getPoints()));
        }
    }

    // sort from major to minor score
    std::sort(aux.begin(), aux.end(), sortInv);

    for (int j = 0; j < aux.size(); j++)
        scoreTexts[aux[j].second]->setPosition(14, 120 + j * 30);

    // update and write highscore in file "data.txt"
    // if best score is greater
    if (aux.size() > 0 && highScore < aux[0].first)
    {
        highScore = aux[0].first;
        highScoreNick = static_cast<Player *>(players[aux[0].second])->getNick();

        std::ofstream file;
        file.open("data.txt");

        if (file.is_open())
        {
            file << highScoreNick << " " << highScore;
            file.close();
        }

        highscoreText->setText(highScoreNick + ": " + std::to_string(highScore));
    }
}

void GameWorld::updatePlayerTexts()
{
    for (int i = 0; i < playerTexts.size(); i++)
    {
        if (players[i]->isActive())
            playerTexts[i]->setPosition(players[i]->getX() - 20, players[i]->getY() - 70);
    }
}

void GameWorld::createGear(const sf::RenderWindow &window)
{
    Gear *g = static_cast<Gear *>(getObjectFromPool(gears));

    if (g != nullptr)
    {
        g->setActive(true);

        bool generate = false;
        do
        {
            g->setPosition((rand() % (window.getSize().x - leftX + 1)) + leftX, rand() % window.getSize().y);

            generate = false;

            // check collision with a player--------------
            std::vector<GameObject *> players = getPlayers();

            for (auto it = players.begin(); it != players.end(); ++it)
            {
                sf::Sprite *mySprite = g->getSprite();
                sf::Sprite *playerSprite = (*it)->getSprite();
                if (mySprite != nullptr && playerSprite != nullptr &&
                    (*it)->isActive() &&
                    mySprite->getGlobalBounds().intersects(playerSprite->getGlobalBounds()) && (*it)->getId().compare(0, 6, "Player") == 0)
                {
                    generate = true;
                }
            }

            // check collision with a wall--------------
            std::vector<GameObject *> walls = getWalls();

            for (auto it = walls.begin(); it != walls.end(); ++it)
            {
                sf::Sprite *mySprite = g->getSprite();
                sf::Sprite *wallSprite = (*it)->getSprite();
                if (mySprite != nullptr && wallSprite != nullptr &&
                    (*it)->isActive() &&
                    mySprite->getGlobalBounds().intersects(wallSprite->getGlobalBounds()) &&
                    (*it)->getId().compare(0, 4, "Wall") == 0)
                {
                    generate = true;
                }
            }
        } while (generate);
    }
}

void GameWorld::render(sf::RenderWindow &window)
{
    for (auto go : gameObjects)
    {
        if (go->isActive())
            go->render(window);
    }
}

void GameWorld::update(sf::RenderWindow &window, float deltaTime)
{
    for (auto go : gameObjects)
    {
        if (go->isActive())
            go->update(window, deltaTime);
    }

    updatePlayerTexts();
}

bool GameWorld::handleInput(sf::RenderWindow &window, BTMessage &message)
{
    result = false;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
            result = true;
        }
        else
        {
            if (event.type == sf::Event::KeyPressed)
            {
                message.message = "Press";
                // MOVEMENT INPUT
                if (event.key.code == sf::Keyboard::D)
                    appendMessage(message, "D");
                if (event.key.code == sf::Keyboard::A)
                    appendMessage(message, "A");
                if (event.key.code == sf::Keyboard::W)
                    appendMessage(message, "W");
                if (event.key.code == sf::Keyboard::S)
                    appendMessage(message, "S");
                // GUN ROTATION INPUT
                if (event.key.code == sf::Keyboard::Q)
                    appendMessage(message, "Q");
                if (event.key.code == sf::Keyboard::E)
                    appendMessage(message, "E");
            }

            if (event.type == sf::Event::KeyReleased)
            {
                message.message = "Relea";
                if (event.key.code == sf::Keyboard::D)
                    appendMessage(message, "D");
                if (event.key.code == sf::Keyboard::A)
                    appendMessage(message, "A");
                if (event.key.code == sf::Keyboard::W)
                    appendMessage(message, "W");
                if (event.key.code == sf::Keyboard::S)
                    appendMessage(message, "S");
                if (event.key.code == sf::Keyboard::Q)
                    appendMessage(message, "Q");
                if (event.key.code == sf::Keyboard::E)
                    appendMessage(message, "E");
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                message.message = "Mouse";
                result = true;
            }
        }
    }

    return result;
}

void GameWorld::appendMessage(BTMessage &message, const char *c)
{
    message.message.append(c);
    result = true;
}

void GameWorld::processInput(BTMessage message)
{
    if (players[message.index]->isActive())
        static_cast<Player *>(players[message.index])->processInput(message);
}

void GameWorld::to_bin()
{
    MESSAGE_SIZE = 0;

    // calculate GameWorld message_size
    for (auto go : gameObjects)
    {
        go->to_bin();
        MESSAGE_SIZE += go->MESSAGE_SIZE;
    }

    alloc_data(MESSAGE_SIZE);
    memset(_data, 0, MESSAGE_SIZE);

    // serialize each gameObject
    for (auto go : gameObjects)
    {
        memcpy(_data, static_cast<void *>(go->data()), go->size());
        _data += go->size();
    }

    _data -= MESSAGE_SIZE;
}

int GameWorld::from_bin(char *data)
{
    try
    {
        // deserialize each gameObject
        for (auto go : gameObjects)
        {
            go->from_bin(data);
            data += go->size();
        }
        return 0;
    }
    catch (const std::exception &e)
    {
        return -1;
    }
}