#include "ArcherTower.h"
#include <iostream>

ArcherTower::ArcherTower(sf::Vector2f pos, const sf::Texture& tex)
    : Tower(tex, pos, 200.f, 15)        // alto alcance, daño bajo
{
    cooldown = 1.0f;
    cost     = 100;
    specialChance = 0.f;
}

void ArcherTower::attack(Enemy* target) {

    Tower::attack(target);

    // Ataque especial: "disparo preciso"
    if (target && (rand() / float(RAND_MAX)) < specialChance) {
        std::cout << "[ArcherTower] ¡Disparo preciso activado!\n";
        // Daño verdadero (ignora defensas)
        target->applyDamage(damage * 0.5f, DamageType::Arrow);

    }
}
