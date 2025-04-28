#include "WaveManager.h"
#include "../ai/Pathfinding.h"

WaveManager::WaveManager(int initialWaveSize_,
                         int waveIncrement_,
                         float mutationRate_,
                         int totalWaves_,
                         float interval_)
  : initialWaveSize(initialWaveSize_)
  , waveIncrement(waveIncrement_)
  , mutationRate(mutationRate_)
  , ga(initialWaveSize_, initialWaveSize_, mutationRate_)
  , currentWave(1)
  , totalWaves(totalWaves_)
  , waveActive(false)
  , interval(interval_)
  , timer(0.f)
  , spawnInterval(0.5f)
  , spawnTimer(0.f)
{}

int WaveManager::getCurrentWave()    const { return currentWave; }
int WaveManager::getTotalWaves()     const { return totalWaves; }
float WaveManager::getTimeToNext()   const {
    float rem = interval - timer;
    return rem > 0.f ? rem : 0.f;
}
int WaveManager::getCurrentGen()     const { return ga.getCurrentGeneration(); }
float WaveManager::getAverageFitness() const { return ga.getAverageFitness(); }
float WaveManager::getBestFitness()  const { return ga.getBestFitness(); }

void WaveManager::update(float dt,
                         Map& map,
                         std::vector<std::unique_ptr<Enemy>>& enemies)
{
    timer += dt;
    // 1) Iniciar ola
    if (!waveActive
        && currentWave <= totalWaves
        && (currentWave == 1 || timer >= interval))
    {
        timer = 0.f;
        pending.clear();
        int waveSize = initialWaveSize + (currentWave - 1) * waveIncrement;
        ga = GeneticAlgorithm(waveSize, waveSize, mutationRate);

        auto path = Pathfinding(map)
                      .findPath(map.findLeftmostPathTile(),
                                map.findRightmostPathTile());
        if (path.empty()) return;

        auto wave = ga.createWave(path);
        for (auto& e : wave)
            pending.push_back(std::move(e));

        waveActive = true;
        spawnTimer = 0.f;
        if (!pending.empty()) {
            enemies.push_back(std::move(pending.front()));
            pending.pop_front();
        }
    }

    // 2) Spawn espaciado
    if (waveActive && !pending.empty()) {
        spawnTimer += dt;
        if (spawnTimer >= spawnInterval) {
            enemies.push_back(std::move(pending.front()));
            pending.pop_front();
            spawnTimer = 0.f;
        }
    }

    // 3) Cuando todos murieron/escaparon evolucionar
    if (waveActive && pending.empty() && enemies.empty()) {
        ga.evolve();
        ++currentWave;
        waveActive = false;
    }
}
