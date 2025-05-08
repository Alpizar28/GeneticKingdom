// UIManager.cpp
#include "UIManager.h"
#include "Constantes.h"
#include <iostream>

UIManager::UIManager() { }

void UIManager::init(sf::Font& font) {
    this->font = &font;
    if (!this->font) {
        std::cerr<<"Error: fuente inválida en UIManager\n";
        return;
    }
    const float PW = 192, PX = GAME_WIDTH - PW;
    sidebarOverlay.setSize({PW, float(GAME_HEIGHT)});
    sidebarOverlay.setPosition(PX, 0.f);
    sidebarOverlay.setFillColor({0,0,0,150});

    title.setFont(font);
    title.setString("Genetic Kingdom");
    title.setCharacterSize(36);
    title.setFillColor(sf::Color::White);
    title.setPosition((GAME_WIDTH - title.getGlobalBounds().width)/2, 16);

    auto mkText = [&](sf::Text& t, float y, int sz=18){
        t.setFont(font);
        t.setCharacterSize(sz);
        t.setFillColor(sf::Color::White);
        t.setPosition(PX + 16, y);
    };
    mkText(goldText, 80);
    mkText(waveText,120);
    mkText(genText,160,16);
    mkText(avgFitText,185,16);
    mkText(bestFitText,205,16);
    mkText(enemiesText,240);
    mkText(timerText,280);
    mkText(debugText, GAME_HEIGHT-80,14);

    auto mkBtn = [&](Button& b, float y, const char* str){
        b.box.setSize({PW-32, 40});
        b.box.setPosition(PX+16, y);
        b.box.setFillColor({70,70,70,200});
        b.label.setFont(font);
        b.label.setString(str);
        b.label.setCharacterSize(18);
        b.label.setFillColor(sf::Color::White);
        b.label.setPosition(
            b.box.getPosition().x + (b.box.getSize().x - b.label.getGlobalBounds().width)/2,
            b.box.getPosition().y + (b.box.getSize().y - b.label.getGlobalBounds().height)/2
        );
    };
    mkBtn(pauseBtn,   320, "Pausa");
    mkBtn(restartBtn, 370, "Reiniciar");
    mkBtn(exitBtn,    420, "Salir");

    archerButton.setSize({100,40});
    archerButton.setPosition(10,150);
    archerButton.setFillColor(sf::Color::Green);

    mageButton.setSize({100,40});
    mageButton.setPosition(10,200);
    mageButton.setFillColor(sf::Color::Blue);

    artilleryButton.setSize({100,40});
    artilleryButton.setPosition(10,250);
    artilleryButton.setFillColor(sf::Color::Red);

    tutorialBg.setSize({GAME_WIDTH*0.6f, GAME_HEIGHT*0.4f});
    tutorialBg.setFillColor({40,40,40,220});
    tutorialBg.setOutlineColor({255,255,255,180});
    tutorialBg.setOutlineThickness(2);
    tutorialBg.setPosition(
        (GAME_WIDTH - tutorialBg.getSize().x)/2,
        (GAME_HEIGHT - tutorialBg.getSize().y)/2
    );
    tutorialText.setFont(font);
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
    w.draw(sidebarOverlay);
    w.draw(title);

    goldText.setString("Gold: "+std::to_string(s.gold));       w.draw(goldText);
    waveText.setString("Wave: "+std::to_string(s.currentWave)+
                       "/"+std::to_string(s.totalWaves));      w.draw(waveText);
    genText.setString("Gen: "+std::to_string(s.currentGen));   w.draw(genText);
    avgFitText.setString("AvgFit: "+std::to_string(int(s.avgFitness))); w.draw(avgFitText);
    bestFitText.setString("BestFit: "+std::to_string(int(s.bestFitness))); w.draw(bestFitText);
    enemiesText.setString("Enemies: "+std::to_string(s.enemiesCount));     w.draw(enemiesText);
    timerText.setString("Next: "+std::to_string(int(s.timeToNext))+"s");   w.draw(timerText);

    w.draw(pauseBtn.box);   w.draw(pauseBtn.label);
    w.draw(restartBtn.box); w.draw(restartBtn.label);
    w.draw(exitBtn.box);    w.draw(exitBtn.label);

    if (s.showTutorial) {
        sf::RectangleShape sh = tutorialBg; sh.move(5,5);
        sh.setFillColor({0,0,0,120});
        w.draw(sh);
        w.draw(tutorialBg);
        w.draw(tutorialText);
    }
    if (s.debugMode) {
        debugText.setString("DEBUG\nPaused="+std::to_string(s.paused)
                          +"\nWave="+std::to_string(s.currentWave));
        w.draw(debugText);
    }
}

void UIManager::handleMouseClick(const sf::Vector2f& m) {
    if (archerButton.getGlobalBounds().contains(m)) {
        selectedTowerType = TowerType::Archer;
        std::cout<<"Torre arquera seleccionada\n";
    } else if (mageButton.getGlobalBounds().contains(m)) {
        selectedTowerType = TowerType::Mage;
        std::cout<<"Torre mágica seleccionada\n";
    } else if (artilleryButton.getGlobalBounds().contains(m)) {
        selectedTowerType = TowerType::Artillery;
        std::cout<<"Torre artillera seleccionada\n";
    }
}

TowerType UIManager::getSelectedTower() const {
    return selectedTowerType;
}

void UIManager::setSelectedTowerType(TowerType t) {
    selectedTowerType = t;
}

void UIManager::drawTowerButtons(sf::RenderWindow& w) {
    w.draw(archerButton);
    w.draw(mageButton);
    w.draw(artilleryButton);
}
// UIManager.cpp
void UIManager::drawUpgradeButton(sf::RenderWindow& w, int cost, bool enabled) {
    // Dibuja un botón y texto con el costo
    sf::RectangleShape btn({100,30});
    btn.setPosition(10, GAME_HEIGHT-50);
    btn.setFillColor(enabled ? sf::Color::Green : sf::Color(100,100,100));
    w.draw(btn);
    sf::Text txt(std::to_string(cost), *font, 16);
    txt.setPosition(btn.getPosition()+sf::Vector2f{10,5});
    w.draw(txt);
}

sf::FloatRect UIManager::getUpgradeBtnBounds() const {
    return sf::FloatRect({10, GAME_HEIGHT-50}, {100,30});
}

sf::FloatRect UIManager::getPauseBtnBounds()   const { return pauseBtn.box.getGlobalBounds(); }
sf::FloatRect UIManager::getRestartBtnBounds() const { return restartBtn.box.getGlobalBounds(); }
sf::FloatRect UIManager::getExitBtnBounds()    const { return exitBtn.box.getGlobalBounds(); }
bool UIManager::isMouseOverUI(const sf::Vector2f& p) const {
    return getPauseBtnBounds().contains(p)
        || getRestartBtnBounds().contains(p)
        || getExitBtnBounds().contains(p)
        || sidebarOverlay.getGlobalBounds().contains(p)
        || archerButton.getGlobalBounds().contains(p)
        || mageButton.getGlobalBounds().contains(p)
        || artilleryButton.getGlobalBounds().contains(p);
}
