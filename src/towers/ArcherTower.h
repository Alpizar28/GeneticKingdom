#pragma once
#include "Tower.h"

class ArcherTower : public Tower {
public:
    ArcherTower(sf::Vector2f pos, const sf::Texture& tex);

    /* tipo de daño */
    DamageType getDamageType() const override { return DamageType::Arrow; }

protected:
    void attack(Enemy* target) override;   // por ahora solo daño base
};
