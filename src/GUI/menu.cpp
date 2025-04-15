#include "menu.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Constantes.h"

bool Menu::mostrar() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Genetic Kingdom - Menú");
    window.setFramerateLimit(TARGET_FPS);

    // Cargar fondo completo con botones incluidos
    sf::Texture backgroundTex;
    if (!backgroundTex.loadFromFile("assets/images/menu_full_background.png")) {
        std::cerr << "❌ Error al cargar fondo completo\n";
        return false;
    }

    sf::Sprite background(backgroundTex);
    background.setScale(
        float(WINDOW_WIDTH) / backgroundTex.getSize().x,
        float(WINDOW_HEIGHT) / backgroundTex.getSize().y
    );

    // Coordenadas aproximadas de los botones visuales
    const float buttonWidth = 380;
    const float buttonHeight = 80;
    const float centerX = WINDOW_WIDTH / 2 - buttonWidth / 2;

    sf::FloatRect jugarBtn(centerX, 450, buttonWidth, buttonHeight);
    sf::FloatRect opcionesBtn(centerX, 540, buttonWidth, buttonHeight);
    sf::FloatRect salirBtn(centerX, 630, buttonWidth, buttonHeight);

    bool iniciarJuego = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mouse(sf::Mouse::getPosition(window));

                if (jugarBtn.contains(mouse)) {
                    iniciarJuego = true;
                    window.close();
                } else if (salirBtn.contains(mouse)) {
                    window.close();
                } else if (opcionesBtn.contains(mouse)) {
                    std::cout << "⚙️ Menú de opciones (a implementar)\n";
                }
            }
        }

        window.clear();
        window.draw(background);
        window.display();
    }

    return iniciarJuego;
}
