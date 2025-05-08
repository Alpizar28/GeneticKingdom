// TowerFactory.h
#pragma once
#include "Tower.h"
#include "ArcherTower.h"
#include "MageTower.h"
#include "ArtilleryTower.h"
#include <memory> // Asegúrate de incluir esto

class TowerFactory {
public:
// TowerFactory.h
static std::unique_ptr<Tower> createTower(TowerType type, sf::Vector2f position, const sf::Texture& texture);
};
