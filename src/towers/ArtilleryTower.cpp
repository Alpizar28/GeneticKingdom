#include "ArtilleryTower.h"
#include <cstdlib>
#include <cmath>
#include <iostream>

ArtilleryTower::ArtilleryTower(sf::Vector2f pos, const sf::Texture& tex)
    : Tower(tex, pos, 300.f, 50)        // daño alto, alcance bajo
{
    cooldown       = 3.0f;
    specialChance  = 0.15f;             // 15 % splash
    cost           = 250;
}

void ArtilleryTower::attack(Enemy* target) {
    /* daño directo */
    Tower::attack(target);

    /* splash (radio 60 px) */
    if (!target) return;
    if ((rand() / float(RAND_MAX)) < specialChance) {
        std::cout << "[ArtilleryTower] ¡Daño en área activado!\n";

        sf::Vector2f center = target->getSprite().getPosition();

    }
}
