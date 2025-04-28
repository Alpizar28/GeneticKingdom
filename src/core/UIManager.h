#pragma once
#include <SFML/Graphics.hpp>
#include "Constantes.h"

struct UIState {
    bool paused;
    bool debugMode;
    bool showTutorial;
    int  gold;
    int  currentWave;
    int  totalWaves;
    int  enemiesCount;
    float timeToNext;
    int   currentGen;
    float avgFitness;
    float bestFitness;    // ← añadido
};

class UIManager {
public:
    UIManager()=default;
    void init(sf::Font& font);
    void render(sf::RenderWindow&, const UIState&);

    sf::FloatRect getPauseBtnBounds()   const;
    sf::FloatRect getRestartBtnBounds() const;
    sf::FloatRect getExitBtnBounds()    const;
    bool          isMouseOverUI(const sf::Vector2f&) const;

private:
    sf::RectangleShape sidebarOverlay;
    sf::Text title,
             goldText, waveText,
             genText, avgFitText, bestFitText,
             enemiesText, timerText,
             debugText;
    struct Button{ sf::RectangleShape box; sf::Text label; };
    Button pauseBtn, restartBtn, exitBtn;
    sf::RectangleShape tutorialBg;
    sf::Text           tutorialText;
};
