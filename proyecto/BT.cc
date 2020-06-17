#include "BT.h"

#include "GameWorld.h"
#include "BTMessage.h"
#include "GameObject.h"

void BTServer::start()
{
    time = 0;
    // ----WINDOW CREATION-----
    window = new sf::RenderWindow(sf::VideoMode(800, 600), "Window title");
    window->setTitle("Bouncy Tanks - SERVER");
    bg = new sf::Color(180, 180, 180); //grey

    window->setVisible(false);

    // ----LOAD RESOURCES-----
    world = new GameWorld();
    world->loadTexture("assets/tankBase.png");
    world->loadTexture("assets/tankGun.png");
    world->loadTexture("assets/bullet.png");
    world->loadTexture("assets/gear.png");
    world->loadTexture("assets/wall.png");

    world->loadFont("assets/arial.ttf");

    // ----INIT WORLD---
    clock = new sf::Clock();
    world->createObjects();
    world->init(*window, level);
}

void BTServer::do_messages()
{
    while (true)
    {
        Socket *client = &socket;
        BTMessage message;

        if (socket.recv(message, client) == -1)
            std::cout << "ERROR recv -1\n";

        switch (message.type)
        {
        case BTMessage::LOGIN:
        {
            std::cout << "LOGIN " << *client << "\n";

            if (client == nullptr)
                printf("ERROR: Trying to login a nullptr client\n");
            else
            {
                // añadir el client al vector clients
                clients.push_back(client);

                // enviar mensaje de accept con index
                BTMessage acceptMessage;
                acceptMessage.type = BTMessage::ACCEPT;

                // crear tanque del jugador y asignar index
                acceptMessage.index = world->createPlayer(message.nick, *window);
                if (acceptMessage.index != -1)
                    socket.send(acceptMessage, *clients[clients.size() - 1]);
            }

            std::cout << "CLIENTS CONNECTED: " << clients.size() << "\n";
            break;
        }
        case BTMessage::LOGOUT:
        {
            std::cout << "LOGOUT " << *client << "\n";

            int pos = 0;
            // buscar si existe el socket client en el vector clients
            for (auto it = clients.begin(); it != clients.end(); ++it)
            {
                if (*(*it) == *client)
                {
                    delete *it;
                    clients.erase(it);

                    world->removePlayer(pos);
                    break;
                }
                pos++;
            }

            std::cout << "CLIENTS CONNECTED: " << clients.size() << "\n";
            break;
        }
        case BTMessage::INPUT:
        {
            // TODO: CREAR UN HILO DEDICADO A CADA CLIENTE??????

            // transcribir el texto del mensaje a input
            if (message.message.size() > 0)
                world->processInput(message);

            break;
        }
        default:
            break;
        }
    }
}

void BTServer::simulate()
{
    while (true)
    {
        elapsedTime = clock->restart();
        //deltaTime = elapsedTime.asMilliseconds();

        //printf("time: %f deltaTime: %f\n", time, deltaTime);

        //if (time > 33.33f)
        //{
        //time = 0;

        // UPDATE WORLD
        // update
        world->update(*window, elapsedTime);

        // render
        window->clear(*bg);
        world->render(*window);
        window->display();

        // SEND WORLD TO ALL THE CLIENTS
        for (auto it = clients.begin(); it != clients.end(); ++it)
            socket.send(*world, *(*it));
    }
}

// --------------------------------------------------------------------
// --------------------------------------------------------------------

void BTClient::start()
{
    // ----WINDOW CREATION-----
    window = new sf::RenderWindow(sf::VideoMode(800, 600, 16), "Window title");
    window->setTitle("Bouncy Tanks - CLIENT");
    bg = new sf::Color(180, 180, 180); //grey

    window->setActive(false);

    // ----LOAD RESOURCES-----
    world = new GameWorld();
    world->loadTexture("assets/tankBase.png");
    world->loadTexture("assets/tankGun.png");
    world->loadTexture("assets/bullet.png");
    world->loadTexture("assets/gear.png");
    world->loadTexture("assets/wall.png");

    world->loadFont("assets/arial.ttf");

    // ----INIT WORLD (READ MAP)---
    world->createObjects();

    clock = new sf::Clock();
    time = 0;
    deltaTime = 0;
}

void BTClient::login()
{
    std::cout << "BTCLIENT::LOGIN\n";

    BTMessage em(nick);
    em.type = BTMessage::LOGIN;

    socket.send(em, socket);
}

void BTClient::logout()
{
    std::cout << "BTCLIENT::LOGOUT\n";

    BTMessage em(nick);
    em.type = BTMessage::LOGOUT;

    socket.send(em, socket);
}

void BTClient::input_thread()
{
    while (window->isOpen())
    {
        // HANDLE INPUT
        BTMessage inputMessage(nick);
        inputMessage.type = BTMessage::INPUT;
        inputMessage.index = index;

        if (world->handleInput(*window, inputMessage))
        {
            // SEND INPUT MESSAGES TO SERVER
            if (socket.send(inputMessage, socket) < 0)
                printf("send error!\n");
        }
    }

    logout();
}

void BTClient::net_thread()
{
    while (window->isOpen())
    {
        // RECEIVE WORLD
        socket.recv(*world);

        // RENDER WORLD
        window->clear(*bg);
        world->render(*window);
        window->display();
    }

    // TODO: METER ESTO EN METODO END() -------

    /*delete bg;
    delete window;
    delete world;*/
}

void BTClient::wait()
{
    BTMessage msg;
    socket.recv(msg);

    if (msg.type == BTMessage::ACCEPT)
        index = msg.index;
    else
        wait();
}