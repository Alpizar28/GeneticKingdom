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
    GameManager();
    void run();

private:
    // Fases del loop principal
    void handleInput();
    void updateLogic(float dt);
    void renderFrame();

    // Helpers
    void resetGame();
    void addTowerAtPosition(sf::Vector2f position);
    void drawTowers();

    // SFML básicos
    sf::RenderWindow   window;
    sf::Clock          deltaClock;
    sf::Font           font;

    // Audio
    sf::SoundBuffer    shotBuffer;
    sf::Sound          shotSound;

    // Sub-sistemas
    InputManager       input;
    WaveManager        waves;
    UIManager          ui;

    // Mundo
    Map                                   map;
    std::vector<std::unique_ptr<Enemy>>   enemies;
    std::vector<Tower>                    towers;

    // Proyectiles
    sf::Texture                   towerTexture;
    sf::Texture                   projectileTex;
    std::vector<Projectile>       projectiles;

    // Fondo
    sf::Texture   backgroundTex;
    sf::Sprite    backgroundSprite;

    // Estado de UI / Juego
    bool          paused       = false;
    bool          debugMode    = false;
    bool          showTutorial = true;
    int           gold         = 100;
};
