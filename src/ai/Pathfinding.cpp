#include "Pathfinding.h"
#include <queue>
#include <map>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <tuple>

// Comparador para std::map (para usar Vector2i como clave)
struct Vector2iCompare {
    bool operator()(const sf::Vector2i& a, const sf::Vector2i& b) const {
        return std::tie(a.x, a.y) < std::tie(b.x, b.y);
    }
};

// Comparador para priority_queue basado en menor costo
struct PriorityQueueCompare {
    bool operator()(const std::pair<float, sf::Vector2i>& a, const std::pair<float, sf::Vector2i>& b) const {
        return a.first > b.first;
    }
};

// Heurística Manhattan
float heuristic(const sf::Vector2i& a, const sf::Vector2i& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

// Constructor que recibe el mapa
Pathfinding::Pathfinding(const Map& map) : mapRef(map) {
    std::cout << "🧭 Pathfinding activado" << std::endl;
}

std::vector<sf::Vector2i> Pathfinding::findPath(const sf::Vector2i& start, const sf::Vector2i& end) {
    std::priority_queue<
        std::pair<float, sf::Vector2i>,
        std::vector<std::pair<float, sf::Vector2i>>,
        PriorityQueueCompare
    > frontier;

    frontier.push({0, start});
    std::map<sf::Vector2i, sf::Vector2i, Vector2iCompare> cameFrom;
    std::map<sf::Vector2i, float, Vector2iCompare> costSoFar;

    cameFrom[start] = start;
    costSoFar[start] = 0;

    const std::vector<sf::Vector2i> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };

    while (!frontier.empty()) {
        sf::Vector2i current = frontier.top().second;
        frontier.pop();

        if (current == end) break;

        for (const auto& dir : directions) {
            sf::Vector2i next = current + dir;

            // Usar el mapa para saber si el tile es caminable
            if (!mapRef.isWalkable(next)) continue;

            float newCost = costSoFar[current] + 1;
            if (!costSoFar.count(next) || newCost < costSoFar[next]) {
                costSoFar[next] = newCost;
                float priority = newCost + heuristic(next, end);
                frontier.push({priority, next});
                cameFrom[next] = current;
            }
        }
    }

    std::vector<sf::Vector2i> path;
    sf::Vector2i current = end;

    if (!cameFrom.count(end)) {
        std::cout << "⚠️ No se encontró camino.\n";
        return path;
    }

    while (current != start) {
        path.push_back(current);
        current = cameFrom[current];
    }

    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}
