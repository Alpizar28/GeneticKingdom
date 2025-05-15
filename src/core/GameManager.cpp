// src/core/GameManager.cpp

#include "GameManager.h"
#include "../towers/TowerFactory.h"
#include <iostream>
#include <algorithm>
#include <numeric>
#include "InputManager.h"

GameManager::GameManager()
  : window(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT), "Genetic Kingdom",
           sf::Style::Titlebar | sf::Style::Close),
    waves(5, 0.6f, DEFAULT_TOTAL_WAVES, 20.f), ui()
{
    window.setFramerateLimit(TARGET_FPS);
    if (!font.loadFromFile(FONT_PATH))
        std::cerr << "❌ Error cargando fuente\n";
    ui.init(font);

    // Carga texturas y sonidos
    archerTexture.loadFromFile("assets/sprites/towers/arqueros.png");
    mageTexture.loadFromFile("assets/sprites/towers/magos.png");
    artilleryTexture.loadFromFile("assets/sprites/towers/artilleros.png");
    projectileTex.loadFromFile("assets/sprites/projectiles/bullet.png");
    shotBuffer.loadFromFile("assets/sounds/shoot.wav");
    shotSound.setBuffer(shotBuffer);
    if (backgroundTex.loadFromFile("assets/images/game_background.png")) {
        backgroundSprite.setTexture(backgroundTex);
        backgroundSprite.setScale(
            float(GAME_WIDTH) / backgroundTex.getSize().x,
            float(GAME_HEIGHT) / backgroundTex.getSize().y
        );
    }

    resetGame();
}

void GameManager::resetGame() {
    paused             = false;
    debugMode          = false;
    showTutorial       = true;
    gameOver           = false;
    gold               = 300;
    totalKills        = 0;      
    buildMode          = BuildMode::NONE;
    selectedTowerIndex = -1;
    hoveredTowerIndex  = -1;
    waves = WaveManager(5, 0.6f, DEFAULT_TOTAL_WAVES, 20.f);
    enemies.clear();
    towers.clear();
    projectiles.clear();
}

void GameManager::run() {
    while (window.isOpen()) {
        handleInput();
        float dt = deltaClock.restart().asSeconds();

        if (!showTutorial && !paused && !gameOver)
            updateGameLogic(dt);

        renderFrame();
    }
}

void GameManager::handleInput() {
    sf::Event ev;
    while (window.pollEvent(ev)) {
        auto st = input.processEvent(ev, showTutorial);
        if (st.requestExit)     window.close();
        if (st.toggleDebug)     debugMode = !debugMode;
        if (st.advanceTutorial) showTutorial = false;

        if (ev.type == sf::Event::MouseButtonPressed &&
            ev.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mp = window.mapPixelToCoords(
                {ev.mouseButton.x, ev.mouseButton.y});
            handleMouseClick(mp);
        }

        if (ev.type == sf::Event::MouseMoved) {
            sf::Vector2f mp = window.mapPixelToCoords(
                {ev.mouseMove.x, ev.mouseMove.y});
            hoveredTowerIndex = -1;
            for (int i = 0; i < (int)towers.size(); ++i) {
                if (towers[i]->getBounds().contains(mp)) {
                    hoveredTowerIndex = i;
                    break;
                }
            }
        }
    }
}

void GameManager::handleMouseClick(sf::Vector2f mousePos) {
    // Pasamos a UIManager si estamos en Game Over o no
    ui.handleMouseClick(mousePos, gameOver);

    switch (ui.getAction()) {
        case UIManager::Action::Pause:
            paused = !paused;
            break;
        case UIManager::Action::Restart:
            resetGame();
            break;
        case UIManager::Action::Exit:
            window.close();
            break;
        case UIManager::Action::CloseGameOver:
            // Al pulsar "Cerrar" en pantalla de Game Over
            window.close();
            break;
        default:
            break;
    }
    ui.clearAction();

    if (gameOver) return;

    // 1) Mejorar torre seleccionada
    if (ui.getUpgradeBtnBounds().contains(mousePos) &&
        selectedTowerIndex >= 0)
    {
        auto& tw = towers[selectedTowerIndex];
        if (tw->upgrade(gold))
            std::cout << "Torre mejorada a nivel " << tw->getUpgradeLevel()
                      << ", oro=" << gold << "\n";
        return;
    }

    // 2) Selección de tipo de torre a colocar
    if (auto sel = ui.getSelectedTower();
        sel != TowerType::None && buildMode == BuildMode::NONE)
    {
        buildMode = static_cast<BuildMode>(sel);
        selectedTowerIndex = -1;
        return;
    }

    // 3) Colocar torre en el mapa
    if (buildMode != BuildMode::NONE &&
        map.isValidTowerPosition(mousePos))
    {
        placeTower(mousePos);
        ui.setSelectedTowerType(TowerType::None);
        buildMode = BuildMode::NONE;
        return;
    }

    // 4) Seleccionar torre existente
    for (int i = 0; i < (int)towers.size(); ++i) {
        if (towers[i]->getBounds().contains(mousePos)) {
            selectedTowerIndex = i;
            return;
        }
    }
}

