// Pathfinding.h (completo y actualizado)
#pragma once
#include <SFML/System.hpp>
#include <vector>
#include "map/Map.h"

class Pathfinding {
public:
    Pathfinding(const Map& map); // Recibe referencia al mapa
    std::vector<sf::Vector2i> findPath(const sf::Vector2i& start, const sf::Vector2i& end);

private:
    const Map& mapRef; // Referencia constante al mapa
};
