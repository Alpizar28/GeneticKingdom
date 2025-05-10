// UIManager.h
#pragma once
#include <SFML/Graphics.hpp>
#include "../towers/Tower.h"
#include "Constantes.h"
#include <array>
#include <string>

struct UIState {
    bool paused;
    bool debugMode;
    bool showTutorial;
    int gold;
    int currentWave;
    int totalWaves;
    int enemiesCount;
    float timeToNext;
    int currentGen;
    float avgFitness;
    float bestFitness;
    bool isBuildingTower;
    int upgradeLevel = 1;
    int  nextUpgradeCost;
    bool upgradeAvailable;
};

class UIManager {
public:
    UIManager();
    void init(sf::Font& font);
    void render(sf::RenderWindow& window, const UIState& state);

    // Gestión de clics y dibujo de botones
    void handleMouseClick(const sf::Vector2f& mousePos);
    void drawTowerButtons(sf::RenderWindow& window);  // :contentReference[oaicite:2]{index=2}:contentReference[oaicite:3]{index=3}
    TowerType getSelectedTower()       const;
    void setSelectedTowerType(TowerType type);

    sf::FloatRect getPauseBtnBounds()   const;
    sf::FloatRect getRestartBtnBounds() const;
    sf::FloatRect getExitBtnBounds()    const;
    bool isMouseOverUI(const sf::Vector2f& pos) const;
    sf::FloatRect getUpgradeBtnBounds()       const;
    void drawUpgradeButton(sf::RenderWindow& w, int level, int cost, bool enabled);


private:
    TowerType selectedTowerType = TowerType::None;
    sf::Texture archerTexture, mageTexture, artilleryTexture;
    sf::Sprite archerSprite, mageSprite, artillerySprite;

    sf::RectangleShape sidebarOverlay;
    sf::Text title, goldText, waveText, genText, avgFitText, bestFitText,
             enemiesText, timerText, debugText;
    struct Button { sf::RectangleShape box; sf::Text label; };
    Button pauseBtn, restartBtn, exitBtn;
    sf::RectangleShape tutorialBg;
    sf::Text tutorialText;
    bool showingTowerMenu = false;
    std::array<sf::Sprite,3> towerIcons;
    sf::Font* font = nullptr;
};
