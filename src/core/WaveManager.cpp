// src/core/WaveManager.cpp
#include "WaveManager.h"
#include "../ai/Pathfinding.h"

WaveManager::WaveManager(int popSize,
                         int wSize,
                         float mutRate,
                         int tWaves,
                         float interval)
  : ga(popSize, wSize, mutRate)
  , currentWave(1)
  , totalWaves(tWaves)
  , waveActive(false)
  , waveInterval(interval)
  , waveTimer(0.f)
  , waveSize(wSize)
  , spawnInterval(0.7f)  // Ajusta a tu gusto (segundos entre enemigos)
  , spawnTimer(0.f)
{}

void WaveManager::update(float dt,
                         Map& map,
                         std::vector<std::unique_ptr<Enemy>>& enemies)
{
    waveTimer += dt;

    // 1) Si no hay ola activa y toca iniciar, generamos toda la ola en 'pending'
    if (!waveActive
        && currentWave <= totalWaves
        && (currentWave == 1 || waveTimer >= waveInterval))
    {
        waveTimer = 0.f;
        pending.clear();

        // Calculamos ruta
        Pathfinding pf(map);
        auto start = map.findLeftmostPathTile();
        auto end   = map.findRightmostPathTile();
        auto path  = pf.findPath(start, end);

        if (!path.empty()) {
            // Creamos todos los enemigos y los encolamos
            auto fullWave = ga.createWave(path);
            for (auto& e : fullWave)
                pending.push_back(std::move(e));

            waveActive = true;
            spawnTimer = 0.f;

            // Spawn inmediato del primer enemigo
            enemies.push_back(std::move(pending.front()));
            pending.pop_front();
        }
    }

    // 2) Si la ola está activa y aún quedan 'pending', espaciamos el resto
    if (waveActive && !pending.empty()) {
        spawnTimer += dt;
        if (spawnTimer >= spawnInterval) {
            enemies.push_back(std::move(pending.front()));
            pending.pop_front();
            spawnTimer = 0.f;
        }
    }

    // 3) Si la ola terminó (ni pending ni vivos), evolucionamos y pasamos a la siguiente ola
    if (waveActive && pending.empty() && enemies.empty()) {
        ga.evolve();
        ++currentWave;
        waveActive = false;
    }
}

int   WaveManager::getCurrentWave()    const { return currentWave; }
int   WaveManager::getTotalWaves()     const { return totalWaves; }
bool  WaveManager::isWaveActive()      const { return waveActive; }
float WaveManager::getTimeToNext()     const {
    float rem = waveInterval - waveTimer;
    return rem > 0.f ? rem : 0.f;
}
int   WaveManager::getCurrentGen()     const { return ga.getCurrentGen(); }
float WaveManager::getAverageFitness() const { return ga.getAverageFitness(); }
