#include "UIManager.h"
#include "Constantes.h"
#include <iostream>

UIManager::UIManager() { }

void UIManager::init(sf::Font& fontRef) {
    font = &fontRef;

    // Sidebar background
    const float PW = 192, PX = GAME_WIDTH - PW;
    sidebarOverlay.setSize({ PW, float(GAME_HEIGHT) });
    sidebarOverlay.setPosition(PX, 0);
    sidebarOverlay.setFillColor({ 0, 0, 0, 150 });

    // Title
    title.setFont(*font);
    title.setString("Genetic Kingdom");
    title.setCharacterSize(36);
    title.setFillColor(sf::Color::White);
    title.setPosition((GAME_WIDTH - title.getGlobalBounds().width) / 2, 16);

    // Prepare state texts
    for (sf::Text* t : { &waveText, &genText, &avgFitText, &bestFitText, &enemiesText, &timerText, &debugText }) {
        t->setFont(*font);
        t->setFillColor(sf::Color::White);
    }
    waveText.setCharacterSize(18);
    genText.setCharacterSize(18);
    avgFitText.setCharacterSize(16);
    bestFitText.setCharacterSize(16);
    enemiesText.setCharacterSize(18);
    timerText.setCharacterSize(18);
    debugText.setCharacterSize(14);

    // Buttons
    auto mkBtn = [&](Button& b, const char* str) {
        b.box.setSize({ 160, 40 });
        b.box.setFillColor({ 70, 70, 70, 200 });
        b.label.setFont(*font);
        b.label.setString(str);
        b.label.setCharacterSize(18);
        b.label.setFillColor(sf::Color::White);
    };
    mkBtn(configBtn,  "Config");
    mkBtn(pauseBtn,   "Pausar");
    mkBtn(restartBtn, "Reiniciar");
    mkBtn(exitBtn,    "Salir");

    // Load tower icons
    if (!archerTexture.loadFromFile("assets/sprites/towers/btn_arquero.png") ||
        !mageTexture.loadFromFile("assets/sprites/towers/btn_mage.png") ||
        !artilleryTexture.loadFromFile("assets/sprites/towers/btn_artillery.png")) {
        std::cerr << "Error cargando texturas de torres" << std::endl;
    }
    archerSprite.setTexture(archerTexture);
    mageSprite.setTexture(mageTexture);
    artillerySprite.setTexture(artilleryTexture);

    // Tutorial
    tutorialBg.setSize({ GAME_WIDTH * 0.6f, GAME_HEIGHT * 0.4f });
    tutorialBg.setFillColor({ 40, 40, 40, 220 });
    tutorialBg.setOutlineColor({ 255, 255, 255, 180 });
    tutorialBg.setOutlineThickness(2);
    tutorialBg.setPosition(
        (GAME_WIDTH - tutorialBg.getSize().x) / 2,
        (GAME_HEIGHT - tutorialBg.getSize().y) / 2
    );
    tutorialText.setFont(*font);
    tutorialText.setString(
        "Bienvenido a Genetic Kingdom!\n\n"
        "- Espera tu 1ª oleada.\n"
        "- Luego coloca tus torres.\n\n"
        "Presiona cualquier tecla para continuar."
    );
    tutorialText.setCharacterSize(16);
    tutorialText.setFillColor(sf::Color::White);
    tutorialText.setPosition(
        tutorialBg.getPosition().x + 20,
        tutorialBg.getPosition().y + 20
    );
}

