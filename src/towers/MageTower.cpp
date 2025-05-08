// src/towers/MageTower.cpp
#include "MageTower.h"
#include <cstdlib>

MageTower::MageTower(sf::Vector2f pos, const sf::Texture& tex)
    : Tower(tex, pos, 150.f, 10)  // range=150, damage=10
{
    cooldown       = 2.0f;    // 2s entre disparos
    specialChance  = 0.2f;    // 20% de efecto especial
    cost           = 200;
}

void MageTower::attack(Enemy* target) {
    Tower::attack(target);
    if (target && (rand() / static_cast<float>(RAND_MAX)) < specialChance) {
        // TODO: efecto mágico adicional
    }
}
