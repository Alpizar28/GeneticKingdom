// src/core/GameManager.cpp

#include "GameManager.h"
#include <iostream>
#include <algorithm>

GameManager::GameManager()
  : window(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT),
           "Genetic Kingdom",
           sf::Style::Titlebar | sf::Style::Close),
    input(),
    waves(5, 1, 0.1f, 20, 20.f),
    ui()
{
    window.setFramerateLimit(TARGET_FPS);

    if (!font.loadFromFile(FONT_PATH))
        std::cerr << "❌ Error cargando fuente\n";
    ui.init(font);

    if (!towerTexture.loadFromFile("assets/sprites/towers/cannon.png"))
        std::cerr << "❌ Error torre\n";
    if (!projectileTex.loadFromFile("assets/sprites/projectiles/bullet.png"))
        std::cerr << "❌ Error bala\n";
    if (!shotBuffer.loadFromFile("assets/sounds/shoot.wav"))
        std::cerr << "❌ Error sonido\n";
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

    waves = WaveManager(5, 1, 0.1f, 20, 20.f);
    enemies.clear();
    towers.clear();
    projectiles.clear();
}

void GameManager::run() {
    while (window.isOpen()) {
        handleInput();

        // Capturamos dt correctamente
        float dt = deltaClock.restart().asSeconds();

        // 1) Torres disparan, proyectiles avanzan y enemigos mueren
        if (!showTutorial && !paused) {
            // a) Torres intentan disparar
            std::vector<Enemy*> ep;
            ep.reserve(enemies.size());
            for (auto& e : enemies) ep.push_back(e.get());
            for (auto& t : towers) {
                if (auto* trg = t.tryFire(ep)) {
                    shotSound.play();
                    projectiles.emplace_back(
                      projectileTex,
                      t.getPosition(),
                      trg,
                      400.f,
                      t.getDamage()
                    );
                }
            }

            // b) Proyectiles
            for (auto it = projectiles.begin(); it != projectiles.end();) {
                it->update(dt);
                if (!it->isAlive())
                    it = projectiles.erase(it);
                else
                    ++it;
            }

            // c) Enemigos: update, asignar fitness, oro y limpieza de proyectiles huérfanos
            for (auto it = enemies.begin(); it != enemies.end();) {
                auto& en = *it;
                en->update(dt);
                if (en->isFinished()) {
                    // Eliminamos proyectiles que aún apunten a este enemigo
                    projectiles.erase(
                      std::remove_if(projectiles.begin(), projectiles.end(),
                        [&](const Projectile& p){
                          return p.getTarget() == en.get();
                        }),
                      projectiles.end()
                    );

                    // Asignar fitness
                    if (auto* ind = waves.getIndividualForEnemy(en.get())) {
                        ind->fitness =
                          en->getWaypointsReached()
                          + (en->getHp() / en->getMaxHp());
                    }

                    if (!en->hasEscaped())
                        gold += en->getRewardGold();

                    it = enemies.erase(it);
                } else {
                    ++it;
                }
            }
        }

        // 2) Generación, spawn y evolución de oleadas usando ¡dt!
        waves.update(dt, map, enemies);

        // 3) Render
        window.clear();
        window.draw(backgroundSprite);
        map.draw(window);
        for (auto& t : towers)      t.draw(window);
        for (auto& e : enemies)     e->draw(window);
        for (auto& p : projectiles) p.draw(window);

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
            waves.getAverageFitness(),
            waves.getBestFitness()
        };
        ui.render(window, s);
        window.display();
    }
}

void GameManager::handleInput() {
    sf::Event e;
    while (window.pollEvent(e)) {
        InputState st = input.processEvent(e, showTutorial);
        if (st.requestExit)     window.close();
        if (st.toggleDebug)     debugMode = !debugMode;
        if (st.advanceTutorial) showTutorial = false;

        if (!showTutorial
            && e.type == sf::Event::MouseButtonPressed
            && e.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mp(e.mouseButton.x, e.mouseButton.y);
            if      (ui.getPauseBtnBounds().contains(mp))   paused = !paused;
            else if (ui.getRestartBtnBounds().contains(mp)) resetGame();
            else if (ui.getExitBtnBounds().contains(mp))    window.close();
            else if (map.isValidTowerPosition(mp)) {
                towers.emplace_back(towerTexture, mp, TOWER_RANGE, TOWER_DAMAGE);
                gold -= TOWER_COST;
            }
        }
    }
}
