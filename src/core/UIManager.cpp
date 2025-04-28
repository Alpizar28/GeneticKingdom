#include "UIManager.h"
#include "Constantes.h"
#include <string>

void UIManager::init(sf::Font& font){
    const float PW=192,PX=GAME_WIDTH-PW;
    sidebarOverlay.setSize({PW,float(GAME_HEIGHT)});
    sidebarOverlay.setPosition(PX,0);
    sidebarOverlay.setFillColor({0,0,0,150});

    title.setFont(font);
    title.setString("Genetic Kingdom");
    title.setCharacterSize(36);
    title.setFillColor(sf::Color::White);
    title.setPosition((GAME_WIDTH-title.getGlobalBounds().width)/2,16);

    auto mk=[&](sf::Text& t,float y,int sz=18){
        t.setFont(font);
        t.setCharacterSize(sz);
        t.setFillColor(sf::Color::White);
        t.setPosition(PX+16,y);
    };
    mk(goldText,80);
    mk(waveText,120);
    mk(genText,160,16);
    mk(avgFitText,185,16);
    mk(bestFitText,205,16);
    mk(enemiesText,240);
    mk(timerText,280);
    mk(debugText,GAME_HEIGHT-80,14);

    auto mb=[&](Button& b,float y,const std::string& l){
        b.box.setSize({PW-32,40});
        b.box.setPosition(PX+16,y);
        b.box.setFillColor({70,70,70,200});
        b.label.setFont(font);
        b.label.setString(l);
        b.label.setCharacterSize(18);
        b.label.setFillColor(sf::Color::White);
        b.label.setPosition(
           b.box.getPosition().x + (b.box.getSize().x - b.label.getGlobalBounds().width)/2,
           b.box.getPosition().y + (b.box.getSize().y - b.label.getGlobalBounds().height)/2
        );
    };
    mb(pauseBtn,320,"Pausa");
    mb(restartBtn,370,"Reiniciar");
    mb(exitBtn,420,"Salir");

    tutorialBg.setSize({GAME_WIDTH*0.6f,GAME_HEIGHT*0.4f});
    tutorialBg.setFillColor({40,40,40,220});
    tutorialBg.setOutlineColor({255,255,255,180});
    tutorialBg.setOutlineThickness(2);
    tutorialBg.setPosition((GAME_WIDTH-tutorialBg.getSize().x)/2,
                           (GAME_HEIGHT-tutorialBg.getSize().y)/2);
    const std::string tut = u8R"(Bienvenido a Genetic Kingdom!

- Espera tu 1ª oleada.
- Luego coloca tus torres.

Presiona cualquier tecla para continuar.
)";
    tutorialText.setFont(font);
    tutorialText.setString(sf::String::fromUtf8(tut.begin(),tut.end()));
    tutorialText.setCharacterSize(16);
    tutorialText.setFillColor(sf::Color::White);
    tutorialText.setPosition(
      tutorialBg.getPosition().x+20,
      tutorialBg.getPosition().y+20
    );
}

void UIManager::render(sf::RenderWindow& w, const UIState& s){
    w.draw(sidebarOverlay);
    w.draw(title);

    goldText.setString("Gold: "+std::to_string(s.gold));          w.draw(goldText);
    waveText.setString("Wave: "+std::to_string(s.currentWave)
                 +"/"+std::to_string(s.totalWaves));             w.draw(waveText);
    genText.setString("Gen: "+std::to_string(s.currentGen));      w.draw(genText);
    avgFitText.setString("AvgFit: "+std::to_string(int(s.avgFitness)));w.draw(avgFitText);
    bestFitText.setString("BestFit: "+std::to_string(int(s.bestFitness)));w.draw(bestFitText);
    enemiesText.setString("Enemies: "+std::to_string(s.enemiesCount));   w.draw(enemiesText);
    timerText.setString("Next: "+std::to_string(int(s.timeToNext))+"s");  w.draw(timerText);

    w.draw(pauseBtn.box);   w.draw(pauseBtn.label);
    w.draw(restartBtn.box); w.draw(restartBtn.label);
    w.draw(exitBtn.box);    w.draw(exitBtn.label);

    if(s.showTutorial){
        sf::RectangleShape sh= tutorialBg; sh.move(5,5);
        sh.setFillColor({0,0,0,120});
        w.draw(sh);
        w.draw(tutorialBg);
        w.draw(tutorialText);
    }
    if(s.debugMode){
        debugText.setString("DEBUG\nPaused="+std::to_string(s.paused)
                         +"\nWave="+std::to_string(s.currentWave));
        w.draw(debugText);
    }
}

sf::FloatRect UIManager::getPauseBtnBounds()   const { return pauseBtn.box.getGlobalBounds(); }
sf::FloatRect UIManager::getRestartBtnBounds() const { return restartBtn.box.getGlobalBounds(); }
sf::FloatRect UIManager::getExitBtnBounds()    const { return exitBtn.box.getGlobalBounds(); }
bool UIManager::isMouseOverUI(const sf::Vector2f& m) const {
    return getPauseBtnBounds().contains(m)
        || getRestartBtnBounds().contains(m)
        || getExitBtnBounds().contains(m)
        || sidebarOverlay.getGlobalBounds().contains(m);
}
