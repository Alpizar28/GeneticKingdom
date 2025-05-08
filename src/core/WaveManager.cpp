#include "WaveManager.h"
#include "../ai/Pathfinding.h"

/* ---------- ctor ---------- */
WaveManager::WaveManager(int waveSize_,
                         float mutationRate_,
                         int totalWaves_,
                         float interval_)
  : waveSize(waveSize_)
  , mutationRate(mutationRate_)
  , ga(waveSize_, mutationRate_)    // GA con población fija
  , currentWave(1)
  , totalWaves(totalWaves_)
  , waveActive(false)
  , interval(interval_)
  , timer(0.f)
  , spawnInterval(0.5f)
  , spawnTimer(0.f)
{}

/* -------------------------------------------------- */
void WaveManager::update(float dt,
                         Map& map,
                         std::vector<std::unique_ptr<Enemy>>& enemies)
{
    timer += dt;

    /* 1) Iniciar ola --------------------------------------------------- */
    if (!waveActive
        && currentWave <= totalWaves
        && (currentWave == 1 || timer >= interval))
    {
        timer = 0.f;
        pending.clear();

        auto path = Pathfinding(map)
                      .findPath(map.findLeftmostPathTile(),
                                map.findRightmostPathTile());
        if (path.empty()) return;

        auto wave = ga.createWave(path);
        for (auto& e : wave) pending.push_back(std::move(e));

        waveActive = true;
        spawnTimer = 0.f;

        if (!pending.empty()) {
            enemies.push_back(std::move(pending.front()));
            pending.pop_front();
        }
    }

    /* 2) Spawn escalonado --------------------------------------------- */
    if (waveActive && !pending.empty()) {
        spawnTimer += dt;
        if (spawnTimer >= spawnInterval) {
            enemies.push_back(std::move(pending.front()));
            pending.pop_front();
            spawnTimer = 0.f;
        }
    }

    /* 3) Evolucionar y pasar a la siguiente --------------------------- */
    if (waveActive && pending.empty() && enemies.empty()) {
        ga.evolve();
        ++currentWave;
        waveActive = false;
    }
}
