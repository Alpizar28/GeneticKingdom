#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

enum TileType {
    EMPTY,
    PATH,
    TOWER
};

struct Tile {
    TileType type;
    sf::RectangleShape shape;
};

class Map {
public:
    static const int TILE_SIZE = 40;
    static const int COLS = 26;
    static const int ROWS = 14;


    Map();
    void generateRandomPath();
    void draw(sf::RenderWindow& window);
private:
    std::vector<std::vector<Tile>> grid;
    sf::Color getColor(TileType type);
};