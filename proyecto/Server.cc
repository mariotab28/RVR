//#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
//#include <SFML/Graphics/Rect.hpp>

#include "GameWorld.h"
#include "GameObject.h"

int main()
{
    // ----WINDOW CREATION-----
    sf::RenderWindow window(sf::VideoMode(800, 600), "Window title");
    window.setTitle("Bouncy Tanks - SERVER");
    // background color
    sf::Color bg(180, 180, 180); //grey

    // ----LOAD RESOURCES-----
    GameWorld world;
    world.loadTexture("assets/tankBase.png");
    world.loadTexture("assets/tankGun.png");

    world.loadFont("assets/arial.ttf");

    // TEST

    int highScore = 5;

    GameObject *tank1 = world.createSprite(0);
    tank1->setPosition(200, 200);
    tank1->setScale(0.5f, 0.5f);
    //tank1->setOrigin();

    GameObject *text1 = world.createText(0, 20);
    text1->setPosition(20, 60);
    text1->setText("RANKING");

    GameObject *text2 = world.createText(0, 20);
    text2->setPosition(20, 20);
    text2->setText("HIGH SCORE: " + std::to_string(highScore));

    //world.createGameObject();

    // TANK---------
    // collider
    /*sf::IntRect tankRect(0, 0, 50, 50);
    // render
    sf::Texture texture;
    if (!texture.loadFromFile("Assets/tankBase.png"))
    {
        return -1;
    }
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setScale(0.5,0.5);
    sprite.setOrigin(sprite.getTexture()->getSize().x*0.5,
        sprite.getTexture()->getSize().y*0.5);
    int posX, posY;
    posX = 200;
    posY = 200;
    int angle = 0;
    sprite.setPosition(posX, posY);*/

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed)
            {
                // MOVEMENT INPUT
                /*if (event.key.code == sf::Keyboard::D)
                    angle++;
                if (event.key.code == sf::Keyboard::A)
                    angle--;
                if (event.key.code == sf::Keyboard::W)
                    posY--;
                if (event.key.code == sf::Keyboard::S)
                    posY++;

                sprite.setPosition(posX, posY);
                sprite.setRotation(angle);*/
                ;
            }
        }

        // Clear screen
        window.clear(bg);

        // Render
        world.handleInput(window);
        world.update();
        world.render(window);

        // Update the window
        window.display();
    }

    return 0;
}