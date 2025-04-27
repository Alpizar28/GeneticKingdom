// src/core/GameManager.cpp

#include "GameManager.h"
#include "towers/Projectile.h"
#include <iostream>

GameManager::GameManager()
  : window(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT), "Genetic Kingdom",
           sf::Style::Titlebar | sf::Style::Close),
    input(), waves(20, 5, 0.1f, 10, 30.f), ui(font)
{
    window.setFramerateLimit(TARGET_FPS);

    if (!font.loadFromFile(FONT_PATH))
        std::cerr << "❌ Error cargando fuente: " << FONT_PATH << "\n";

    if (!towerTexture.loadFromFile("assets/sprites/towers/cannon.png"))
        std::cerr << "❌ Error cargando towerTexture\n";
    towers.reserve(10);

    if (!projectileTex.loadFromFile("assets/sprites/projectiles/bullet.png"))
        std::cerr << "❌ No pude cargar bullet.png\n";

    if (!shotBuffer.loadFromFile("assets/sounds/shoot.wav"))
        std::cerr << "❌ No pude cargar shoot.wav\n";
    shotSound.setBuffer(shotBuffer);

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

    // Reemplaza el gestor de olas
    waves = WaveManager(20, 5, 0.1f, 10, 30.f);

    enemies.clear();
    towers.clear();
    projectiles.clear();
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
        InputState st = input.processEvent(e, showTutorial);
        if (st.requestExit)     window.close();
        if (st.toggleDebug)     debugMode = !debugMode;
        if (st.advanceTutorial) showTutorial = false;

        // Una vez quitado el tutorial, las olas ya arrancan en updateLogic()

        if (!showTutorial
            && e.type == sf::Event::MouseButtonPressed
            && e.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mousePos(e.mouseButton.x, e.mouseButton.y);

            if (ui.getPauseBtnBounds().contains(mousePos)) {
                paused = !paused;
            }
            else if (ui.getRestartBtnBounds().contains(mousePos)) {
                resetGame();
            }
            else if (ui.getExitBtnBounds().contains(mousePos)) {
                window.close();
            }
            else if (map.isValidTowerPosition(mousePos)) {
                addTowerAtPosition(mousePos);
            }
        }
    }
}

void GameManager::addTowerAtPosition(sf::Vector2f position) {
    if (gold >= TOWER_COST) {
        towers.emplace_back(towerTexture, position, TOWER_RANGE, TOWER_DAMAGE);
        gold -= TOWER_COST;
    }
}

void GameManager::drawTowers() {
    for (auto& tower : towers)
        tower.draw(window);
}

void GameManager::updateLogic(float dt) {

    if (showTutorial || paused)
        return;

    waves.update(dt, map, enemies);

    // 3) Preparamos punteros a enemigos
    std::vector<Enemy*> enemyPtrs;
    for (auto& e : enemies)
        enemyPtrs.push_back(e.get());

    // 4) Cada torre dispara y crea proyectiles
    for (auto& tower : towers) {
        if (Enemy* target = tower.tryFire(enemyPtrs)) {
            shotSound.play();
            projectiles.emplace_back(
                projectileTex,
                tower.getPosition(),
                target,
                400.f,
                tower.getDamage()
            );
        }
    }

    // 5) Actualizar proyectiles
    for (auto it = projectiles.begin(); it != projectiles.end();) {
        it->update(dt);
        if (!it->isAlive())
            it = projectiles.erase(it);
        else
            ++it;
    }

    // 6) Actualizar enemigos y sumar oro al morir
    for (auto it = enemies.begin(); it != enemies.end();) {
        auto& enemy = *it;
        enemy->update(dt);
        if (enemy->isFinished()) {
            gold += enemy->getRewardGold();
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }
}

void GameManager::renderFrame() {
    UIState s{
        paused,
        debugMode,
        showTutorial,
        gold,
        waves.getCurrentWave(),
        waves.getTotalWaves(),
        static_cast<int>(enemies.size()),
        waves.getTimeToNext(),
        waves.getCurrentGen(),
        waves.getAverageFitness()
    };

    window.clear();
    window.draw(backgroundSprite);
    map.draw(window);

    drawTowers();

    for (auto& e : enemies)
        e->draw(window);

    for (auto& p : projectiles)
        p.draw(window);

    ui.render(window, s);
    window.display();
}