void GameManager::updateGameLogic(float dt) {
    // 1) Torres disparan
    std::vector<Enemy*> ep;
    ep.reserve(enemies.size());
    for (auto& e : enemies) ep.push_back(e.get());
    for (auto& t : towers) {
        if (auto* trg = t->tryFire(ep)) {
            shotSound.play();
            projectiles.emplace_back(
                projectileTex, t->getPosition(), trg, 400.f, t->getDamage()
            );
        }
    }

    // 2) Actualizar enemigos y recabar fitness/oro
    for (auto it = enemies.begin(); it != enemies.end();) {
        auto& en = *it;
        en->update(dt);
        if (en->isFinished()) {
            // fitness y recompensa
            if (auto* ind = waves.getIndividualForEnemy(en.get()))
                ind->fitness = en->getWaypointsReached() + (en->getHp()/en->getMaxHp());
            if (!en->hasEscaped()) {
                gold += en->getRewardGold();
                ++totalKills;     // << contabilizar muerte
            }

            // desenganchar proyectiles
            for (auto& p : projectiles)
                if (p.getTarget() == en.get())
                    p.clearTarget();

            it = enemies.erase(it);
        } else {
            ++it;
        }
    }

    // 3) Eliminar proyectiles sin objetivo
    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
            [](const Projectile& p){ return p.getTarget() == nullptr; }),
        projectiles.end()
    );

    // 4) Actualizar proyectiles válidos
    for (auto it = projectiles.begin(); it != projectiles.end();) {
        it->update(dt);
        if (!it->isAlive())
            it = projectiles.erase(it);
        else
            ++it;
    }

    // 5) Spawn y evolución de olas
    waves.update(dt, map, enemies);

    // 6) Comprobar Game Over
    if (!gameOver
        && waves.getCurrentWave() > waves.getTotalWaves()
        && enemies.empty())
    {
        gameOver = true;
    }
}

void GameManager::renderFrame() {
    window.clear();
    window.draw(backgroundSprite);
    map.draw(window);

    if (buildMode != BuildMode::NONE) {
        sf::CircleShape c(100.f);
        c.setFillColor({0,255,0,50});
        c.setOrigin(100,100);
        c.setPosition(buildPosition);
        window.draw(c);
    }

    for (auto& t : towers) t->draw(window);
    for (auto& e : enemies) e->draw(window);
    for (auto& p : projectiles) p.draw(window);

    window.setView(window.getDefaultView());

    // Construir estado UI
    UIState s {
        paused, debugMode, showTutorial,
        gold,
        waves.getCurrentWave(),
        waves.getTotalWaves(),
        static_cast<int>(enemies.size()),
        totalKills,                     // << pasar el acumulado
        waves.getTimeToNext(),
        waves.getCurrentGen(),
        waves.getAverageFitness(),
        waves.getBestFitness(),
        (buildMode!=BuildMode::NONE),
        (selectedTowerIndex>=0 ? towers[selectedTowerIndex]->getUpgradeLevel():0),
        (selectedTowerIndex>=0 ? towers[selectedTowerIndex]->getNextUpgradeCost():0),
        (selectedTowerIndex>=0),
        waves.getFitnessHistory(),
        waves.getMutationRate(),
        0,
        (hoveredTowerIndex>=0 ? towers[hoveredTowerIndex].get():nullptr)
    };

    if (!gameOver) {
        ui.render(window, s);
    } else {
        ui.renderGameOver(window, s);
    }

    window.display();
}

void GameManager::cancelBuilding() {
    buildMode = BuildMode::NONE;
}

void GameManager::placeTower(sf::Vector2f pos) {
    if (!map.isValidTowerPosition(pos) || buildMode == BuildMode::NONE)
        return;
    auto newT = TowerFactory::createTower(
        static_cast<TowerType>(buildMode),
        pos,
        (buildMode == BuildMode::ARCHER ? archerTexture
         : buildMode == BuildMode::MAGE   ? mageTexture
                                          : artilleryTexture)
    );
    if (!newT) return;
    int cost = newT->getCost();
    if (gold < cost) return;
    gold -= cost;
    towers.push_back(std::move(newT));
    ui.setSelectedTowerType(TowerType::None);
    buildMode = BuildMode::NONE;
}
