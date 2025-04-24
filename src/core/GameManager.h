#pragma once

#include <SFML/Graphics.hpp>
#include "InputManager.h"
#include "WaveManager.h"
#include "UIManager.h"
#include "../map/Map.h"
#include "../enemies/Enemy.h"
#include "Constantes.h"

class GameManager {
public:
    GameManager();
    void run();

private:
    void resetGame();
    void handleInput();
    void updateLogic(float dt);
    void renderFrame();

    // SFML essentials
    sf::RenderWindow                  window;
    sf::Clock                         deltaClock;
    sf::Font                          font;

    // Sub‐sistemas
    InputManager                      input;
    WaveManager                       waves;
    UIManager                         ui;

    // Mundo
    Map                               map;
    std::vector<std::unique_ptr<Enemy>> enemies;

    // Fondo
    sf::Texture                       backgroundTex;
    sf::Sprite                        backgroundSprite;

    // Estado
    bool                              paused       = false;
    bool                              debugMode    = false;
    bool                              showTutorial = true;
    int                               gold         = 100;
};
