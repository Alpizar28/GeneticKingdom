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
    GeneticAlgorithm(int popSize, int waveSize, float mutationRate);

    // Genera la ola y guarda en waveOrder el mapping (Individual*, Enemy*)
    std::vector<std::unique_ptr<Enemy>>
    createWave(const std::vector<sf::Vector2i>& path);

    // Cruce+mutación, usando fitness ya asignado externamente
    void evolve();

    // Getters para UI / GameManager
    int   getCurrentGeneration() const;
    float getAverageFitness()    const;
    float getBestFitness()       const;

    // Permite al GameManager asignar fitness cuando muere cada enemigo
    Individual* getIndividualForEnemy(Enemy* e);

private:
    int popSize;
    int waveSize;
    int currentGen = 1;
    float mutationRate;

    std::vector<Individual> population;
    std::vector<std::pair<Individual*, Enemy*>> waveOrder;

    Individual crossover(const Individual& a, const Individual& b);
    void mutate(Individual& ind);
    // Ya no necesitamos evaluateFitness aquí
};
