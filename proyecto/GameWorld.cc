#include "GameWorld.h"

#include "GameObject.h"
#include <SFML/Graphics.hpp>

#include <fstream>

#include "GameObject.h"
#include "Player.h"
#include "Bullet.h"
#include "Gear.h"

static bool sortInv(const std::pair<int, int> &a,
                    const std::pair<int, int> &b)
{
    return (a.first > b.first);
}

GameWorld::GameWorld() : BTMessage("")
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

    GameObject *rankingText = new GameObject(this, 0);
    gameObjects.push_back(rankingText);

    rankingText->setFont(*fonts[0]);
    rankingText->setActive(true);
    rankingText->setText("RANKING");
    rankingText->setPosition(15, 70);

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
    highscoreText->setPosition(12, 30);
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
    printf("%s: %d\n", highScoreNick.c_str(), highScore);

    highscoreText->setText(highScoreNick + ": " + std::to_string(highScore));

    // map bounds
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

    // create level
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
    {
        createGear(window);
    }
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

            // score
            scoreTexts[i]->setActive(true);
            //scoreTexts[i]->setText("P" + std::to_string(i) + " " + nick + " 0");
            // pos Y

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
    if (players[i] != nullptr && players[i]->isActive())
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

    //printf("pool size: %d i: %d\n", pool.size(), i);

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
    printf("font loaded\n");
    return true;
}

void GameWorld::createBullet(float posX, float posY, float angle, std::string ownerId)
{
    Bullet *b = static_cast<Bullet *>(getObjectFromPool(bullets));

    if (b != nullptr)
    {
        b->setPosition(posX, posY);
        b->setSpeed(500);
        b->setRotation(angle);
        b->setId("Bullet" + ownerId);
        b->setActive(true);
        b->resetBounces();
    }
}

void GameWorld::updateScoreTexts()
{
    // crear aux <points, index>
    std::vector<std::pair<int, int>> aux;

    // inicializo aux
    for (int i = 0; i < players.size(); i++)
    {
        if (players[i]->isActive())
        {
            aux.push_back({static_cast<Player *>(players[i])->getPoints(),
                           i});
            scoreTexts[i]->setText("P" + std::to_string(i + 1) + " " + static_cast<Player *>(players[i])->getNick() +
                                   " " + std::to_string(static_cast<Player *>(players[i])->getPoints()));
        }
    }

    // ordeno aux
    std::sort(aux.begin(), aux.end(), sortInv);

    // recorro aux
    for (int j = 0; j < aux.size(); j++)
    {
        scoreTexts[aux[j].second]->setPosition(14, 100 + j * 30);
    }

    // update highscore
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
        //printf("%s: %d\n", highScoreNick.c_str(), highScore);

        highscoreText->setText(highScoreNick + ": " + std::to_string(highScore));
    }
}

