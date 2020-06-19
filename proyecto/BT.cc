#include "BT.h"

#include <ctime>

#include "GameWorld.h"
#include "GameObject.h"

static bool Sleep(const unsigned long &Time)
{
    clock_t Tick = clock_t(float(clock()) / float(CLOCKS_PER_SEC) * 1000.f);
    if (Tick < 0) // if clock() fails, it returns -1
        return 0;
    clock_t Now = clock_t(float(clock()) / float(CLOCKS_PER_SEC) * 1000.f);
    if (Now < 0)
        return 0;
    while ((Now - Tick) < Time)
    {
        Now = clock_t(float(clock()) / float(CLOCKS_PER_SEC) * 1000.f);
        if (Now < 0)
            return 0;
    }
    return 1;
}

// --------------------------------------------------

void BTServer::start()
{
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
                std::cout << "CLIENTS CONNECTED: " << clients.size() << "\n";

                // enviar mensaje de accept con index
                BTMessage acceptMessage;
                acceptMessage.type = BTMessage::ACCEPT;

                // crear tanque del jugador y asignar index
                acceptMessage.index = world->createPlayer(message.nick, *window);
                if (acceptMessage.index != -1)
                    socket.send(acceptMessage, *clients[clients.size() - 1]);

                // launch the client thread
                clientThreads.push_back(new sf::Thread(&BTServer::do_messages, this));
                clientThreads[clientThreads.size() - 1]->launch();
            }

            break;
        }
        case BTMessage::LOGOUT:
        {
            std::cout << "LOGOUT " << *client << "\n";

            // buscar si existe el socket client en el vector clients
            for (auto it = clients.begin(); it != clients.end(); ++it)
            {
                if (*(*it) == *client)
                {
                    delete *it;
                    clients.erase(it);

                    world->removePlayer(message.index);
                    break;
                }
            }

            std::cout << "CLIENTS CONNECTED: " << clients.size() << "\n";
            break;
        }
        case BTMessage::INPUT:
        {
            // save the input message into the queue
            m.lock(); //mutex lock
            if (message.message.size() > 0)
                inputMessages.push(message);
            m.unlock(); //mutex unlock

            break;
        }
        default:
            break;
        }
    }
}

void BTServer::simulate()
{
    elapsedTime = clock->restart();
    while (true)
    {
        elapsedTime = clock->restart();
        float deltaTime = tickRate;

        // UPDATE WORLD
        // sacar de la cola de inputMessages y
        // transcribir el texto del mensaje a input
        m.lock(); //mutex lock
        while (inputMessages.size() > 0)
        {
            world->processInput(inputMessages.front());
            inputMessages.pop();
        }
        m.unlock(); //mutex unlock

        // update
        world->update(*window, deltaTime / 1000);

        // render
        window->clear(*bg);
        world->render(*window);
        window->display();

        // SEND WORLD TO ALL THE CLIENTS
        for (auto it = clients.begin(); it != clients.end(); ++it)
            socket.send(*world, *(*it));

        // FORCE TICK RATE
        float elapsed = elapsedTime.asMilliseconds();
        if (tickRate > elapsed)
            Sleep(tickRate - elapsed);
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
    em.index = index;

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
            // ARTIFICIAL LAG
            //Sleep(100);

            // SEND INPUT MESSAGES TO SERVER
            if (socket.send(inputMessage, socket) < 0)
                printf("ERROR: client send input\n");
        }
    }

    logout();
}

void BTClient::net_thread()
{
    while (window->isOpen())
    {
        elapsedTime = clock->restart();

        // RECEIVE WORLD
        socket.recv(*world);

        // RENDER WORLD
        window->clear(*bg);
        world->render(*window);
        window->display();

        // FORCE TICK RATE
        /*float deltaTime = elapsedTime.asMilliseconds();
        if (tickRate > deltaTime)
            Sleep(tickRate - deltaTime);*/
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