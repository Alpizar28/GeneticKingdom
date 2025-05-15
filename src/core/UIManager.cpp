#include "UIManager.h"
#include "Constantes.h"
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <numeric>


UIManager::UIManager() {}

void UIManager::init(sf::Font& fontRef) {
    font = &fontRef;
    const float PW = 192, PX = GAME_WIDTH - PW;

    // Sidebar
    sidebarOverlay.setSize({PW,float(GAME_HEIGHT)});
    sidebarOverlay.setPosition(PX,0);
    sidebarOverlay.setFillColor({0,0,0,150});

    // Title
    title.setFont(*font);
    title.setString("Genetic Kingdom");
    title.setCharacterSize(36);
    title.setFillColor(sf::Color::White);
    title.setPosition((GAME_WIDTH-title.getGlobalBounds().width)/2,16);

    // Stats
    auto it = [&](sf::Text& t,int sz,float y){
        t.setFont(*font);
        t.setCharacterSize(sz);
        t.setFillColor(sf::Color::White);
        t.setPosition(PX+16,y);
    };
    it(goldText,18,50);
    it(waveText,18,80);
    it(genText,18,105);
    it(enemiesText,18,130);
    it(totalKillsText,16,155);   // << nueva línea (ajusta tamaño y Y si quieres)
    it(mutText,16,180);
    it(avgFitText,16,205);
    it(bestFitText,16,230);
    

    // Buttons
    auto ib = [&](Button& b,const char* lbl,float y){
        b.box.setSize({PW-32,30});
        b.box.setPosition(PX+16,y);
        b.box.setFillColor({70,70,70,200});
        b.label.setFont(*font);
        b.label.setCharacterSize(16);
        b.label.setString(lbl);
        b.label.setFillColor(sf::Color::White);
        b.label.setPosition(PX+24,y+5);
    };
    ib(fitnessBtn,"Historial",GAME_HEIGHT-175);
    ib(pauseBtn,  "Pausa",     GAME_HEIGHT-105);
    ib(restartBtn,"Reiniciar", GAME_HEIGHT-70);
    ib(exitBtn,   "Salir",     GAME_HEIGHT-35);

    // Tutorial
    tutorialBg.setSize({GAME_WIDTH*0.6f,GAME_HEIGHT*0.4f});
    tutorialBg.setFillColor({40,40,40,220});
    tutorialBg.setOutlineThickness(2);
    tutorialBg.setOutlineColor(sf::Color::White);
    tutorialBg.setPosition(GAME_WIDTH*0.2f,GAME_HEIGHT*0.3f);

    std::wstring tut =
        L"Bienvenido a Genetic Kingdom!\n\n"
        L"- Espera tu primera oleada.\n"
        L"- Coloca tus torres estratégicamente.\n\n"
        L"Presiona cualquier tecla para continuar.";
    tutorialText.setFont(*font);
    tutorialText.setCharacterSize(16);
    tutorialText.setFillColor(sf::Color::White);
    tutorialText.setString(sf::String(tut));
    tutorialText.setPosition(
        tutorialBg.getPosition().x+20,
        tutorialBg.getPosition().y+20
    );

    // Improve button + tower icons
    sf::Vector2f base(16.f,GAME_HEIGHT-80.f);
    float iconSize=48,sp=8;
    archerTexture.loadFromFile("assets/sprites/towers/btn_arquero.png");
    mageTexture   .loadFromFile("assets/sprites/towers/btn_mage.png");
    artilleryTexture.loadFromFile("assets/sprites/towers/btn_artillery.png");
    archerSprite.setTexture(archerTexture);
    mageSprite  .setTexture(mageTexture);
    artillerySprite.setTexture(artilleryTexture);
    archerSprite.setScale(iconSize/archerTexture.getSize().x,
                          iconSize/archerTexture.getSize().y);
    mageSprite  .setScale(iconSize/mageTexture.getSize().x,
                          iconSize/mageTexture.getSize().y);
    artillerySprite.setScale(iconSize/artilleryTexture.getSize().x,
                             iconSize/artilleryTexture.getSize().y);
    archerSprite.setPosition(base.x+160+sp,      base.y+20);
    mageSprite  .setPosition(base.x+160+sp+(iconSize+sp), base.y+20);
    artillerySprite.setPosition(
        base.x+160+sp+2*(iconSize+sp), base.y+20
    );
}

