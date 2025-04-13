#!/bin/bash

echo "🔧 Creando clases base del proyecto Genetic Kingdom..."

# Crear archivos con contenido básico
declare -A modules=(
    ["map/Map"]="class Map { public: Map(); void load(); };"
    ["towers/Tower"]="class Tower { public: Tower(); };"
    ["enemies/Enemy"]="class Enemy { public: Enemy(); };"
    ["core/Game"]="class Game { public: Game(); void start(); };"
    ["ai/GeneticAlgorithm"]="class GeneticAlgorithm { public: GeneticAlgorithm(); };"
    ["ai/Pathfinding"]="class Pathfinding { public: Pathfinding(); };"
)

for path in "${!modules[@]}"; do
    header="src/$path.h"
    source="src/$path.cpp"
    classdef="${modules[$path]}"
    classname=$(basename "$path")

    echo "#pragma once

$classdef" > "$header"

    echo -e "#include "$classname.h"
#include <iostream>

$classdef
{
    std::cout << "✅ $classname creado" << std::endl;
}" > "$source"
done

# Crear contenido real para métodos específicos
echo -e "#include "Map.h"
#include <iostream>

Map::Map() { std::cout << "✅ Mapa creado" << std::endl; }
void Map::load() { std::cout << "📦 Cargando mapa..." << std::endl; }" > src/map/Map.cpp
echo -e "#include "Tower.h"
#include <iostream>

Tower::Tower() { std::cout << "🏹 Torre creada" << std::endl; }" > src/towers/Tower.cpp
echo -e "#include "Enemy.h"
#include <iostream>

Enemy::Enemy() { std::cout << "👾 Enemigo creado" << std::endl; }" > src/enemies/Enemy.cpp
echo -e "#include "Game.h"
#include <iostream>

Game::Game() { std::cout << "🎮 Juego inicializado" << std::endl; }
void Game::start() { std::cout << "▶️ Empezando juego..." << std::endl; }" > src/core/Game.cpp
echo -e "#include "GeneticAlgorithm.h"
#include <iostream>

GeneticAlgorithm::GeneticAlgorithm() { std::cout << "🧬 Algoritmo genético cargado" << std::endl; }" > src/ai/GeneticAlgorithm.cpp
echo -e "#include "Pathfinding.h"
#include <iostream>

Pathfinding::Pathfinding() { std::cout << "🧭 Pathfinding activado" << std::endl; }" > src/ai/Pathfinding.cpp

echo "✅ Archivos creados con éxito."