#pragma once
#include <SFML/Graphics.hpp>
#include "../towers/Tower.h"
#include "Constantes.h"
#include <vector>

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
    int upgradeLevel;
    int nextUpgradeCost;
    bool upgradeAvailable;
    std::vector<std::vector<float>> fitnessHistory;
    float mutationRate;           // para mostrar “Mut: XX%”
    const Tower* hoveredTower;    // para el tooltip de la torre bajo el cursor
};

class UIManager {
public:
    enum class Action { None, Pause, Restart, Exit };

    UIManager();
    void init(sf::Font& font);
    void render(sf::RenderWindow& window, const UIState& state);
    void handleMouseClick(const sf::Vector2f& mousePos);

    // Query del menú desplegable
    Action getAction() const;
    void   clearAction();

    // Para que GameManager obtenga la torre seleccionada
    TowerType getSelectedTower() const;
    void      setSelectedTowerType(TowerType t);

    // Bounds para upgrade
    sf::FloatRect getUpgradeBtnBounds() const;
    void drawUpgradeButton(sf::RenderWindow& w, int level, int cost, bool enabled);

    // Dibujo de botones de torres
    void drawTowerButtons(sf::RenderWindow& w);

private:
    // --- Widgets UI ---
    sf::RectangleShape sidebarOverlay;
    sf::Text title,
             goldText, waveText, genText, avgFitText,
             bestFitText, enemiesText, timerText, debugText,
             tutorialText;
    sf::RectangleShape tutorialBg;

    struct Button {
        sf::RectangleShape box;
        sf::Text label;
    };

    Button configBtn, pauseBtn, restartBtn, exitBtn;

    // Sprites de torres
    sf::Texture archerTexture, mageTexture, artilleryTexture;
    sf::Sprite  archerSprite, mageSprite, artillerySprite;

    sf::Font* font = nullptr;

    // Estado interno
    bool     showDropdown    = false;
    Action   lastAction      = Action::None;
    TowerType selectedTower = TowerType::None;
};
