#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

enum TileType {
    EMPTY,
    PATH_CHOC,
    PATH_GRAY,
    TREE
};

struct Tile {
    TileType type;
    sf::Sprite baseSprite;
    sf::Sprite overlaySprite; // árbol, si aplica
};

class Map {
public:
    static const int TILE_SIZE = 40;
    static const int COLS = 26;
    static const int ROWS = 14;

    Map();
    void draw(sf::RenderWindow& window);

private:
    std::vector<std::vector<Tile>> grid;

    sf::Texture grassTex;
    sf::Texture dirtChocoTex;
    sf::Texture dirtGrayTex;
    sf::Texture treeTex[4]; // 4 árboles distintos

    void generateVisualMap(); // reemplazo de generateRandomPath()
};
