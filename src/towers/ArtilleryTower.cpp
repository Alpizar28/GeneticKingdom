#include "ArtilleryTower.h"
#include <cstdlib>
#include <cmath>

ArtilleryTower::ArtilleryTower(sf::Vector2f pos, const sf::Texture& tex)
    : Tower(tex, pos, 300.f, 50)        // daño alto, alcance bajo
{
    cooldown       = 3.0f;
    specialChance  = 0.15f;             // 15 % splash
    cost           = 300;
}

void ArtilleryTower::attack(Enemy* target) {
    /* daño directo */
    Tower::attack(target);

    /* splash (radio 60 px) */
    if (!target) return;
    if ((rand() / float(RAND_MAX)) < specialChance) {
        sf::Vector2f center = target->getSprite().getPosition();

        /* itera por los enemigos cercanos (esto lo harás en GameManager
           o guarda un vector<Enemy*> en esta clase) */
        // Pseudocódigo: for (auto* e : enemiesInScene) …
        // e->applyDamage(damage*0.4f, DamageType::Artillery);
    }
}
