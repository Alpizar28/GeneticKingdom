#pragma once

#include <SFML/Graphics.hpp>
#include "../map/Map.h"                  // ← ruta relativa correcta
#include "../ai/GeneticAlgorithm.h"
#include "../enemies/Enemy.h"
#include "../ai/Pathfinding.h"

class GameManager {
public:
    GameManager();
    void run();

private:
    void handleEvents();
    void update();
    void render();

    sf::RenderWindow window;
    sf::Font font;
    sf::Text title;

    // Layout/UI
    sf::Texture sidebarTex;
    sf::Sprite  sidebarSprite;
    sf::RectangleShape sidebarOverlay;
    sf::Texture backgroundTex;
    sf::Sprite  backgroundSprite;

    sf::RectangleShape startWaveButton;
    sf::Text            startWaveText;
    sf::Text            goldText;
    sf::Text            waveText;
    sf::Text            enemiesText;

    Map map;                             // ← aquí el miembro que faltaba

    GeneticAlgorithm ga;
    std::vector<std::unique_ptr<Enemy>> enemies;
    bool waveActive = false;

    int gold        = 100;
    int wave        = 1;
    int totalWaves  = 10;

    sf::Clock deltaClock;
};
