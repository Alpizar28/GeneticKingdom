#pragma once

// Dimensiones base del juego
constexpr int GAME_WIDTH = 1280;
constexpr int GAME_HEIGHT = 720;

constexpr int WINDOW_WIDTH = 768;
constexpr int WINDOW_HEIGHT = 768;

// Rutas de recursos
constexpr const char* FONT_PATH = "assets/fonts/default.ttf";
// Asegúrate que la ruta es correcta relativa al ejecutable
constexpr const char* ICON_PATH = "assets/icon.png";

constexpr int TOWER_COST = 50;
constexpr float TOWER_RANGE = 150.f;
constexpr int TOWER_DAMAGE = 20;// Añade esto junto a tus otras constantes:


constexpr int ARCHER_COST = 50;
constexpr int MAGE_COST = 100;
constexpr int ARTILLERY_COST = 200;
    
constexpr float ARCHER_RANGE = 200.f;
constexpr int ARCHER_DAMAGE = 10;
constexpr float ARCHER_COOLDOWN = 0.8f;
    
constexpr float MAGE_RANGE = 150.f;
constexpr int MAGE_DAMAGE = 25;
constexpr float MAGE_COOLDOWN = 1.5f;
    
constexpr float ARTILLERY_RANGE = 100.f;
constexpr int ARTILLERY_DAMAGE = 50;
constexpr float ARTILLERY_COOLDOWN = 2.5f;

// FPS del juego
constexpr int TARGET_FPS = 60;