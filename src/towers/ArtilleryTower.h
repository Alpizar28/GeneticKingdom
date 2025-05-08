// src/towers/ArtilleryTower.h
#pragma once
#include "Tower.h"

class ArtilleryTower : public Tower {
public:
    ArtilleryTower(sf::Vector2f pos, const sf::Texture& tex);
    // quitamos 'override'
    void attack(Enemy* target);
};
