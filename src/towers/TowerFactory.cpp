// TowerFactory.cpp
#include "TowerFactory.h"
#include "ArcherTower.h"
#include "MageTower.h"
#include "ArtilleryTower.h"

std::unique_ptr<Tower> TowerFactory::createTower(TowerType type, sf::Vector2f position, const sf::Texture& texture) {
    switch (type) {
        case TowerType::Archer:
            return std::make_unique<ArcherTower>(position, texture);
        case TowerType::Mage:
            return std::make_unique<MageTower>(position, texture);
        case TowerType::Artillery:
            return std::make_unique<ArtilleryTower>(position, texture);
        default:
            return nullptr;
    }
}
