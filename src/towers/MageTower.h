#pragma once
#include "Tower.h"

class MageTower : public Tower {
public:
    MageTower(sf::Vector2f pos, const sf::Texture& tex);

    DamageType getDamageType() const override { return DamageType::Magic; }

protected:
    void attack(Enemy* target) override;
};
