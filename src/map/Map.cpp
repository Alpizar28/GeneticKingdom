#include "map/Map.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

const int MAP_OFFSET_X = 0;
const int MAP_OFFSET_Y = 80;

Map::Map() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    fs::path basePath = fs::current_path().parent_path();
    std::string spritePath = basePath.string() + "/assets/sprites/landscape/";

    if (!grassTex.loadFromFile(spritePath + "grass.png")) std::cerr << "❌ Falta grass\n";
    if (!dirtChocoTex.loadFromFile(spritePath + "dirt_choco.png")) std::cerr << "❌ Falta dirt_choco\n";
    for (int i = 0; i < 4; ++i) {
        std::string path = spritePath + "tree_0" + std::to_string(i + 1) + ".png";
        if (!treeTex[i].loadFromFile(path)) std::cerr << "❌ Falta " << path << "\n";
    }

    grid.resize(ROWS, std::vector<Tile>(COLS));

    // 1. Inicializar todo como EMPTY
    for (int y = 0; y < ROWS; ++y) {
        for (int x = 0; x < COLS; ++x) {
            grid[y][x].type = EMPTY;
        }
    }

    // 2. Generar camino chocolate continuo
    int y = std::rand() % ROWS;
    int x = 0;
    while (x < COLS) {
        grid[y][x].type = PATH_CHOC;

        int dir = std::rand() % 3;
        if (dir == 1 && y > 0 && grid[y - 1][x].type != PATH_CHOC) {
            --y;
        } else if (dir == 2 && y < ROWS - 1 && grid[y + 1][x].type != PATH_CHOC) {
            ++y;
        } else {
            ++x;
        }
    }

    // 3. Colocar árboles lejos del camino
    for (int y = 0; y < ROWS; ++y) {
        for (int x = 0; x < COLS; ++x) {
            if (grid[y][x].type != EMPTY) continue;

            bool cercaDeCamino = false;
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    int ny = y + dy;
                    int nx = x + dx;
                    if (ny >= 0 && ny < ROWS && nx >= 0 && nx < COLS) {
                        if (grid[ny][nx].type == PATH_CHOC) {
                            cercaDeCamino = true;
                        }
                    }
                }
            }

            if (!cercaDeCamino && std::rand() % 100 < 20) {
                grid[y][x].type = TREE;
            }
        }
    }

    // 4. Asignar sprites por tipo
    for (int y = 0; y < ROWS; ++y) {
        for (int x = 0; x < COLS; ++x) {
            Tile& tile = grid[y][x];

            sf::Texture* baseTexture = &grassTex;
            if (tile.type == PATH_CHOC) baseTexture = &dirtChocoTex;

            tile.baseSprite.setTexture(*baseTexture);
            tile.baseSprite.setPosition(MAP_OFFSET_X + x * TILE_SIZE, MAP_OFFSET_Y + y * TILE_SIZE);
            tile.baseSprite.setScale(
                (float)TILE_SIZE / baseTexture->getSize().x,
                (float)TILE_SIZE / baseTexture->getSize().y
            );

            if (tile.type == TREE) {
                int treeIndex = std::rand() % 4;
                tile.overlaySprite.setTexture(treeTex[treeIndex]);
                tile.overlaySprite.setPosition(tile.baseSprite.getPosition());
                tile.overlaySprite.setScale(
                    (float)TILE_SIZE / treeTex[treeIndex].getSize().x,
                    (float)TILE_SIZE / treeTex[treeIndex].getSize().y
                );
            }
        }
    }
}

void Map::draw(sf::RenderWindow& window) {
    for (int y = 0; y < ROWS; ++y) {
        for (int x = 0; x < COLS; ++x) {
            const Tile& tile = grid[y][x];
            window.draw(tile.baseSprite);
            if (tile.type == TREE)
                window.draw(tile.overlaySprite);
        }
    }
}


sf::Vector2i Map::getStartTile() const {
    return startTile;
}

bool Map::isWalkable(const sf::Vector2i& pos) const {
    if (pos.x < 0 || pos.y < 0 || pos.x >= COLS || pos.y >= ROWS)
        return false;
    return grid[pos.y][pos.x].type == PATH_CHOC;
}

sf::Vector2i Map::findRightmostPathTile() const {
    for (int x = COLS - 1; x >= 0; --x) {
        for (int y = 0; y < ROWS; ++y) {
            if (grid[y][x].type == PATH_CHOC) {
                return {x, y};
            }
        }
    }
    return {0, 0}; // fallback
}
sf::Vector2i Map::findLeftmostPathTile() const {
    for (int x = 0; x < COLS; ++x) {
        for (int y = 0; y < ROWS; ++y) {
            if (grid[y][x].type == PATH_CHOC) {
                return {x, y};
            }
        }
    }
    return {0, 0}; // fallback
}
bool Map::isValidTowerPosition(sf::Vector2f position) const {
    // 1. Convertir coordenadas mundiales a coordenadas de tile
    int tileX = static_cast<int>((position.x - MAP_OFFSET_X) / TILE_SIZE);
    int tileY = static_cast<int>((position.y - MAP_OFFSET_Y) / TILE_SIZE);

    // 2. Verificar que esté dentro de los límites del mapa
    if (tileX < 0 || tileY < 0 || tileX >= COLS || tileY >= ROWS) {
        return false;
    }

    // 3. Obtener el tile correspondiente
    const Tile& tile = grid[tileY][tileX];

    // 4. Reglas de validación:
    // - No puede estar en un camino (PATH_CHOC/PATH_GRAY)
    // - No puede estar en un árbol (TREE)
    // - Debe ser terreno vacío (EMPTY)
    return tile.type == EMPTY;
}
