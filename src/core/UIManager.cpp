#include "UIManager.h"
#include "Constantes.h"
#include <string>

UIManager::UIManager(sf::Font& font)
  : pFont(&font)
{
    // 1) Sombra de la barra lateral
    const float PW = 192.f;
    const float PX = GAME_WIDTH - PW;
    sidebarOverlay.setSize({ PW, float(GAME_HEIGHT) });
    sidebarOverlay.setPosition(PX, 0);
    sidebarOverlay.setFillColor({ 0, 0, 0, 150 });

    // 2) Título centrado arriba
    title.setFont(font);
    title.setString("Genetic Kingdom");
    title.setCharacterSize(36);
    title.setFillColor(sf::Color::White);
    {
        float x = (GAME_WIDTH - title.getGlobalBounds().width) / 2.f;
        title.setPosition(x, 16.f);
    }

    // 3) Textos del HUD en la barra lateral
    const float marginX = PX + 16.f;
    auto mkText = [&](sf::Text& t, float y, int sz = 18) {
        t.setFont(font);
        t.setCharacterSize(sz);
        t.setFillColor(sf::Color::White);
        t.setPosition(marginX, y);
    };
    mkText(goldText,    80.f);
    mkText(waveText,   120.f);
    mkText(genText,    160.f, 16);
    mkText(avgFitText, 185.f, 16);
    mkText(enemiesText,220.f);
    mkText(timerText,  260.f);
    mkText(debugText,  GAME_HEIGHT - 80.f, 14);

    // 4) Botones
    auto mkBtn = [&](Button& b, float y, const std::string& lbl) {
        b.box.setSize({ PW - 32.f, 40.f });
        b.box.setPosition(PX + 16.f, y);
        b.box.setFillColor({ 70, 70, 70, 200 });
        b.label.setFont(font);
        b.label.setString(lbl);
        b.label.setCharacterSize(18);
        b.label.setFillColor(sf::Color::White);
        float lx = b.box.getPosition().x + (b.box.getSize().x - b.label.getGlobalBounds().width)  / 2.f;
        float ly = b.box.getPosition().y + (b.box.getSize().y - b.label.getGlobalBounds().height) / 2.f;
        b.label.setPosition(lx, ly);
    };
    mkBtn(pauseBtn,    300.f, "Pausa");
    mkBtn(restartBtn,  350.f, "Reiniciar");
    mkBtn(exitBtn,     400.f, "Salir");

    // 5) Panel de tutorial con UTF‑8
    tutorialBg.setSize({ GAME_WIDTH * 0.6f, GAME_HEIGHT * 0.4f });
    tutorialBg.setFillColor({ 40, 40, 40, 220 });
    tutorialBg.setOutlineColor({ 255, 255, 255, 180 });
    tutorialBg.setOutlineThickness(2.f);
    tutorialBg.setPosition(
        (GAME_WIDTH - tutorialBg.getSize().x) / 2.f,
        (GAME_HEIGHT - tutorialBg.getSize().y) / 2.f
    );

    // Usamos un literal raw en UTF‑8 y sf::String::fromUtf8
    const std::string tut = u8R"(Bienvenido a Genetic Kingdom!

- Espera tu 1ª oleada automática.
- Luego coloca tus torres (próximamente).

Presiona cualquier tecla para continuar.
)";
    tutorialText.setFont(font);
    tutorialText.setString(sf::String::fromUtf8(tut.begin(), tut.end()));
    tutorialText.setCharacterSize(16);
    tutorialText.setFillColor(sf::Color::White);
    tutorialText.setPosition(
        tutorialBg.getPosition().x + 20.f,
        tutorialBg.getPosition().y + 20.f
    );
}

void UIManager::render(sf::RenderWindow& window, const UIState& s) {
    // 1) Sombra de la barra lateral
    window.draw(sidebarOverlay);

    // 2) Título
    window.draw(title);

    // 3) HUD dinámico
    goldText.setString("Gold: "   + std::to_string(s.gold));
    window.draw(goldText);

    waveText.setString("Wave: "   + std::to_string(s.currentWave)
                              + " / " + std::to_string(s.totalWaves));
    window.draw(waveText);

    genText.setString("Gen: "     + std::to_string(s.currentGen));
    window.draw(genText);

    avgFitText.setString("AvgFit: " + std::to_string(int(s.avgFitness)));
    window.draw(avgFitText);

    enemiesText.setString("Enemies: " + std::to_string(s.enemiesCount));
    window.draw(enemiesText);

    timerText.setString("Next in: "  + std::to_string(int(s.timeToNext)) + "s");
    window.draw(timerText);

    // 4) Botones
    window.draw(pauseBtn.box);
    window.draw(pauseBtn.label);
    window.draw(restartBtn.box);
    window.draw(restartBtn.label);
    window.draw(exitBtn.box);
    window.draw(exitBtn.label);

    // 5) Panel tutorial sobre todo
    if (s.showTutorial) {
        sf::RectangleShape shadow = tutorialBg;
        shadow.move(5.f, 5.f);
        shadow.setFillColor({ 0, 0, 0, 120 });
        window.draw(shadow);

        window.draw(tutorialBg);
        window.draw(tutorialText);
    }

    // 6) Debug
    if (s.debugMode) {
        debugText.setString(
            "DEBUG:\nPaused=" + std::to_string(s.paused) +
            "\nWave="   + std::to_string(s.currentWave)
        );
        window.draw(debugText);
    }
}
bool UIManager::isMouseOverUI(const sf::Vector2f& mousePos) const {
    // Verifica todos los elementos interactivos de la UI
    return getPauseBtnBounds().contains(mousePos) ||
           getRestartBtnBounds().contains(mousePos) ||
           getExitBtnBounds().contains(mousePos) ||
           sidebarOverlay.getGlobalBounds().contains(mousePos);
}
sf::FloatRect UIManager::getPauseBtnBounds()   const { return pauseBtn.box.getGlobalBounds(); }
sf::FloatRect UIManager::getRestartBtnBounds() const { return restartBtn.box.getGlobalBounds(); }
sf::FloatRect UIManager::getExitBtnBounds()    const { return exitBtn.box.getGlobalBounds(); }
