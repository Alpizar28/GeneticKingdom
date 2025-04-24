#include "InputManager.h"

InputState InputManager::processEvent(const sf::Event& e, bool showTutorial) {
    InputState st;
    if (e.type == sf::Event::Closed) {
        st.requestExit = true;
    }
    if (showTutorial && e.type == sf::Event::KeyPressed) {
        st.advanceTutorial = true;
    }
    if (!showTutorial && e.type == sf::Event::KeyPressed) {
        if (e.key.code == sf::Keyboard::F1)
            st.toggleDebug = true;
    }
    return st;
}
