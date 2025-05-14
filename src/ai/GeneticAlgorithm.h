// src/ai/GeneticAlgorithm.h
#pragma once

#include <vector>
#include <memory>
#include <utility>
#include <algorithm>
#include <SFML/System/Vector2.hpp>
#include "../enemies/Enemy.h"

struct Individual {
    std::vector<float> genes;
    float fitness = 0.f;
};

class GeneticAlgorithm {
public:
    GeneticAlgorithm(int popSize, float mutationRate);

    // Crea la ola de enemigos usando la población actual
    std::vector<std::unique_ptr<Enemy>>
    createWave(const std::vector<sf::Vector2i>& path);

    // Evoluciona la población a la siguiente generación
    void evolve();
    // Reinicia los fitness a 0 antes de cada ola
    void resetFitness();

    /* getters ------------------------------------------------ */
    // Generación actual (1-based)
    int   getCurrentGeneration() const;
    // Promedio de fitness en la generación actual
    float getAverageFitness()    const;
    // Mejor fitness en la generación actual
    float getBestFitness()       const;
    // Historial completo: fitness de cada individuo por generación
    const std::vector<std::vector<float>>& getFitnessHistory() const;

    // Permite mapear un Enemy->su Individual para asignar fitness tras la ola
    Individual* getIndividualForEnemy(Enemy* e);

private:
    int   popSize;         // tamaño de población
    float mutationRate;    // probabilidad de mutación
    int   currentGen = 1;  // índice de generación

    std::vector<Individual> population;
    // Asociaciones temporales ola: Individual* ↔ Enemy*
    std::vector<std::pair<Individual*, Enemy*>> waveOrder;

    // ← Nuevo: historial de fitness
    std::vector<std::vector<float>> fitnessHistory;

    // Métodos internos de GA
    Individual crossover(const Individual& a, const Individual& b);
    void mutate(Individual& ind);
};