void UIManager::render(sf::RenderWindow& w, const UIState& s) {
    w.draw(sidebarOverlay);
    w.draw(title);

    goldText.setString("Gold: " + std::to_string(s.gold));
    waveText.setString("Oleada: " + std::to_string(s.currentWave) + "/" + std::to_string(s.totalWaves));
    genText.setString("Gen: " + std::to_string(s.currentGen));
    enemiesText.setString("Restantes: " + std::to_string(s.enemiesCount));
    totalKillsText.setString("Muertos totales: " + std::to_string(s.totalKills));  // << nuevo

    mutText.setString("Mut: " + std::to_string(int(s.mutationRate * 100)) + "%");
    avgFitText.setString("Fit Prom: " + std::to_string(int(s.avgFitness)));
    bestFitText.setString("Mejor Fit: " + std::to_string(int(s.bestFitness)));

    w.draw(goldText);
    w.draw(waveText);
    w.draw(genText);
    w.draw(enemiesText);
    w.draw(totalKillsText);    // << nuevo
    w.draw(mutText);
    w.draw(avgFitText);
    w.draw(bestFitText);

    w.draw(fitnessBtn.box);   w.draw(fitnessBtn.label);
    w.draw(pauseBtn.box);     w.draw(pauseBtn.label);
    w.draw(restartBtn.box);   w.draw(restartBtn.label);
    w.draw(exitBtn.box);      w.draw(exitBtn.label);

    // Mejora + iconos
    drawUpgradeButton(w,s.upgradeLevel,s.nextUpgradeCost,s.upgradeAvailable);
    w.draw(archerSprite);
    w.draw(mageSprite);
    w.draw(artillerySprite);

    // Hover tooltip...
    if(s.hoveredTower){
        std::vector<std::wstring> lines = {
            L"Nivel: "+std::to_wstring(s.hoveredTower->getUpgradeLevel()),
            L"Daño: "+std::to_wstring(int(s.hoveredTower->getDamage())),
            L"Alcance: "+std::to_wstring(int(s.hoveredTower->getRange())),
            L"Cooldown: "+std::to_wstring(int(s.hoveredTower->getCooldown()))+L"s"
        };
        float x=16,y=GAME_HEIGHT-80-lines.size()*18-8;
        float wmax=0;
        for(auto&str:lines){
            sf::Text t(str,*font,14);
            wmax=std::max(wmax,t.getLocalBounds().width);
        }
        wmax+=8; float htot=lines.size()*18+8;
        sf::RectangleShape bg({wmax,htot});
        bg.setFillColor({0,0,0,180});
        bg.setPosition(x,y);
        w.draw(bg);
        for(size_t i=0;i<lines.size();++i){
            sf::Text t(lines[i],*font,14);
            t.setFillColor(sf::Color::White);
            t.setPosition(x+4,y+4+i*18);
            w.draw(t);
        }
    }

    if(s.showTutorial){
        w.draw(tutorialBg);
        w.draw(tutorialText);
    }

    // Fitness dropdown
    if(showFitnessDropdown){
        sf::Vector2f bp=fitnessBtn.box.getPosition();
        float btnW=fitnessBtn.box.getSize().x,lh=16;
        size_t n=s.fitnessHistory.size();
        float boxH=(n+1)*lh+8,boxW=btnW;
        sf::RectangleShape bg({boxW,boxH});
        bg.setFillColor({0,0,0,200});
        bg.setPosition(bp.x,bp.y-boxH-4);
        w.draw(bg);
        sf::Text th("Historial Fit:",*font,14);
        th.setFillColor(sf::Color::White);
        th.setPosition(bp.x+4,bp.y-boxH+4);
        w.draw(th);
        for(size_t i=0;i<n;++i){
            std::ostringstream oss;
            oss<<"G"<<(i+1)<<": ";
            for(float f: s.fitnessHistory[i]) oss<<int(f)<<" ";
            sf::Text e(oss.str(),*font,12);
            e.setFillColor(sf::Color::White);
            e.setPosition(bp.x+4,bp.y-boxH+4+(i+1)*lh);
            w.draw(e);
        }
    }
}