void UIManager::render(sf::RenderWindow& w, const UIState& s) {
    // 0) Tutorial overlay
    if (s.showTutorial) {
        sf::RectangleShape full({ float(GAME_WIDTH), float(GAME_HEIGHT) });
        full.setFillColor({ 0, 0, 0, 180 });
        w.draw(full);
        w.draw(tutorialBg);
        w.draw(tutorialText);
        return;
    }

    // 1) Use default view for UI
    w.setView(w.getDefaultView());

    const float PW = sidebarOverlay.getSize().x;
    const float PX = sidebarOverlay.getPosition().x;

    // 2) Sidebar background + title
    w.draw(sidebarOverlay);
    w.draw(title);

    // 3) Config button
    float cfgX = PX + (PW - configBtn.box.getSize().x) / 2;
    float cfgY = 8.f;
    configBtn.box.setPosition(cfgX, cfgY);
    configBtn.label.setPosition(
        cfgX + (configBtn.box.getSize().x - configBtn.label.getGlobalBounds().width) / 2,
        cfgY + (configBtn.box.getSize().y - configBtn.label.getGlobalBounds().height) / 2
    );
    w.draw(configBtn.box);
    w.draw(configBtn.label);

    // 4) Dynamic stats below Config
    float statsX = PX + 16.f;
    float statsY = cfgY + configBtn.box.getSize().y + 8.f;

    waveText.setString("Wave:    " + std::to_string(s.currentWave) + "/" + std::to_string(s.totalWaves));
    waveText.setPosition(statsX, statsY); w.draw(waveText);
    statsY += 22.f;

    genText.setString("Gen:     " + std::to_string(s.currentGen));
    genText.setPosition(statsX, statsY); w.draw(genText);
    statsY += 20.f;

    avgFitText.setString("AvgFit:  " + std::to_string(int(s.avgFitness)));
    avgFitText.setPosition(statsX, statsY); w.draw(avgFitText);
    statsY += 20.f;

    bestFitText.setString("BestFit: " + std::to_string(int(s.bestFitness)));
    bestFitText.setPosition(statsX, statsY); w.draw(bestFitText);
    statsY += 20.f;

    enemiesText.setString("Enemies: " + std::to_string(s.enemiesCount));
    enemiesText.setPosition(statsX, statsY); w.draw(enemiesText);
    statsY += 24.f;

    sf::Text mutText(("Mut: " + std::to_string(int(s.mutationRate*100)) + "%"), *font, 14);
    mutText.setFillColor(sf::Color::White);
    mutText.setPosition(statsX, statsY); w.draw(mutText);
    statsY += 20.f;
    
    sf::Text histLabel("Historial de fitness:", *font, 14);
    histLabel.setFillColor(sf::Color::White);
    histLabel.setPosition(statsX, statsY);
    w.draw(histLabel);
    statsY += 18.f;

    for (size_t i = 0; i < s.fitnessHistory.size(); ++i) {
        const auto& gen = s.fitnessHistory[i];
        std::string line = "G" + std::to_string(i + 1) + ": ";
        for (float f : gen) line += std::to_string(int(f)) + " ";
        sf::Text txt(line, *font, 12);
        txt.setFillColor(sf::Color::White);
        txt.setPosition(statsX, statsY);
        w.draw(txt);
        statsY += 16.f;
        if (statsY > GAME_HEIGHT - 120) break;
    }

    // 5) Draw stats early, then dropdown to overlay stats
    if (showDropdown) {
        const float pad = 4;
        const float btnH = pauseBtn.box.getSize().y;
        sf::RectangleShape dropBg({ pauseBtn.box.getSize().x + pad * 2,
                                    3 * btnH + pad * 4 });
        dropBg.setFillColor({ 50, 50, 50, 220 });
        dropBg.setPosition(cfgX - pad, cfgY + configBtn.box.getSize().y);
        w.draw(dropBg);

        float y0 = cfgY + configBtn.box.getSize().y + pad;
        for (auto& pr : std::initializer_list<std::pair<Button&, Action>>{
                 { pauseBtn,   Action::Pause },
                 { restartBtn, Action::Restart },
                 { exitBtn,    Action::Exit }
        }) {
            Button& b = pr.first;
            b.box.setPosition(cfgX, y0);
            b.label.setPosition(
                cfgX + (b.box.getSize().x - b.label.getGlobalBounds().width) / 2,
                y0 + (b.box.getSize().y - b.label.getGlobalBounds().height) / 2
            );
            w.draw(b.box);
            w.draw(b.label);
            y0 += btnH + pad;
        }
    }

    // 6) Tower icons positioned so artillery flush above improve button
    float iconW = float(archerTexture.getSize().x);
    float iconH = float(archerTexture.getSize().y);
    float iconsX = PX + (PW - iconW) / 2.f;
    float improveY = GAME_HEIGHT - 80.f + 20.f; // top of improve button
    float iconsY = improveY - 3 * iconH;

    archerSprite.setPosition(iconsX, iconsY);        w.draw(archerSprite);
    mageSprite.setPosition(iconsX, iconsY + iconH); w.draw(mageSprite);
    artillerySprite.setPosition(iconsX, iconsY + 2*iconH); w.draw(artillerySprite);

    // 7) Upgrade button
    drawUpgradeButton(w, s.upgradeLevel, s.nextUpgradeCost, s.upgradeAvailable);

    // 8) Gold at bottom-left of game
    goldText.setFont(*font);
    goldText.setCharacterSize(18);
    goldText.setFillColor(sf::Color::White);
    goldText.setString("Gold: " + std::to_string(s.gold));
    goldText.setPosition(16, GAME_HEIGHT - 32);
    w.draw(goldText);

    // 9) Tooltip on hover
    if (s.hoveredTower) {
        std::string tip =
            "Damage:   " + std::to_string(s.hoveredTower->getDamage())    + "\n" +
            "Cost:     " + std::to_string(s.hoveredTower->getCost())       + "\n" +
            "Range:    " + std::to_string(int(s.hoveredTower->getRange())) + "\n" +
            "Cooldown: " + std::to_string(s.hoveredTower->getCooldown())   + "s";

        sf::Text tooltipText(tip, *font, 14);
        tooltipText.setFillColor(sf::Color::White);
        sf::Vector2f mousePos = w.mapPixelToCoords(sf::Mouse::getPosition(w));
        tooltipText.setPosition(mousePos + sf::Vector2f{ 12.f, -tooltipText.getLocalBounds().height - 12.f });

        sf::FloatRect bounds = tooltipText.getGlobalBounds();
        sf::RectangleShape tooltipBg({ bounds.width + 8.f, bounds.height + 8.f });
        tooltipBg.setFillColor({ 0, 0, 0, 180 });
        tooltipBg.setPosition(bounds.left - 4.f, bounds.top - 4.f);

        w.draw(tooltipBg);
        w.draw(tooltipText);
    }
}

