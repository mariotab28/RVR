//#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
//#include <SFML/Graphics/Rect.hpp>

#include "GameWorld.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
    window.setTitle("Bouncy Tanks - SERVER");
    // background color
    sf::Color bg(180,180,180); //grey

    GameWorld world;

    world.createGameObject();



    // TANK---------
    // collider
    sf::IntRect tankRect(0, 0, 50, 50);
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
    sprite.setPosition(posX, posY);
    //--------------

    //texture.setSmooth
    //sf::Rectangle b1.setFillColor(sf::Color::Green);

    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile("Assets/arial.ttf"))
        return EXIT_FAILURE;

    int highScore = 2;

    sf::Text rankingText("RANKING", font, 30);
    rankingText.setPosition(10, 10);
    sf::Text highScoreText("HIGH SCORE: " + std::to_string(highScore), font, 30);
    highScoreText.setPosition(10, 50);

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
                if (event.key.code == sf::Keyboard::D)
                    angle++;
                if (event.key.code == sf::Keyboard::A)
                    angle--;
                if (event.key.code == sf::Keyboard::W)
                    posY--;
                if (event.key.code == sf::Keyboard::S)
                    posY++;

                sprite.setPosition(posX, posY);
                sprite.setRotation(angle);
            }
        }

        // Clear screen
        window.clear(bg);
        // Render
        /*
        window.draw(sprite);
        window.draw(rankingText);
        window.draw(highScoreText);*/
        world.handleInput();
        world.update();
        world.render(window);

        // Update the window
        window.display();
    }

    return 0;
}