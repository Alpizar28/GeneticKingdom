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

void GameManager::addTowerAtPosition(sf::Vector2f position) {
    if (gold >= TOWER_COST) {
        towers.emplace_back(position, TOWER_RANGE, TOWER_DAMAGE);
        gold -= TOWER_COST;
    }
}

void GameManager::updateTowerTargeting() {
    // Convertimos enemies a vector<Enemy*> para compatibilidad
    std::vector<Enemy*> enemyPtrs;
    for (auto& enemy : enemies) {
        enemyPtrs.push_back(enemy.get());
    }
    
    for (auto& tower : towers) {
        tower.update(enemyPtrs);
    }
}

void GameManager::drawTowers() {
    for (auto& tower : towers) {
        tower.draw(window);
    }
}
void GameManager::handleInput() {
    sf::Event e;
    while (window.pollEvent(e)) {
        InputState st = input.processEvent(e, showTutorial);
        if (st.requestExit) window.close();
        if (st.toggleDebug) debugMode = !debugMode;
        if (st.advanceTutorial) showTutorial = false;

        if (!showTutorial && e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(e.mouseButton.x, e.mouseButton.y);
            
            // Primero verificar botones de UI
            if (ui.getPauseBtnBounds().contains(mousePos)) {
                paused = !paused;
            }
            else if (ui.getRestartBtnBounds().contains(mousePos)) {
                resetGame();
            }
            else if (ui.getExitBtnBounds().contains(mousePos)) {
                window.close();
            }
            // Luego verificar colocación de torres
            else if (map.isValidTowerPosition(mousePos)) {
                addTowerAtPosition(mousePos);
            }
        }
    }
}


void GameManager::updateLogic(float dt) {
    if (showTutorial || paused) return;

    updateTowerTargeting();  // Añade esta línea

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
    drawTowers();  // Añade esta línea antes de dibujar enemigos
    for (auto& e : enemies)         // enemigos
        e->draw(window);

    ui.render(window, s);           // UI completa (sidebar, HUD, botones, tutorial, debug)
    window.display();
}