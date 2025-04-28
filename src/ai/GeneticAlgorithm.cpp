#include "GeneticAlgorithm.h"
#include "../map/Map.h"
#include "../enemies/EnemySanta.h"
#include "../enemies/EnemyZombie.h"
#include "../enemies/EnemyCaballero.h"
#include <random>

static std::mt19937_64 rng{std::random_device{}()};

GeneticAlgorithm::GeneticAlgorithm(int pop, int wSize, float mRate)
  : popSize(pop), waveSize(wSize), mutationRate(mRate)
{
    population.resize(popSize);
    std::uniform_real_distribution<float> dHp(  80.f,150.f),
                                        dSpd(  40.f,100.f),
                                        dType(   0.f,  1.f);
    for (auto& ind : population)
        ind.genes = { dHp(rng), dSpd(rng), dType(rng) };
}

std::vector<std::unique_ptr<Enemy>>
GeneticAlgorithm::createWave(const std::vector<sf::Vector2i>& path) {
    waveOrder.clear();
    std::vector<std::unique_ptr<Enemy>> wave;
    wave.reserve(waveSize);

    for (int i = 0; i < waveSize; ++i) {
        Individual* ind = &population[i % popSize];
        float t = ind->genes[2];
        std::unique_ptr<Enemy> e;
        if      (t < 0.33f) e = std::make_unique<EnemySanta>();
        else if (t < 0.66f) e = std::make_unique<EnemyZombie>();
        else                e = std::make_unique<EnemyCaballero>();

        e->setMaxHp(ind->genes[0]);
        e->setSpeed(ind->genes[1]);
        e->setPath(path, Map::TILE_SIZE);

        waveOrder.emplace_back(ind, e.get());
        wave.push_back(std::move(e));
    }
    return wave;
}

void GeneticAlgorithm::evolve() {
    // --- NO llames a evaluateFitness() aquí! ---
    // Fitness ya fue asignado desde GameManager al morir cada Enemy.
    std::sort(population.begin(), population.end(),
              [](auto& a, auto& b){ return a.fitness > b.fitness; });

    int elite = std::max(1, popSize / 10);
    std::vector<Individual> newPop(population.begin(),
                                   population.begin() + elite);
    std::uniform_int_distribution<int> dIdx(0, elite - 1);
    while ((int)newPop.size() < popSize) {
        auto child = crossover(population[dIdx(rng)],
                               population[dIdx(rng)]);
        mutate(child);
        newPop.push_back(std::move(child));
    }
    population.swap(newPop);
    ++currentGen;
}

Individual GeneticAlgorithm::crossover(const Individual& a,
                                       const Individual& b) {
    Individual child;
    child.genes.resize(a.genes.size());
    std::uniform_int_distribution<int> coin(0,1);
    for (size_t i = 0; i < a.genes.size(); ++i)
        child.genes[i] = coin(rng) ? a.genes[i] : b.genes[i];
    return child;
}

void GeneticAlgorithm::mutate(Individual& ind) {
    std::uniform_real_distribution<float> dMut(  0.f,1.f),
                                        dNoise(-5.f,5.f);
    for (auto& g : ind.genes) {
        if (dMut(rng) < mutationRate) {
            g += dNoise(rng);
            if (g < 0.f) g = 0.f;
        }
    }
}

int GeneticAlgorithm::getCurrentGeneration() const {
    return currentGen;
}

float GeneticAlgorithm::getAverageFitness() const {
    float sum = 0.f;
    for (auto& ind : population) sum += ind.fitness;
    return sum / popSize;
}

float GeneticAlgorithm::getBestFitness() const {
    float best = population.empty() ? 0.f : population[0].fitness;
    return best;
}

Individual* GeneticAlgorithm::getIndividualForEnemy(Enemy* e) {
    for (auto& pr : waveOrder)
        if (pr.second == e)
            return pr.first;
    return nullptr;
}
