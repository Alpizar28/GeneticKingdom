#include "ArcherTower.h"

ArcherTower::ArcherTower(sf::Vector2f pos, const sf::Texture& tex)
    : Tower(tex, pos, 200.f, 15)        // alto alcance, daño bajo
{
    cooldown = 1.0f;
    cost     = 150;
    specialChance = 0.f;
}

void ArcherTower::attack(Enemy* target) {
    /* solo daño base; luego puedes añadir “fuego rápido”, etc. */
    Tower::attack(target);
}
