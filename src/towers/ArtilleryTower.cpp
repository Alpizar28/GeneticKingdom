// src/towers/ArtilleryTower.cpp
#include "ArtilleryTower.h"
#include <cstdlib>
#include <cmath>

ArtilleryTower::ArtilleryTower(sf::Vector2f pos, const sf::Texture& tex)
    : Tower(tex, pos, 300.f, 50)  // range=300, damage=50
{
    cooldown       = 3.0f;    // 3s entre disparos
    specialChance  = 0.1f;    // 10% de splash
    cost           = 300;
}

void ArtilleryTower::attack(Enemy* target) {
    Tower::attack(target);
    if (target && (rand() / static_cast<float>(RAND_MAX)) < specialChance) {
        // TODO: lógica de splash area
    }
}