void GameWorld::updatePlayerTexts()
{
    for (int i = 0; i < playerTexts.size(); i++)
    {
        if (players[i]->isActive())
        {
            playerTexts[i]->setPosition(players[i]->getX() - 20, players[i]->getY() - 70);
        }
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

void GameWorld::update(sf::RenderWindow &window, sf::Time &elapsedTime)
{
    for (auto go : gameObjects)
    {
        if (go->isActive())
            go->update(window, elapsedTime);
    }

    updatePlayerTexts();
    //updateScoreTexts();

    /*highScore++;
    text1->setText("RANKING" + std::to_string(highScore));*/
}

bool GameWorld::handleInput(sf::RenderWindow &window, BTMessage &message)
{
    bool result = false;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
            result = true;
        }
        else
        { // LLAMAR AL HANDLEINPUT DE 1 PLAYER SOLO
            /*for (auto go : gameObjects)
            {
                if (go->isActive())
                    go->handleInput(event, window);
            }*/

            if (event.type == sf::Event::KeyPressed)
            {
                message.message = "Press";
                // MOVEMENT INPUT
                if (event.key.code == sf::Keyboard::D)
                {
                    message.message.append("D");
                    result = true;
                }
                if (event.key.code == sf::Keyboard::A)
                {
                    message.message.append("A");
                    result = true;
                }
                if (event.key.code == sf::Keyboard::W)
                {
                    message.message.append("W");
                    result = true;
                }
                if (event.key.code == sf::Keyboard::S)
                {
                    message.message.append("S");
                    result = true;
                }
                if (event.key.code == sf::Keyboard::Q)
                {
                    message.message.append("Q");
                    result = true;
                }
                if (event.key.code == sf::Keyboard::E)
                {
                    message.message.append("E");
                    result = true;
                }
            }

            if (event.type == sf::Event::KeyReleased)
            {
                message.message = "Relea";
                if (event.key.code == sf::Keyboard::D)
                {
                    message.message.append("D");
                    result = true;
                }
                if (event.key.code == sf::Keyboard::A)
                {
                    message.message.append("A");
                    result = true;
                }
                if (event.key.code == sf::Keyboard::W)
                {
                    message.message.append("W");
                    result = true;
                }
                if (event.key.code == sf::Keyboard::S)
                {
                    message.message.append("S");
                    result = true;
                }
                if (event.key.code == sf::Keyboard::Q)
                {
                    message.message.append("Q");
                    result = true;
                }
                if (event.key.code == sf::Keyboard::E)
                {
                    message.message.append("E");
                    result = true;
                }
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                message.message = "Mouse";
                result = true;
            }

            // TODO: ESTO DA ERROR CUANDO SE CIERRA LA VENTANA!!
            /*if(mouseX != sf::Mouse::getPosition(window).x)
            {
                mouseX = sf::Mouse::getPosition(window).x;
                message.mouseX = sf::Mouse::getPosition(window).x;
                result = true;
            }

            if(mouseY != sf::Mouse::getPosition(window).y)
            {
                mouseY = sf::Mouse::getPosition(window).y;
                message.mouseY = sf::Mouse::getPosition(window).y;
                result = true;
            }*/

            //message.mouseY = sf::Mouse::getPosition(window).y;
        }
    }

    return result;
}

void GameWorld::processInput(BTMessage message)
{
    //printf("process input index: %d\n", message.index);
    if (players[message.index]->isActive())
        static_cast<Player *>(players[message.index])->processInput(message);
}

void GameWorld::to_bin()
{
    //BTMessage::to_bin();

    MESSAGE_SIZE = 0;

    //printf("gameobjects size: %d\n", gameObjects.size());

    // Calculate GameWorld message_size
    for (auto go : gameObjects)
    //for (size_t i = 0; i < gameObjects.size(); i++)
    {
        go->to_bin();
        MESSAGE_SIZE += go->MESSAGE_SIZE;
        //printf("go size: %d\n", go->MESSAGE_SIZE);
        //printf("%d\n", gameObjects.size());
    }
    //printf("eeeee\n");
    //printf("%s\n",gameObjects[0]->getId().c_str());
    //std::cout << MESSAGE_SIZE << "\n";

    alloc_data(MESSAGE_SIZE);
    //printf("ooooo\n");
    memset(_data, 0, MESSAGE_SIZE);

    // Serialize each gameObject
    for (auto go : gameObjects)
    {
        // serializar goType
        memcpy(_data, static_cast<void *>(go->data()), go->size());

        //printf("go data: %c\n", _data);
        _data += go->size();

        //printf("copied go size: %d\n", go->size());
    }

    //printf("finished: %d\n", MESSAGE_SIZE);

    _data -= MESSAGE_SIZE;

    //printf("gameobjects size: %d\n", gameObjects.size());

    //printf("data size: %d\n", strlen(_data));
    //printf("tobin done\n");
    //std::cout << gameObjects[0]->data() << "\n";
}

int GameWorld::from_bin(char *data)
{
    //std::cout << "gameWorld from_bin" << "\n";
    for (auto go : gameObjects)
    {
        if (go == nullptr)
            printf("null\n");
        else
            go->from_bin(data);

        data += go->size();

        //printf("size: %d\n", go->size());
        //printf("a\n");
    }

    //printf("gear active: %d posX: %f\n", gameObjects[8]->isActive(), gameObjects[8]->getX());

    //printf("frombin finished\n");
    //printf("1x: %f 1y: %f\n", gameObjects[0]->getX(), gameObjects[0]->getY());
    //printf("2x: %f 2y: %f\n", gameObjects[1]->getX(), gameObjects[1]->getY());
    //std::cout << "angle: " << gameObjects[0]->getRotation() << "\n";

    return 0;
}