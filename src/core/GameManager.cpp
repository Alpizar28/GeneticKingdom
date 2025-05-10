// src/core/GameManager.cpp
#include "GameManager.h"
#include "../towers/TowerFactory.h"
#include <iostream>
#include <algorithm>

GameManager::GameManager()
  : window(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT), "Genetic Kingdom",
           sf::Style::Titlebar | sf::Style::Close),
    input(), waves(5, 0.1f, 20, 20.f), ui()
{
    window.setFramerateLimit(TARGET_FPS);
    if (!font.loadFromFile(FONT_PATH))
        std::cerr<<"❌ Error cargando fuente\n";
    ui.init(font);

    archerTexture.loadFromFile("assets/sprites/towers/arqueros.png");
    mageTexture.loadFromFile("assets/sprites/towers/magos.png");
    artilleryTexture.loadFromFile("assets/sprites/towers/artilleros.png");
    projectileTex.loadFromFile("assets/sprites/projectiles/bullet.png");
    shotBuffer.loadFromFile("assets/sounds/shoot.wav");
    shotSound.setBuffer(shotBuffer);

    if (backgroundTex.loadFromFile("assets/images/game_background.png")) {
        backgroundSprite.setTexture(backgroundTex);
        backgroundSprite.setScale(
            float(GAME_WIDTH)/backgroundTex.getSize().x,
            float(GAME_HEIGHT)/backgroundTex.getSize().y
        );
    }

    resetGame();
}

void GameManager::resetGame() {
    paused            = false;
    debugMode         = false;
    showTutorial      = true;
    gold              = 100;
    buildMode         = BuildMode::NONE;
    selectedTowerIndex= -1;
    waves = WaveManager(5, 0.1f, 20, 20.f);
    enemies.clear();
    towers.clear();
    projectiles.clear();
}

void GameManager::run() {
    while (window.isOpen()) {
        handleInput();
        float dt = deltaClock.restart().asSeconds();
        if (!showTutorial && !paused)
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

        if (buildMode != BuildMode::NONE) {
            buildPosition = window.mapPixelToCoords(
                sf::Mouse::getPosition(window));
        }
    }
}

void GameManager::handleMouseClick(sf::Vector2f mousePos) {
    ui.handleMouseClick(mousePos);

    // Botones de la UI
    if (ui.getPauseBtnBounds().contains(mousePos)) {
        paused = !paused;
        return;
    }
    if (ui.getRestartBtnBounds().contains(mousePos)) {
        resetGame();
        return;
    }
    if (ui.getExitBtnBounds().contains(mousePos)) {
        window.close();
        return;
    }

    // Upgrade de torre seleccionada
    if (ui.getUpgradeBtnBounds().contains(mousePos) && selectedTowerIndex >= 0) {
        auto& tw = towers[selectedTowerIndex];
        if (tw->upgrade(gold))
            std::cout << "Torre mejorada a nivel " << tw->getUpgradeLevel()
                      << ", oro restante " << gold << "\n";
        else
            std::cout << "No se pudo mejorar (oro insuficiente o nivel máximo)\n";
        return;
    }

    // Selección de torre a colocar
    if (auto sel = ui.getSelectedTower();
        sel != TowerType::None && buildMode == BuildMode::NONE)
    {
        buildMode = static_cast<BuildMode>(sel);
        selectedTowerIndex = -1;
        return;
    }

    // Colocación de torre en el mapa
    if (buildMode != BuildMode::NONE &&
        map.isValidTowerPosition(mousePos))
    {
        placeTower(mousePos);
        ui.setSelectedTowerType(TowerType::None);
        buildMode = BuildMode::NONE;
        return;
    }

    // Selección de torre existente
    for (int i = 0; i < (int)towers.size(); ++i) {
        if (towers[i]->getBounds().contains(mousePos)) {
            selectedTowerIndex = i;
            return;
        }
    }
}

void GameManager::updateGameLogic(float dt) {
    std::vector<Enemy*> ep;
    ep.reserve(enemies.size());
    for (auto& e : enemies) ep.push_back(e.get());

    // Torres disparan
    for (auto& t : towers) {
        if (auto* trg = t->tryFire(ep)) {
            shotSound.play();
            projectiles.emplace_back(
                projectileTex,
                t->getPosition(),
                trg,
                400.f,
                t->getDamage()
            );
        }
    }

    // Proyectiles
    for (auto it = projectiles.begin(); it != projectiles.end();) {
        it->update(dt);
        if (!it->isAlive())
            it = projectiles.erase(it);
        else
            ++it;
    }

    // Enemigos
    for (auto it = enemies.begin(); it != enemies.end();) {
        auto& en = *it;
        en->update(dt);
        if (en->isFinished()) {
            projectiles.erase(
                std::remove_if(
                    projectiles.begin(),
                    projectiles.end(),
                    [&](auto& p){ return p.getTarget() == en.get(); }
                ),
                projectiles.end()
            );
            if (auto* ind = waves.getIndividualForEnemy(en.get()))
                ind->fitness = en->getWaypointsReached()
                             + (en->getHp() / en->getMaxHp());
            if (!en->hasEscaped())
                gold += en->getRewardGold();
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }

    waves.update(dt, map, enemies);
}

void GameManager::renderFrame() {
    window.clear();
    window.draw(backgroundSprite);
    map.draw(window);

    if (buildMode != BuildMode::NONE) {
        sf::CircleShape c(100.f);
        c.setFillColor({0,255,0,50});
        c.setOrigin(100.f,100.f);
        c.setPosition(buildPosition);
        window.draw(c);
    }

    for (auto& t : towers)    t->draw(window);
    for (auto& e : enemies)   e->draw(window);
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
        waves.getBestFitness(),
        (buildMode != BuildMode::NONE),
        (selectedTowerIndex >= 0 ? towers[selectedTowerIndex]->getNextUpgradeCost() : 0),
        (selectedTowerIndex >= 0)
    };
    ui.render(window, s);
    ui.drawTowerButtons(window);
    ui.drawUpgradeButton(window, s.upgradeLevel, s.nextUpgradeCost, s.upgradeAvailable);
    window.display();
}

void GameManager::cancelBuilding() {
    buildMode = BuildMode::NONE;
}

void GameManager::placeTower(sf::Vector2f pos) {
    if (!map.isValidTowerPosition(pos) || buildMode == BuildMode::NONE)
        return;
    const int cost = 100;
    if (gold < cost) return;
    const sf::Texture* tex = nullptr;
    switch (buildMode) {
        case BuildMode::ARCHER:    tex = &archerTexture;    break;
        case BuildMode::MAGE:      tex = &mageTexture;      break;
        case BuildMode::ARTILLERY: tex = &artilleryTexture; break;
        default: return;
    }
    if (auto nt = TowerFactory::createTower(
            static_cast<TowerType>(buildMode), pos, *tex))
    {
        towers.push_back(std::move(nt));
        gold -= cost;
    }
    buildMode = BuildMode::NONE;
}
