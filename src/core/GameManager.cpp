#include "GameManager.h"
#include "../ai/Pathfinding.h"
#include "Constantes.h"
#include <iostream>


GameManager::GameManager()
    : window(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT), "Genetic Kingdom")
    , ga(20, 5, 0.1f) // población=20, enemigos/oleada=5, mutRate=0.1
{
    window.setFramerateLimit(TARGET_FPS);

    if (!font.loadFromFile(FONT_PATH)) {
        std::cerr << "❌ Error al cargar la fuente." << std::endl;
    }

    // Título
    title.setFont(font);
    title.setString("Genetic Kingdom");
    title.setCharacterSize(36);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);
    float centerX = (window.getSize().x - title.getGlobalBounds().width) / 2;
    title.setPosition(centerX, 16);

    // Sidebar
    const float PANEL_WIDTH = 192.f;
    const float PANEL_X = GAME_WIDTH - PANEL_WIDTH;
    if (!sidebarTex.loadFromFile("assets/images/sidebar_blur.png")) {
        std::cerr << "❌ Error al cargar sidebar_blur.png" << std::endl;
    } else {
        sidebarSprite.setTexture(sidebarTex);
        sidebarSprite.setPosition(PANEL_X, 0);
        sidebarSprite.setScale(
            PANEL_WIDTH / sidebarTex.getSize().x,
            GAME_HEIGHT / sidebarTex.getSize().y
        );
    }
    sidebarOverlay.setSize({ PANEL_WIDTH, static_cast<float>(GAME_HEIGHT) });
    sidebarOverlay.setPosition(PANEL_X, 0);
    sidebarOverlay.setFillColor(sf::Color(0, 0, 0, 100));

    // HUD Textos
    goldText.setFont(font);
    goldText.setCharacterSize(22);
    goldText.setFillColor(sf::Color::White);
    goldText.setString("Gold: " + std::to_string(gold));
    goldText.setPosition(PANEL_X + 16, 120);

    waveText.setFont(font);
    waveText.setCharacterSize(22);
    waveText.setFillColor(sf::Color::White);
    waveText.setString("Wave: " + std::to_string(wave) + " / " + std::to_string(totalWaves));
    waveText.setPosition(PANEL_X + 16, 160);

    enemiesText.setFont(font);
    enemiesText.setCharacterSize(22);
    enemiesText.setFillColor(sf::Color::White);
    enemiesText.setString("Enemies: 0");
    enemiesText.setPosition(PANEL_X + 16, 200);

    // Botón Start Wave
    startWaveButton.setSize({ PANEL_WIDTH - 32, 48 });
    startWaveButton.setPosition(PANEL_X + 16, 600);
    startWaveButton.setFillColor(sf::Color(70, 70, 70));

    startWaveText.setFont(font);
    startWaveText.setString("Start Wave");
    startWaveText.setCharacterSize(20);
    startWaveText.setFillColor(sf::Color::White);
    {
        float tx = startWaveButton.getPosition().x
                 + (startWaveButton.getSize().x - startWaveText.getLocalBounds().width) / 2;
        float ty = startWaveButton.getPosition().y
                 + (startWaveButton.getSize().y - startWaveText.getLocalBounds().height) / 2
                 - 5;
        startWaveText.setPosition(tx, ty);
    }

    // Fondo de juego
    if (!backgroundTex.loadFromFile("assets/images/game_background.png")) {
        std::cerr << "❌ Error al cargar game_background.png" << std::endl;
    } else {
        backgroundSprite.setTexture(backgroundTex);
        backgroundSprite.setScale(
            float(GAME_WIDTH) / backgroundTex.getSize().x,
            float(GAME_HEIGHT) / backgroundTex.getSize().y
        );
    }
}

void GameManager::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void GameManager::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        // Sólo lanzar oleada si no hay una activa
        if (!waveActive
            && event.type == sf::Event::MouseButtonPressed
            && event.mouseButton.button == sf::Mouse::Left) 
        {
            sf::Vector2f mouse(event.mouseButton.x, event.mouseButton.y);
            if (startWaveButton.getGlobalBounds().contains(mouse)) {
                // Construir ruta
                auto start = map.findLeftmostPathTile();
                auto end   = map.findRightmostPathTile();
                Pathfinding pf(map);
                auto path = pf.findPath(start, end);

                if (!path.empty()) {
                    // Genera la oleada con el GA
                    enemies = ga.createWave(path);
                    waveActive = true;
                }
            }
        }
    }
}

void GameManager::update() {
    float dt = deltaClock.restart().asSeconds();

    // Actualizar enemigos y eliminar los que finalizaron
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        (*it)->update(dt);
        if ((*it)->isFinished()) it = enemies.erase(it);
        else ++it;
    }

    enemiesText.setString("Enemies: " + std::to_string(enemies.size()));

    // Cuando todos terminen, evolucionar y próxima oleada
    if (waveActive && enemies.empty()) {
        ga.evolve();
        ++wave;
        waveText.setString("Wave: " + std::to_string(wave) + " / " + std::to_string(totalWaves));
        waveActive = false;
    }
}

void GameManager::render() {
    window.clear();

    // 1) Fondo
    window.draw(backgroundSprite);

    // 2) Mapa y enemigos
    map.draw(window);
    for (auto& e : enemies) 
        e->draw(window);

    // 3) UI encima
    window.draw(sidebarSprite);
    window.draw(sidebarOverlay);
    window.draw(title);
    window.draw(goldText);
    window.draw(waveText);
    window.draw(enemiesText);
    window.draw(startWaveButton);
    window.draw(startWaveText);

    window.display();
}
