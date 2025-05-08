#pragma once
#include "Tower.h"

class ArtilleryTower : public Tower {
public:
    ArtilleryTower(sf::Vector2f pos, const sf::Texture& tex);

    DamageType getDamageType() const override { return DamageType::Artillery; }

protected:
    void attack(Enemy* target) override;
};
