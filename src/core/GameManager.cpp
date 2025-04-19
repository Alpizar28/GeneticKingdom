#include "core/GameManager.h"
#include "ai/Pathfinding.h"
#include <iostream>
#include "Constantes.h"

GameManager::GameManager()
    : window(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT), "Genetic Kingdom") {
    window.setFramerateLimit(60);

    if (!font.loadFromFile(FONT_PATH)) {
        std::cerr << "❌ Error al cargar la fuente." << std::endl;
    }

    title.setFont(font);
    title.setString("Genetic Kingdom");
    title.setCharacterSize(36);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);
    float centerX = (window.getSize().x - title.getGlobalBounds().width) / 2;
    title.setPosition(centerX, 16);

    const float PANEL_WIDTH = 192.f;
    const float PANEL_X = GAME_WIDTH - PANEL_WIDTH;

    // 📷 Cargar fondo del sidebar
    if (!sidebarTex.loadFromFile("assets/images/sidebar_blur.png")) {
        std::cerr << "❌ Error al cargar fondo del sidebar\n";
    } else {
        sidebarSprite.setTexture(sidebarTex);
        sidebarSprite.setPosition(PANEL_X, 0);
        sidebarSprite.setScale(
            PANEL_WIDTH / sidebarTex.getSize().x,
            GAME_HEIGHT / sidebarTex.getSize().y
        );
    }

    // 🟫 Sombra superpuesta para mejorar contraste
    sidebarOverlay.setSize(sf::Vector2f(PANEL_WIDTH, GAME_HEIGHT));
    sidebarOverlay.setPosition(PANEL_X, 0);
    sidebarOverlay.setFillColor(sf::Color(0, 0, 0, 100)); // negro transparente

    // 🪙 Textos
    goldText.setFont(font);
    goldText.setCharacterSize(22);
    goldText.setFillColor(sf::Color::White);
    goldText.setString("Gold: 100");
    goldText.setPosition(PANEL_X + 16, 120);

    waveText.setFont(font);
    waveText.setCharacterSize(22);
    waveText.setFillColor(sf::Color::White);
    waveText.setString("Wave: 1 / 10");
    waveText.setPosition(PANEL_X + 16, 160);

    enemiesText.setFont(font);
    enemiesText.setCharacterSize(22);
    enemiesText.setFillColor(sf::Color::White);
    enemiesText.setString("Enemies: 5");
    enemiesText.setPosition(PANEL_X + 16, 200);

    // ▶️ Botón
    startWaveButton.setSize(sf::Vector2f(PANEL_WIDTH - 32, 48));
    startWaveButton.setPosition(PANEL_X + 16, 600);
    startWaveButton.setFillColor(sf::Color(70, 70, 70));

    startWaveText.setFont(font);
    startWaveText.setString("Start Wave");
    startWaveText.setCharacterSize(20);
    startWaveText.setFillColor(sf::Color::White);

    float textX = startWaveButton.getPosition().x + (startWaveButton.getSize().x - startWaveText.getLocalBounds().width) / 2;
    float textY = startWaveButton.getPosition().y + (startWaveButton.getSize().y - startWaveText.getLocalBounds().height) / 2 - 5;
    startWaveText.setPosition(textX, textY);

    // 🌄 Fondo del juego
    if (!backgroundTex.loadFromFile("assets/images/game_background.png")) {
        std::cerr << "❌ Error al cargar fondo del juego\n";
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

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                if (startWaveButton.getGlobalBounds().contains(mousePos)) {
                    std::cout << "🟢 Start Wave pressed!\n";

                    sf::Vector2i start = map.findLeftmostPathTile();
                    sf::Vector2i end = map.findRightmostPathTile();

                    Pathfinding pf(map);
                    auto path = pf.findPath(start, end);

                    if (path.empty()) {
                        std::cout << "⚠️ No se encontró camino...\n";
                    } else {
                        std::cout << "✅ Camino con " << path.size() << " pasos\n";
                        santa.setPath(path, Map::TILE_SIZE);
                    }
                }
            }
        }
    }
}

void GameManager::update() {
    float dt = deltaClock.restart().asSeconds();
    santa.update(dt);
}

void GameManager::render() {
    window.clear();
    window.draw(backgroundSprite);     
    map.draw(window);
    santa.draw(window);    
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
