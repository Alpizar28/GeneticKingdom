#include "Tower.h"

constexpr int   Tower::upgradeCosts[];
constexpr float Tower::damageMults[];
constexpr float Tower::cooldownMults[];
constexpr float Tower::specialChanceAdd[];

Tower::Tower(const sf::Texture& tex, sf::Vector2f pos,
             float range_, int damage_)
  : sprite(tex), position(pos),
    range(range_), damage(damage_),
    cooldown(1.0f), specialChance(0.0f), cost(100)
{
    sprite.setOrigin(tex.getSize().x/2.f, tex.getSize().y/2.f);
    sprite.setPosition(position);
    atkTimer.restart();
}

/* -------------------- disparo -------------------- */
Enemy* Tower::tryFire(const std::vector<Enemy*>& enemies) {
    if (atkTimer.getElapsedTime().asSeconds() < cooldown)
        return nullptr;

    if (Enemy* tgt = findTarget(enemies)) {
        attack(tgt);
        atkTimer.restart();
        return tgt;
    }
    return nullptr;
}

/* -------------------- daño base ------------------ */
void Tower::attack(Enemy* tgt) {
    if (tgt) tgt->applyDamage(damage, getDamageType());
}

/* -------------------- util ----------------------- */
bool Tower::canAttack() const {
    return atkTimer.getElapsedTime().asSeconds() >= cooldown;
}

Enemy* Tower::findTarget(const std::vector<Enemy*>& enemies) const {
    Enemy* nearest = nullptr;
    float minSq = range * range;
    for (auto* e : enemies) {
        sf::Vector2f p = e->getSprite().getPosition();
        float dx = p.x - position.x, dy = p.y - position.y;
        float dsq = dx*dx + dy*dy;
        if (dsq <= minSq) {
            nearest = e;
            minSq   = dsq;
        }
    }
    return nearest;
}

void Tower::draw(sf::RenderWindow& w) const {
    w.draw(sprite);
}

/* -------------------- upgrades ------------------- */
bool Tower::upgrade(int& playerGold) {
    if (upgradeLevel >= MAX_UPGRADES) return false;
    int uc = upgradeCosts[upgradeLevel];
    if (playerGold < uc) return false;

    playerGold -= uc;
    damage         = int(damage * damageMults[upgradeLevel]);
    cooldown      *= cooldownMults[upgradeLevel];
    specialChance += specialChanceAdd[upgradeLevel];
    ++upgradeLevel;
    return true;
}

int Tower::getNextUpgradeCost() const {
    return (upgradeLevel < MAX_UPGRADES)
         ? upgradeCosts[upgradeLevel]
         : 0;
}
