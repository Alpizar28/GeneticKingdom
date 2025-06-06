#include "MageTower.h"
#include <cstdlib>
#include <iostream>

MageTower::MageTower(sf::Vector2f pos, const sf::Texture& tex)
    : Tower(tex, pos, 150.f, 25)        // alcance medio, daño medio
{
    cooldown       = 2.0f;
    specialChance  = 0.25f;             // 25 % de “explosión mágica”
    cost           = 150;
}

void MageTower::attack(Enemy* target) {
    Tower::attack(target);              // daño base

    /* efecto especial: 25 % de extra-damage */
    if (target && (rand() / float(RAND_MAX)) < specialChance) {
        std::cout << "[MageTower] ¡Explosión mágica activada!\n";

        target->applyDamage(damage * 0.5f, DamageType::Magic);
    }
}