void UIManager::handleMouseClick(const sf::Vector2f& m) {
    // Config toggle
    if (configBtn.box.getGlobalBounds().contains(m)) {
        showDropdown = !showDropdown;
        lastAction   = Action::None;
        return;
    }
    // Dropdown actions
    if (showDropdown) {
        if (pauseBtn.box.getGlobalBounds().contains(m))     lastAction = Action::Pause;
        else if (restartBtn.box.getGlobalBounds().contains(m)) lastAction = Action::Restart;
        else if (exitBtn.box.getGlobalBounds().contains(m))    lastAction = Action::Exit;
        showDropdown = false;
        return;
    }
    // Tower selection
    if (archerSprite.getGlobalBounds().contains(m))      selectedTower = TowerType::Archer;
    else if (mageSprite.getGlobalBounds().contains(m))   selectedTower = TowerType::Mage;
    else if (artillerySprite.getGlobalBounds().contains(m)) selectedTower = TowerType::Artillery;
}

UIManager::Action UIManager::getAction() const {
    return lastAction;
}

void UIManager::clearAction() {
    lastAction = Action::None;
}

TowerType UIManager::getSelectedTower() const {
    return selectedTower;
}

void UIManager::setSelectedTowerType(TowerType t) {
    selectedTower = t;
}

sf::FloatRect UIManager::getUpgradeBtnBounds() const {
    float PX = sidebarOverlay.getPosition().x + 16.f;
    return { { PX, float(GAME_HEIGHT - 60) },
             { sidebarOverlay.getSize().x - 32.f, 30.f } };
}

void UIManager::drawUpgradeButton(sf::RenderWindow& w, int level, int cost, bool enabled) {
    float PX = sidebarOverlay.getPosition().x;
    sf::Vector2f pos(PX + 16.f, GAME_HEIGHT - 80.f);

    sf::Text label("Mejorar:", *font, 16);
    label.setFillColor(sf::Color::White);
    label.setPosition(pos);
    w.draw(label);

    sf::RectangleShape btn({ sidebarOverlay.getSize().x - 32.f, 30.f });
    btn.setPosition(pos.x, pos.y + 20.f);
    btn.setFillColor(enabled ? sf::Color::Green : sf::Color(100,100,100));
    w.draw(btn);

    std::string txtStr = "Nivel " + std::to_string(level) + ": $" + std::to_string(cost);
    sf::Text txt(txtStr, *font, 16);
    txt.setFillColor(sf::Color::White);
    txt.setPosition(btn.getPosition() + sf::Vector2f{ 10.f, 5.f });
    w.draw(txt);
}

void UIManager::drawTowerButtons(sf::RenderWindow& w) {
    w.draw(archerSprite);
    w.draw(mageSprite);
    w.draw(artillerySprite);
}
