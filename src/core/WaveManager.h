#pragma once

#include <vector>
#include <deque>
#include <memory>
#include "../map/Map.h"
#include "../ai/GeneticAlgorithm.h"
#include "../enemies/Enemy.h"

class WaveManager {
public:
    WaveManager(int initialWaveSize,
                int waveIncrement,
                float mutationRate,
                int totalWaves,
                float interval);

    void update(float dt,
                Map& map,
                std::vector<std::unique_ptr<Enemy>>& enemies);

    Individual* getIndividualForEnemy(Enemy* e) {
        return ga.getIndividualForEnemy(e);
    }

    // Consultas UI
    int   getCurrentWave()    const;
    int   getTotalWaves()     const;
    float getTimeToNext()     const;
    int   getCurrentGen()     const;
    float getAverageFitness() const;
    float getBestFitness()    const;
    

private:
    int                                   initialWaveSize;
    int                                   waveIncrement;
    float                                 mutationRate;
    GeneticAlgorithm                      ga;
    int                                   currentWave;
    int                                   totalWaves;
    bool                                  waveActive;
    float                                 interval;
    float                                 timer;
    std::deque<std::unique_ptr<Enemy>>    pending;
    float                                 spawnInterval;
    float                                 spawnTimer;
};
