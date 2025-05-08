// src/towers/ArcherTower.cpp
#include "ArcherTower.h"

ArcherTower::ArcherTower(sf::Vector2f pos, const sf::Texture& tex)
    : Tower(tex, pos, 200.f, 15)  // range=200, damage=15
{
    cooldown       = 1.0f;   // 1s entre disparos
    specialChance  = 0.0f;   // sin efecto especial
    cost           = 150;
}

void ArcherTower::attack(Enemy* target) {
    Tower::attack(target);
}
