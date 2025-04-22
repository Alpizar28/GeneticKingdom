// src/core/WaveManager.h
#pragma once
#include <vector>
#include <memory>
#include "../map/Map.h"
#include "../ai/GeneticAlgorithm.h"
#include "../enemies/Enemy.h"

class WaveManager {
public:
    WaveManager(int populationSize, int waveSize, float mutationRate, int totalWaves, float interval);

    // Se llama cada frame
    void update(float dt, Map& map, std::vector<std::unique_ptr<Enemy>>& enemies);

    // Fuerza el inicio inmediato de la ola (sin esperar el intervalo)
    void forceStartWave(Map& map, std::vector<std::unique_ptr<Enemy>>& enemies);

    // Consultas de estado
    int  getCurrentWave()   const;
    int  getTotalWaves()    const;
    bool isWaveActive()     const;

    float getTimeToNext()       const;
    int   getCurrentGen()       const;
    float getAverageFitness()   const;

private:
    GeneticAlgorithm      ga;
    int                   currentWave;
    int                   totalWaves;
    bool                  waveActive;
    float                 waveInterval;
    float                 waveTimer;
    int                   waveSize;
};
