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

    // Input handler
    InputManager input;

    // Vista y temporización
    sf::RenderWindow window;
    sf::Clock        deltaClock;

    // Estado de juego
    bool paused       = false;
    bool debugMode    = false;
    bool showTutorial = true;

    // Game-over
    bool    gameOver    = false;
    float   gameOverT   = 0.f;
    static constexpr float GAMEOVER_TIME = 5.f;

    int gold             = 100;
    int totalKills       = 0;        

    // Mundo y oleadas
    Map         map;
    WaveManager waves;

    // Construcción de torres
    enum class BuildMode buildMode        = BuildMode::NONE;
    int                  selectedTowerIndex = -1;
    sf::Vector2f         buildPosition;

    // Entidades
    std::vector<std::unique_ptr<Enemy>>  enemies;
    std::vector<std::unique_ptr<Tower>>  towers;
    std::vector<Projectile>              projectiles;

    // Hover
    int hoveredTowerIndex = -1;

    // UI
    sf::Font    font;
    UIManager   ui;

    // Recursos gráficos y sonoros
    sf::Texture archerTexture, mageTexture, artilleryTexture;
    sf::Texture projectileTex, backgroundTex;
    sf::Sprite  backgroundSprite;
    sf::SoundBuffer shotBuffer;
    sf::Sound       shotSound;
};
