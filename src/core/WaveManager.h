#pragma once

#include <vector>
#include <deque>
#include <memory>
#include "../map/Map.h"
#include "../ai/GeneticAlgorithm.h"
#include "../enemies/Enemy.h"

class WaveManager {
public:
    WaveManager(int   waveSize,
                float mutationRate,
                int   totalWaves,
                float interval);

    void update(float dt,
                Map& map,
                std::vector<std::unique_ptr<Enemy>>& enemies);

    /* ------ Consultas para GameManager / UI ---------------- */
    int   getCurrentWave()    const { return currentWave; }
    int   getTotalWaves()     const { return totalWaves;  }
    float getTimeToNext()     const { return interval - timer; }
    int   getCurrentGen()     const { return ga.getCurrentGeneration(); }
    float getAverageFitness() const { return ga.getAverageFitness(); }
    float getBestFitness()    const { return ga.getBestFitness(); }

    /* Forward: GameManager sigue usando esto ---------------- */
    Individual* getIndividualForEnemy(Enemy* e)
        { return ga.getIndividualForEnemy(e); }

private:
    /* parámetros fijos */
    int   waveSize;
    float mutationRate;

    GeneticAlgorithm ga;

    /* runtime */
    int   currentWave;
    int   totalWaves;
    bool  waveActive;
    float interval;
    float timer;

    /* spawn */
    float spawnInterval;
    float spawnTimer;

    std::deque<std::unique_ptr<Enemy>> pending;
};
