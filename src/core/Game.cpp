#include "Game.h"
#include <iostream>

Game::Game() {
    std::cout << "🎮 Juego inicializado" << std::endl;  // ✅ emojis como string
}

void Game::start() {
    std::cout << "▶️ Empezando juego..." << std::endl;
}
