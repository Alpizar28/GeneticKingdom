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

    std::vector<std::unique_ptr<Enemy>>
    createWave(const std::vector<sf::Vector2i>& path);

    void evolve();
    void resetFitness();

    /* getters ------------------------------------------------ */
    int   getCurrentGeneration() const;
    float getAverageFitness()    const;
    float getBestFitness()       const;
    Individual* getIndividualForEnemy(Enemy* e);

private:
    int   popSize;       
    float mutationRate;  
    int currentGen = 1;

    std::vector<Individual> population;
    std::vector<std::pair<Individual*, Enemy*>> waveOrder;

    Individual crossover(const Individual& a, const Individual& b);
    void mutate(Individual& ind);
};
