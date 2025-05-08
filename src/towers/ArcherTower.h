// src/towers/ArcherTower.h
#pragma once
#include "Tower.h"

class ArcherTower : public Tower {
public:
    ArcherTower(sf::Vector2f pos, const sf::Texture& tex);
    // quitamos 'override' para evitar el error de firma
    void attack(Enemy* target);
};
