//#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "GameWorld.h"

int main()
{
    // ----WINDOW CREATION-----
    sf::RenderWindow window(sf::VideoMode(800, 600), "Window title");
    window.setTitle("Bouncy Tanks - SERVER");
    // background color
    sf::Color bg(180, 180, 180); //grey

    printf("going to world\n");

    // ----LOAD RESOURCES-----
    GameWorld world;

    printf("world created\n");

    world.loadTexture("assets/tankBase.png");
    world.loadTexture("assets/tankGun.png");
    world.loadTexture("assets/bullet.png");
    world.loadTexture("assets/gear.png");


    world.loadFont("assets/arial.ttf");

    // ----INIT WORLD (READ MAP)---

    world.init();

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
        // Clear screen
        window.clear(bg);

        // Render
        world.handleInput(window);
        world.update(window);
        world.render(window);

        // Update the window
        window.display();
    }

    return 0;
}