#pragma once
#include <SFML/Graphics.hpp>
#include "map/Map.h"
#include "../enemies/EnemySanta.h"

class GameManager {
public:
    GameManager();
    void run();
    EnemySanta santa;
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text title;
    sf::RectangleShape rightPanel;
    // Botón Start Wave
    sf::RectangleShape startWaveButton;
    sf::Text startWaveText;
    sf::Texture sidebarTex;
    sf::Sprite sidebarSprite;
    sf::RectangleShape sidebarOverlay;
    
    Map map;

    // HUD
    sf::Text goldText;
    sf::Text waveText;
    sf::Text enemiesText;
    int gold = 100;
    int wave = 1;
    int totalWaves = 10;
    int remainingEnemies = 5;

    sf::Texture backgroundTex;
    sf::Sprite backgroundSprite;
    
    sf::Clock deltaClock;

    void handleEvents();
    void update();
    void render();
};
