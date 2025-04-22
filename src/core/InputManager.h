#pragma once
#include <SFML/Window.hpp>

struct InputState {
    bool requestExit     = false;
    bool toggleDebug     = false;
    bool advanceTutorial = false;
};

class InputManager {
public:
    // Procesa UN evento y devuelve el pequeño estado de teclado/viento
    InputState processEvent(const sf::Event& e, bool showTutorial);
};
