#include "map/Map.h"
#include <cstdlib>
#include <ctime>

const int MAP_OFFSET_X = 10; // espacio lateral izquierdo (para info)
const int MAP_OFFSET_Y = 80;  // espacio arriba (para el título)

Map::Map() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    grid.resize(ROWS, std::vector<Tile>(COLS));

    for (int y = 0; y < ROWS; ++y) {
        for (int x = 0; x < COLS; ++x) {
            grid[y][x].type = EMPTY;
            grid[y][x].shape.setSize(sf::Vector2f(TILE_SIZE - 1, TILE_SIZE - 1)); // 1px de separación
            grid[y][x].shape.setPosition(
                MAP_OFFSET_X + x * TILE_SIZE,
                MAP_OFFSET_Y + y * TILE_SIZE
            );
            grid[y][x].shape.setFillColor(getColor(EMPTY));
        }
    }

    generateRandomPath();
}

void Map::generateRandomPath() {
    int y = std::rand() % ROWS;
    int x = 0;

    while (x < COLS) {
        grid[y][x].type = PATH;
        grid[y][x].shape.setFillColor(getColor(PATH));

        int dir = std::rand() % 3;  // 0 = right, 1 = up, 2 = down

        if (dir == 1 && y > 0 && grid[y - 1][x].type != PATH) {
            --y;
        } else if (dir == 2 && y < ROWS - 1 && grid[y + 1][x].type != PATH) {
            ++y;
        } else {
            ++x;
        }
    }
}

void Map::draw(sf::RenderWindow& window) {
    for (int y = 0; y < ROWS; ++y) {
        for (int x = 0; x < COLS; ++x) {
            window.draw(grid[y][x].shape);
        }
    }
}

sf::Color Map::getColor(TileType type) {
    switch (type) {
        case EMPTY: return sf::Color(40, 40, 40);
        case PATH:  return sf::Color(150, 150, 150);
        case TOWER: return sf::Color::Blue;
        default:    return sf::Color::Black;
    }
}
