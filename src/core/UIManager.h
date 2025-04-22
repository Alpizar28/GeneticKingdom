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
};

class UIManager {
public:
    explicit UIManager(sf::Font& font);
    void render(sf::RenderWindow& window, const UIState& state);

    // Para que GameManager pueda detectar clicks en los botones:
    sf::FloatRect getPauseBtnBounds()   const;
    sf::FloatRect getRestartBtnBounds() const;
    sf::FloatRect getExitBtnBounds()    const;

private:
    // Sombra de la barra lateral
    sf::RectangleShape sidebarOverlay;

    // Título del juego
    sf::Text title;

    // Textos HUD
    sf::Text goldText;
    sf::Text waveText;
    sf::Text genText;
    sf::Text avgFitText;
    sf::Text enemiesText;
    sf::Text timerText;
    sf::Text debugText;

    // Botones
    struct Button { sf::RectangleShape box; sf::Text label; };
    Button pauseBtn;
    Button restartBtn;
    Button exitBtn;

    // Tutorial
    sf::RectangleShape tutorialBg;
    sf::Text           tutorialText;

    sf::Font* pFont;
};
