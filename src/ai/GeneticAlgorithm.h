#pragma once
#include <vector>
#include <memory>
#include "../enemies/Enemy.h"
#include "../enemies/EnemySanta.h"
#include "../enemies/EnemyCaballero.h"
#include "../enemies/EnemyZombie.h"
#include <SFML/System/Vector2.hpp>

class GeneticAlgorithm {
public:
    struct Individual {
        std::vector<int> genome;   // 0=Santa, 1=Caballero, 2=Zombie
        float fitness = 0.f;
    };

    GeneticAlgorithm(int popSize = 20,
                     int waveSize = 5,
                     float mutationRate = 0.1f);

    void initialize();
    std::vector<std::unique_ptr<Enemy>> createWave(const std::vector<sf::Vector2i>& path);
    void evaluate(const std::vector<std::unique_ptr<Enemy>>& wave);
    void evolve();
        int getCurrentGen() const { return currentGen; }
        float getAverageFitness() const {
            if (population.empty()) return 0.f;
            float sum = 0.f;
            for (auto& ind : population) sum += ind.fitness;
            return sum / population.size();
        }
    

private:
    Individual tournamentSelection();
    Individual crossover(const Individual& a, const Individual& b);
    void mutate(Individual& ind);

    int populationSize;
    int waveEnemyCount;
    int numEnemyTypes = 3;
    float mutationRate;
    int currentGen = 0;
    std::vector<Individual> population;
};
