// src/towers/MageTower.h
#pragma once
#include "Tower.h"

class MageTower : public Tower {
public:
    MageTower(sf::Vector2f pos, const sf::Texture& tex);
    // quitamos 'override'
    void attack(Enemy* target);
};
