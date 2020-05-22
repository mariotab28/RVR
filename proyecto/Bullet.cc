#include "Bullet.h"

#include <SFML/Graphics.hpp>

Bullet::Bullet(GameWorld* world) : GameObject(world, 1)
{
}

Bullet::~Bullet()
{
}

void Bullet::to_bin()
{
}

int Bullet::from_bin(char *data)
{
    return 0;
}
