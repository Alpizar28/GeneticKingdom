#include "menu.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Constantes.h"

void Menu::mostrar() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Genetic Kingdom - Menu");
    window.setFramerateLimit(TARGET_FPS);

    sf::Font font;
    if (!font.loadFromFile(FONT_PATH)) {
        std::cerr << "Error: no se pudo cargar la fuente." << std::endl;
        return;
    }

    sf::Text title("Genetic Kingdom", font, 60);
    title.setPosition(WINDOW_WIDTH / 2 - title.getGlobalBounds().width / 2, 150);
    title.setFillColor(sf::Color::White);

    sf::Text startText("Presiona ENTER para comenzar", font, 30);
    startText.setPosition(WINDOW_WIDTH / 2 - startText.getGlobalBounds().width / 2, 350);
    startText.setFillColor(sf::Color::Green);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                std::cout << "Iniciando el juego..." << std::endl;
                window.close();
            }
        }

        window.clear(sf::Color(30, 30, 30));
        window.draw(title);
        window.draw(startText);
        window.display();
    }
}
