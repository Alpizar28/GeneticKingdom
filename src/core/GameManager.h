// src/core/GameManager.h
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include "InputManager.h"
#include "WaveManager.h"
#include "UIManager.h"
#include "../map/Map.h"
#include "../enemies/Enemy.h"
#include "../towers/Tower.h"
#include "../towers/Projectile.h"
#include "Constantes.h"

class GameManager {
public:
    enum class BuildMode { NONE, ARCHER, MAGE, ARTILLERY };

    GameManager();
    void run();
    void cancelBuilding();
    void placeTower(sf::Vector2f position);

private:
    void handleInput();
    void handleMouseClick(sf::Vector2f mousePos);
    void updateGameLogic(float dt);
    void renderFrame();
    void resetGame();

    sf::RenderWindow           window;
    sf::Clock                  deltaClock;
    sf::Font                   font;

    sf::SoundBuffer            shotBuffer;
    sf::Sound                  shotSound;

    InputManager               input;
    WaveManager                waves;
    UIManager                  ui;
    Map                        map;

    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Tower>> towers;
    std::vector<Projectile>             projectiles;

    sf::Texture                archerTexture;
    sf::Texture                mageTexture;
    sf::Texture                artilleryTexture;
    sf::Texture                projectileTex;
    sf::Texture                backgroundTex;
    sf::Sprite                 backgroundSprite;

    BuildMode                  buildMode        = BuildMode::NONE;
    sf::Vector2f               buildPosition;
    bool                       paused           = false;
    bool                       debugMode        = false;
    bool                       showTutorial     = true;
    int                        gold             = 100;

    int                        selectedTowerIndex = -1;  // índice de torre seleccionada
};
