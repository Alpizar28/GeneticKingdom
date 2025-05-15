#pragma once

#include <SFML/Graphics.hpp>
#include "../towers/Tower.h"
#include "Constantes.h"
#include <vector>

struct UIState {
    bool paused;
    bool debugMode;
    bool showTutorial;
    int  gold;
    int  currentWave;
    int  totalWaves;
    int  enemiesCount;
    int  totalKills;      
    float timeToNext;
    int   currentGen;
    float avgFitness;
    float bestFitness;
    bool  isBuildingTower;
    int   upgradeLevel;
    int   nextUpgradeCost;
    bool  upgradeAvailable;
    const std::vector<std::vector<float>>& fitnessHistory;
    float mutationRate;
    int dummy;             
    const Tower* hoveredTower;
};


class UIManager {
public:
    enum class Action { None, Pause, Restart, Exit, CloseGameOver };

    UIManager();
    void init(sf::Font& font);
    void render(sf::RenderWindow& w, const UIState& s);
    void renderGameOver(sf::RenderWindow& w, const UIState& s);
    void handleMouseClick(const sf::Vector2f& mousePos, bool gameOver);

    Action getAction() const;
    void   clearAction();

    TowerType getSelectedTower() const;
    void      setSelectedTowerType(TowerType t);

    sf::FloatRect getUpgradeBtnBounds() const;
    void drawUpgradeButton(sf::RenderWindow& w, int level, int cost, bool enabled);

private:
    // Sidebar
    sf::RectangleShape sidebarOverlay;
    sf::Text           title;

    // Stats
    sf::Text goldText, waveText, genText, enemiesText, totalKillsText,
             mutText, avgFitText, bestFitText;

    // Buttons
    struct Button { sf::RectangleShape box; sf::Text label; };
    Button pauseBtn, restartBtn, exitBtn, fitnessBtn, gameOverBtn;

    // Tutorial
    sf::RectangleShape tutorialBg;
    sf::Text           tutorialText;

    // Tower icons
    sf::Texture archerTexture, mageTexture, artilleryTexture;
    sf::Sprite  archerSprite,   mageSprite,   artillerySprite;

    sf::Font* font = nullptr;

    // Internal
    bool      showFitnessDropdown = false;
    Action    lastAction          = Action::None;
    TowerType selectedTower       = TowerType::None;
};
