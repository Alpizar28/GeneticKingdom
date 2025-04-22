#include "GeneticAlgorithm.h"
#include <random>
#include <algorithm>
#include "../map/Map.h"

GeneticAlgorithm::GeneticAlgorithm(int popSize, int waveSize, float mutationRate)
    : populationSize(popSize)
    , waveEnemyCount(waveSize)
    , mutationRate(mutationRate)
{}

void GeneticAlgorithm::initialize() {
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dist(0, numEnemyTypes - 1);
    population.clear();
    population.reserve(populationSize);
    for (int i = 0; i < populationSize; ++i) {
        Individual ind;
        ind.genome.resize(waveEnemyCount);
        for (int& g : ind.genome) g = dist(gen);
        population.push_back(ind);
    }
}

std::vector<std::unique_ptr<Enemy>> GeneticAlgorithm::createWave(const std::vector<sf::Vector2i>& path) {
    if (population.empty()) initialize();

    // Escoge el mejor o uno aleatorio en gen 0
    int idx = 0;
    if (currentGen > 0) {
        idx = std::distance(population.begin(),
            std::max_element(population.begin(), population.end(),
                [](auto& a, auto& b){ return a.fitness < b.fitness; }));
    } else {
        std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<> dist(0, populationSize - 1);
        idx = dist(gen);
    }

    auto& genome = population[idx].genome;
    std::vector<std::unique_ptr<Enemy>> wave;
    wave.reserve(waveEnemyCount);
    for (int g : genome) {
        std::unique_ptr<Enemy> e;
        switch (g) {
            case 0: e = std::make_unique<EnemySanta>();     break;
            case 1: e = std::make_unique<EnemyCaballero>(); break;
            case 2: e = std::make_unique<EnemyZombie>();    break;
            default: e = std::make_unique<EnemySanta>();    break;
        }
        e->setPath(path, Map::TILE_SIZE);
        wave.push_back(std::move(e));
    }
    return wave;
}

void GeneticAlgorithm::evaluate(const std::vector<std::unique_ptr<Enemy>>&) {
    // Aquí asigna .fitness según cuántos llegaron, tiempo, etc.
}

void GeneticAlgorithm::evolve() {
    std::vector<Individual> next;
    next.reserve(populationSize);
    for (int i = 0; i < populationSize; ++i) {
        auto a = tournamentSelection();
        auto b = tournamentSelection();
        auto child = crossover(a, b);
        mutate(child);
        next.push_back(std::move(child));
    }
    population = std::move(next);
    ++currentGen;
}

GeneticAlgorithm::Individual GeneticAlgorithm::tournamentSelection() {
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dist(0, populationSize - 1);
    Individual best = population[dist(gen)];
    for (int i = 0; i < 2; ++i) {
        auto cand = population[dist(gen)];
        if (cand.fitness > best.fitness) best = cand;
    }
    return best;
}

GeneticAlgorithm::Individual GeneticAlgorithm::crossover(const Individual& a, const Individual& b) {
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> px(0, waveEnemyCount - 1);
    int cp = px(gen);
    Individual c;
    c.genome.resize(waveEnemyCount);
    for (int i = 0; i < waveEnemyCount; ++i)
        c.genome[i] = (i < cp ? a.genome[i] : b.genome[i]);
    return c;
}

void GeneticAlgorithm::mutate(Individual& ind) {
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<> df(0,1);
    std::uniform_int_distribution<> di(0, numEnemyTypes - 1);
    for (int& g : ind.genome)
        if (df(gen) < mutationRate)
            g = di(gen);
}
