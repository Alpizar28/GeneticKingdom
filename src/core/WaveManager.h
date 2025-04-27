// src/core/WaveManager.h
#pragma once

#include <vector>
#include <memory>
#include <deque>

#include "../map/Map.h"
#include "../ai/GeneticAlgorithm.h"
#include "../enemies/Enemy.h"

class WaveManager {
public:
    // populationSize: tamaño de población GA
    // waveSize:      cuántos enemigos por ola
    // mutationRate:  tasa de mutación
    // totalWaves:    número total de olas
    // interval:      segundos entre olas
    WaveManager(int populationSize,
                int waveSize,
                float mutationRate,
                int totalWaves,
                float interval);

    // Llamado cada frame
    void update(float dt,
                Map& map,
                std::vector<std::unique_ptr<Enemy>>& enemies);

    // Consultas de estado
    int   getCurrentWave()    const;
    int   getTotalWaves()     const;
    bool  isWaveActive()      const;
    float getTimeToNext()     const;
    int   getCurrentGen()     const;
    float getAverageFitness() const;

private:
    GeneticAlgorithm                  ga;
    int                                currentWave;
    int                                totalWaves;
    bool                               waveActive;
    float                              waveInterval;
    float                              waveTimer;
    int                                waveSize;

    std::deque<std::unique_ptr<Enemy>> pending;        // cola de spawn
    float                              spawnInterval;  // tiempo entre spawns
    float                              spawnTimer;     // contador interno
};
