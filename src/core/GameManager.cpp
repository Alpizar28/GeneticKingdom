// GameManager.cpp (completo, actualizado con Santa y Pathfinding)
#include "core/GameManager.h"
#include "ai/Pathfinding.h"
#include <iostream>

GameManager::GameManager()
    : window(sf::VideoMode(1280, 720), "Genetic Kingdom") {
    window.setFramerateLimit(60);

    if (!font.loadFromFile("assets/fonts/default.ttf")) {
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
    const float PANEL_X = 1280.f - PANEL_WIDTH;

    rightPanel.setSize(sf::Vector2f(PANEL_WIDTH, 720));
    rightPanel.setPosition(PANEL_X, 0);
    rightPanel.setFillColor(sf::Color(30, 30, 30));

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
                    Pathfinding pf(map);
                    sf::Vector2i end = map.findRightmostPathTile();  // ✅ NUEVO
                    
                    std::cout << "Start: " << start.x << ", " << start.y << "\n";
                    std::cout << "End:   " << end.x << ", " << end.y << "\n";
                    
                    
                    std::cout << "Start: " << start.x << ", " << start.y << "\n";
                    std::cout << "End:   " << end.x << ", " << end.y << "\n";
                    
                    // Intentar encontrar camino
                    auto path = pf.findPath(start, end);
                    
                    if (path.empty()) {
                        std::cout << "⚠️ Aún así no se encontró camino...\n";
                    } else {
                        std::cout << "✅ Camino encontrado con " << path.size() << " pasos\n";
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
    window.clear(sf::Color(20, 20, 20));
    window.draw(rightPanel);
    window.draw(title);
    map.draw(window);
    santa.draw(window);
    window.draw(goldText);
    window.draw(waveText);
    window.draw(enemiesText);
    window.draw(startWaveButton);
    window.draw(startWaveText);
    window.display();
} // Fin de GameManager.cpp