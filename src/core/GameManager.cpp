#include "GameManager.h"
#include <iostream>

GameManager::GameManager()
  : window(
      sf::VideoMode(GAME_WIDTH, GAME_HEIGHT),
      "Genetic Kingdom",
      sf::Style::Titlebar | sf::Style::Close  
    ),
    input(),
    waves(20, 5, 0.1f, 10, 30.f),
    ui(font)
{
    window.setFramerateLimit(TARGET_FPS);

    if (!font.loadFromFile(FONT_PATH)) {
        std::cerr << "❌ Error cargando fuente: " << FONT_PATH << "\n";
    }

    // Carga fondo
    if (backgroundTex.loadFromFile("assets/images/game_background.png")) {
        backgroundSprite.setTexture(backgroundTex);
        backgroundSprite.setScale(
            float(GAME_WIDTH)  / backgroundTex.getSize().x,
            float(GAME_HEIGHT) / backgroundTex.getSize().y
        );
    }

    resetGame();
}

void GameManager::resetGame() {
    paused       = false;
    debugMode    = false;
    showTutorial = true;
    gold         = 100;
    waves        = WaveManager(20, 5, 0.1f, 10, 30.f);
    enemies.clear();
}

void GameManager::run() {
    while (window.isOpen()) {
        handleInput();

        float dt = deltaClock.restart().asSeconds();
        updateLogic(dt);

        renderFrame();
    }
}

void GameManager::handleInput() {
    sf::Event e;
    while (window.pollEvent(e)) {
        // 1) Teclado / cerrar
        InputState st = input.processEvent(e, showTutorial);
        if (st.requestExit)     window.close();
        if (st.toggleDebug)     debugMode    = !debugMode;
        if (st.advanceTutorial) showTutorial = false;

        // 2) Clicks de ratón (solo fuera de tutorial)
        if (!showTutorial
            && e.type == sf::Event::MouseButtonPressed
            && e.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f m(e.mouseButton.x, e.mouseButton.y);

            if (ui.getPauseBtnBounds().contains(m)) {
                paused = !paused;
            }
            else if (ui.getRestartBtnBounds().contains(m)) {
                resetGame();
            }
            else if (ui.getExitBtnBounds().contains(m)) {
                window.close();
            }
        }
    }
}


void GameManager::updateLogic(float dt) {
    if (showTutorial || paused) return;

    // 1) Oleadas / GA
    waves.update(dt, map, enemies);

    // 2) Actualizar enemigos vivos
    for (auto it = enemies.begin(); it != enemies.end();) {
        (*it)->update(dt);
        if ((*it)->isFinished()) it = enemies.erase(it);
        else ++it;
    }
}

void GameManager::renderFrame() {
    // Construir estado UI
    UIState s;
    s.paused        = paused;
    s.debugMode     = debugMode;
    s.showTutorial  = showTutorial;
    s.gold          = gold;
    s.currentWave   = waves.getCurrentWave();
    s.totalWaves    = waves.getTotalWaves();
    s.enemiesCount  = int(enemies.size());
    s.timeToNext    = waves.getTimeToNext();
    s.currentGen    = waves.getCurrentGen();
    s.avgFitness    = waves.getAverageFitness();

    // Dibujado
    window.clear();
    window.draw(backgroundSprite);  // fondo
    map.draw(window);               // mapa
    for (auto& e : enemies)         // enemigos
        e->draw(window);

    ui.render(window, s);           // UI completa (sidebar, HUD, botones, tutorial, debug)
    window.display();
}
