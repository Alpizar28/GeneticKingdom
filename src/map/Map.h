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
    sf::Sprite overlaySprite;
};

class Map {
public:
    static const int TILE_SIZE = 40;
    static const int COLS = 26;
    static const int ROWS = 14;

    Map();
    void draw(sf::RenderWindow& window);
    bool isWalkable(const sf::Vector2i& pos) const;
    sf::Vector2i getStartTile() const;
    sf::Vector2i findRightmostPathTile() const;
    sf::Vector2i findLeftmostPathTile() const;


private:
    std::vector<std::vector<Tile>> grid;
    sf::Vector2i startTile;

    sf::Texture grassTex;
    sf::Texture dirtChocoTex;
    sf::Texture dirtGrayTex;
    sf::Texture treeTex[4];

    void generateVisualMap();

};
