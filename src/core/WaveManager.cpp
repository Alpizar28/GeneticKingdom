// src/core/WaveManager.cpp

#include "WaveManager.h"
#include "../ai/Pathfinding.h"

WaveManager::WaveManager(int popSize, int wSize, float mutRate, int tWaves, float interval)
  : ga(popSize, wSize, mutRate)
  , currentWave(1)
  , totalWaves(tWaves)
  , waveActive(false)
  , waveInterval(interval)
  , waveTimer(0.f)
  , waveSize(wSize)
{}

void WaveManager::update(float dt,
                         Map& map,
                         std::vector<std::unique_ptr<Enemy>>& enemies)
{
    waveTimer += dt;

    // Genera nueva ola si ha pasado el intervalo o es la primera
    if (!waveActive
        && (currentWave == 1 || waveTimer >= waveInterval)
        && currentWave <= totalWaves)
    {
        waveTimer = 0.f;
        Pathfinding pf(map);
        auto start = map.findLeftmostPathTile();
        auto end   = map.findRightmostPathTile();
        auto path  = pf.findPath(start, end);
        if (!path.empty()) {
            enemies    = ga.createWave(path);
            waveActive = true;
        }
    }

    // Cuando todos los enemigos mueren, evolucionar a la siguiente generación
    if (waveActive && enemies.empty()) {
        ga.evolve();
        ++currentWave;
        waveActive = false;
    }
}

void WaveManager::forceStartWave(Map& map,
                                 std::vector<std::unique_ptr<Enemy>>& enemies)
{
    // Fuerza el inicio de una ola actual (sin esperar intervalo)
    if (!waveActive && currentWave <= totalWaves) {
        Pathfinding pf(map);
        auto start = map.findLeftmostPathTile();
        auto end   = map.findRightmostPathTile();
        auto path  = pf.findPath(start, end);
        if (!path.empty()) {
            enemies    = ga.createWave(path);
            waveActive = true;
            waveTimer  = 0.f;
        }
    }
}

int WaveManager::getCurrentWave() const {
    return currentWave;
}

int WaveManager::getTotalWaves() const {
    return totalWaves;
}

bool WaveManager::isWaveActive() const {
    return waveActive;
}

float WaveManager::getTimeToNext() const {
    float rem = waveInterval - waveTimer;
    return rem > 0.f ? rem : 0.f;
}

int WaveManager::getCurrentGen() const {
    return ga.getCurrentGen();
}

float WaveManager::getAverageFitness() const {
    return ga.getAverageFitness();
}
