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
    // background color
    bg = new sf::Color(180, 180, 180); //grey

    window->setVisible(false);

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

    world->createObjects();
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

        //printf("recv\n");

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

                // enviar mensaje de accept con index
                BTMessage acceptMessage;
                acceptMessage.type = BTMessage::ACCEPT;
                // crear tanque del jugador y asignar index
                acceptMessage.index = world->createPlayer(message.nick);
                if (acceptMessage.index != -1)
                    socket.send(acceptMessage, *clients[clients.size() - 1]);
            }

            std::cout << "CLIENTS CONNECTED: " << clients.size() << "\n";

            break;
        }
        case BTMessage::LOGOUT:
        {
            std::cout << "LOGOUT " << *client << "\n";

            // TODO: REORDENAR INDEX Y SETID() DE PLAYERS!!!

            int pos = 0;
            // buscar si existe el socket client en el vector clients
            for (auto it = clients.begin(); it != clients.end(); ++it)
            {
                if (*(*it) == *client)
                {
                    delete *it;
                    clients.erase(it);

                    // destruir tanque del jugador
                    world->removePlayer(pos);

                    break;
                }

                pos++;
            }

            /*for (int i = 0; i < ; i++)
            {
                /* code */

            /*if(pos < clients.size())
            {
                ;
            }*/

            std::cout
                << "CLIENTS CONNECTED: " << clients.size() << "\n";

            break;
        }
        case BTMessage::INPUT:
        {
            // TODO: CREAR UN HILO DEDICADO A CADA CLIENTE??????

            // transcribir el texto del mensaje a input
            if (message.message.size() > 0)
            {
                //printf("message: %s\n", message.message.c_str());
                // hacer handleInput para el jugador indicado en el msg
                //printf("index: %d\n", message.index);

                world->processInput(message);
            }

            // comprobar que el client este logeado (i.e. este en el vector
            // clients)
            //std::cout << "MESSAGE " << *client << "\n";
            /*for (auto it = clients.begin(); it != clients.end(); ++it)
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

        // ver si hay algun input en la cola
        // sacar el input de la cola
        // y evaluar

        world->update(*window, elapsedTime);

        /*world->getGameObjects()[0]->setRotation(
            world->getGameObjects()[0]->getRotation() + 0.1f);

        world->getGameObjects()[0]->setPosition(
            world->getGameObjects()[0]->getX()+0.1f,
            world->getGameObjects()[0]->getY()
        );*/

        // Clear screen
        window->clear(*bg);

        // Render
        //world->update(*window);
        world->render(*window);

        // Update the window
        window->display();

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

        /*}
        else
        {
            time += deltaTime;
        }*/
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

    world->createObjects();

    clock = new sf::Clock();
    time = 0;
    deltaTime = 0;

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

        //elapsedTime = clock->restart();
        //deltaTime = elapsedTime.asMilliseconds();

        //printf("time: %f deltaTime: %f\n", time, deltaTime);

        //if (time > 33.33f)
        //{
        //time = 0;

        //printf("tick\n");

        BTMessage inputMessage(nick);
        inputMessage.type = BTMessage::INPUT;
        inputMessage.index = index;

        //printf("index: %d\n", index);

        if (world->handleInput(*window, inputMessage))
        {
            // SEND INPUT MESSAGES TO SERVER
            if (socket.send(inputMessage, socket) < 0)
                printf("send error!\n");
        }
        /*}
        else
        {
            time += deltaTime;
        }*/
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

void BTClient::wait()
{
    BTMessage msg;
    socket.recv(msg);

    if (msg.type == BTMessage::ACCEPT)
    {
        index = msg.index;
    }
    else
    {
        wait();
    }
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