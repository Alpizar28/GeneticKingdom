#include <iostream>
#include "map/Map.h"
#include "towers/Tower.h"
#include "enemies/Enemy.h"
#include "core/GameManager.h"
#include "ai/GeneticAlgorithm.h"
#include "ai/Pathfinding.h"
#include "GUI/menu.h"

int main() {
    Menu menu;
    if (menu.mostrar()) {
        GameManager game;
        game.run();
    }
    return 0;
}