void UIManager::renderGameOver(sf::RenderWindow& w,const UIState& s){
    // Overlay
    sf::RectangleShape ov({float(GAME_WIDTH),float(GAME_HEIGHT)});
    ov.setFillColor({0,0,0,200});
    w.draw(ov);
    // Title
    sf::Text t("Resumen final",*font,32);
    t.setFillColor(sf::Color::White);
    t.setPosition((GAME_WIDTH-t.getLocalBounds().width)/2,40);
    w.draw(t);
    // Tabla
    float x0=50,y0=100,lh=20;
    sf::Text h("Gen   Best   Avg",*font,18);
    h.setFillColor(sf::Color::Yellow);
    h.setPosition(x0,y0);
    w.draw(h);
    for(size_t i=0;i<s.fitnessHistory.size();++i){
        auto& gen=s.fitnessHistory[i];
        float best=*std::max_element(gen.begin(),gen.end());
        float avg= std::accumulate(gen.begin(),gen.end(),0.f)/gen.size();
        std::ostringstream oss;
        oss<<std::setw(3)<<(i+1)
           <<std::setw(8)<<int(best)
           <<std::setw(8)<<int(avg);
        sf::Text l(oss.str(),*font,16);
        l.setFillColor(sf::Color::White);
        l.setPosition(x0,y0+lh*(i+1));
        w.draw(l);
    }
    // Gráfica debajo
    float gx=x0,gy=y0+lh*(s.fitnessHistory.size()+2),GW=GAME_WIDTH-100-gx,GH=200;
    float gmax=0;
    for(auto&gen:s.fitnessHistory)
        gmax = std::max(gmax,*std::max_element(gen.begin(),gen.end()));
    sf::RectangleShape bgg({GW,GH});
    bgg.setFillColor({50,50,50,180});
    bgg.setPosition(gx,gy);
    w.draw(bgg);
    sf::VertexArray curve(sf::LinesStrip,s.fitnessHistory.size());
    for(size_t i=0;i<s.fitnessHistory.size();++i){
        float pv = *std::max_element(s.fitnessHistory[i].begin(),s.fitnessHistory[i].end());
        float px = gx + (i/float(s.fitnessHistory.size()-1))*GW;
        float py = gy + GH*(1.f - pv/gmax);
        curve[i] = sf::Vertex({px,py},sf::Color::Cyan);
    }
    w.draw(curve);
    // Info cierre
    sf::Text info("Cerrando en breve...",*font,14);
    info.setFillColor(sf::Color::White);
    info.setPosition(x0,gy+GH+20);
    w.draw(info);
}

void UIManager::handleMouseClick(const sf::Vector2f& m, bool gameOver){
    if (gameOver) {
        if (gameOverBtn.box.getGlobalBounds().contains(m)) {
            lastAction = Action::CloseGameOver;
            return;
        }
        lastAction = Action::None;
        return;
    }
    if(fitnessBtn.box.getGlobalBounds().contains(m)){
        showFitnessDropdown = !showFitnessDropdown;
        return;
    }
    if(pauseBtn.box.getGlobalBounds().contains(m)){
        lastAction=Action::Pause; return;
    }
    if(restartBtn.box.getGlobalBounds().contains(m)){
        lastAction=Action::Restart; return;
    }
    if(exitBtn.box.getGlobalBounds().contains(m)){
        lastAction=Action::Exit; return;
    }
    if(archerSprite.getGlobalBounds().contains(m)){
        selectedTower=TowerType::Archer; return;
    }
    if(mageSprite.getGlobalBounds().contains(m)){
        selectedTower=TowerType::Mage; return;
    }
    if(artillerySprite.getGlobalBounds().contains(m)){
        selectedTower=TowerType::Artillery; return;
    }
    lastAction=Action::None;
}

UIManager::Action UIManager::getAction()const{return lastAction;}
void UIManager::clearAction(){ lastAction=Action::None; }
TowerType UIManager::getSelectedTower()const{return selectedTower;}
void UIManager::setSelectedTowerType(TowerType t){selectedTower=t;}
sf::FloatRect UIManager::getUpgradeBtnBounds()const{
    return {16.f,GAME_HEIGHT-60,160,30};
}
void UIManager::drawUpgradeButton(sf::RenderWindow& w,int lvl,int cost,bool en){
    sf::Vector2f pos(16, GAME_HEIGHT-80);
    sf::Text label("Mejorar:",*font,16);
    label.setFillColor(sf::Color::White);
    label.setPosition(pos);
    w.draw(label);
    sf::RectangleShape btn({160,30});
    btn.setPosition(pos.x,pos.y+20);
    btn.setFillColor(en?sf::Color::Green:sf::Color(100,100,100));
    w.draw(btn);
    sf::Text t("Nivel "+std::to_string(lvl)+": $"+std::to_string(cost),*font,16);
    t.setFillColor(sf::Color::White);
    t.setPosition(btn.getPosition()+sf::Vector2f{8,5});
    w.draw(t);
}
