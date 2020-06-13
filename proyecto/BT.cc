#include "BT.h"

#include <SFML/Graphics.hpp>
#include "GameWorld.h"
#include "BTMessage.h"
#include "GameObject.h"

void BTServer::start()
{
    // ----WINDOW CREATION-----
    /*window = new sf::RenderWindow(sf::VideoMode(800, 600), "Window title");
    window->setTitle("Bouncy Tanks - SERVER");
    // background color
    bg = new sf::Color(180, 180, 180); //grey*/

    // ----LOAD RESOURCES-----

    world = new GameWorld();

    printf("world created\n");

    world->loadTexture("assets/tankBase.png");
    world->loadTexture("assets/tankGun.png");
    world->loadTexture("assets/bullet.png");
    world->loadTexture("assets/gear.png");

    world->loadFont("assets/arial.ttf");

    // ----INIT WORLD (READ MAP)---

    clock = new sf::Clock();

    world->init();

    printf("world initialized\n");
}

void BTServer::do_messages()
{
    while (true)
    {
        //Recibir Mensajes en y en función del tipo de mensaje
        // - LOGIN: Añadir al vector clients
        // - LOGOUT: Eliminar del vector clients
        // - MESSAGE: Reenviar el mensaje a todos los clientes (menos el emisor)

        //std::cout << "domessages\n";

        Socket *client = &socket;
        BTMessage message;

        if (socket.recv(message, client) == -1)
        {
            std::cout << "ERROR recv -1\n";
        }
        //std::to_string(message.type);
        //std::cout << "type: " << std::to_string(message.type) << "\n";
        //std::cout << *client << "\n";
        switch (message.type)
        {
        case BTMessage::LOGIN:
        {
            std::cout << "LOGIN " << *client << "\n";
            //std::cout << "LOGIN\n";
            if (client == nullptr)
            {
                printf("ERROR: Trying to login a nullptr client\n");
            }
            else
            {
                // añadir el client al vector clients
                clients.push_back(client);
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
                    break;
                }
            }

            break;
        }
        case BTMessage::OBJECT:
        {
            // comprobar que el client este logeado (i.e. este en el vector
            // clients)
            /*std::cout << "MESSAGE " << *client << "\n";
            for (auto it = clients.begin(); it != clients.end(); ++it)
            {
                if (!(*(*it) == *client))
                {
                    std::cout << "ENVIANDO A " << *(*it) << "\n";
                    socket.send(message, *(*it));
                }
            }*/

            break;
        }
        default:
            break;
        }

        std::cout << "CLIENTS CONNECTED: " << clients.size() << "\n";
        //std::cout << "TYPE: " << (int) message.type << " NICK: "<<  message.nick  << " MESSAGE: " << message.message << "\n";
    }
}

void BTServer::simulate()
{
    while (true)
    {
        sf::Time elapsedTime = clock->getElapsedTime();
        float deltaTime = elapsedTime.asMilliseconds();

        if (deltaTime > 33.33f)
        {
            time = 0;

            // UPDATE WORLD
            //world->update(*window);
            world->getGameObjects()[0]->setRotation(
                world->getGameObjects()[0]->getRotation() + 0.1f);

            /*world->getGameObjects()[0]->setPosition(
            world->getGameObjects()[0]->getX()+0.1f,
            world->getGameObjects()[0]->getY()
        );*/

            // Clear screen
            /*window->clear(*bg);

            // Render
            //world->update(*window);
            world->render(*window);

            // Update the window
            window->display();*/

            //printf("%d\n")

            // SEND ALL WORLD
            //BTMessage msg;
            //msg.type = BTMessage::OBJECT;
            //std::cout << "serializate" << "\n";

            //msg.setData(world->serializate());

            //std::cout << "size: " << msg.size() << "\n";

            //std::cout << "serializate finished" << "\n";

            //std::cout << "MESSAGE \n";
            for (auto it = clients.begin(); it != clients.end(); ++it)
            {
                //std::cout << "ENVIANDO A " << *(*it) << "\n";
                socket.send(*world, *(*it));
            }
        }
        else
        {
            time += deltaTime;
        }
    }
}

// --------------------------------------------------------------------

void BTClient::start()
{
    // ----WINDOW CREATION-----
    window = new sf::RenderWindow(sf::VideoMode(800, 600, 16), "Window title");
    window->setTitle("Bouncy Tanks - CLIENT");
    // background color
    bg = new sf::Color(180, 180, 180); //grey

    window->setActive(false);

    // ----LOAD RESOURCES-----

    world = new GameWorld();

    printf("world created\n");

    world->loadTexture("assets/tankBase.png");
    world->loadTexture("assets/tankGun.png");
    world->loadTexture("assets/bullet.png");
    world->loadTexture("assets/gear.png");

    world->loadFont("assets/arial.ttf");

    // ----INIT WORLD (READ MAP)---

    world->clientInit();

    printf("clientInit\n");
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
    printf("main_thread\n");

    while (window->isOpen())
    {
        // HANDLE INPUT
        //printf("a\n");
        //world->handleInput(*window);

        // SEND INPUT MESSAGES TO SERVER

        //printf("a\n");

        // Leer stdin con std::getline
        // Enviar al servidor usando socket
        /*std::string msg;
        std::getline(std::cin, msg);

        // Si el cliente pulsa 'q' cierra su conexión
        if (msg == "q")
            break;*/

        /*BTMessage em(nick);
        em.type = BTMessage::OBJECT;

        socket.send(em, socket);*/
    }

    logout();
}

void BTClient::net_thread()
{
    printf("net_thread\n");

    while (window->isOpen())
    {
        // RECEIVE WORLD

        //printf("aaaaaa\n");

        //Recibir Mensajes de red
        //Mostrar en pantalla el mensaje de la forma "nick: mensaje"
        //BTMessage msg;
        socket.recv(*world);

        //printf("deserializate\n");

        //world->from_bin(msg.data());

        //printf("a\n");
        // RENDER WORLD

        // Clear screen
        window->clear(*bg);

        // Render
        //world->update(*window);
        world->render(*window);

        // Update the window
        window->display();
    }

    // TODO: METER ESTO EN METODO END() -------

    /*delete bg;
    delete window;
    delete world;*/
}

/*void BTClient::render_thread()
{
    while (window->isOpen())
    {
        // Clear screen
        window->clear(*bg);

        // Render
        //world->update(*window);
        world->render(*window);

        // Update the window
        window->display();
    }

    // TODO: METER ESTO EN METODO END() -------

    delete bg;
    delete window;
    delete world;

    // ----------------------------------------
}*/