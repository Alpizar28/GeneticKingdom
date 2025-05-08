// src/towers/Tower.cpp
#include "Tower.h"

constexpr int   Tower::upgradeCosts    [MAX_UPGRADES];
constexpr float Tower::damageMults     [MAX_UPGRADES];
constexpr float Tower::cooldownMults   [MAX_UPGRADES];
constexpr float Tower::specialChanceAdd[MAX_UPGRADES];

Tower::Tower(const sf::Texture& tex, sf::Vector2f pos, float range, int damage)
  : sprite(tex), position(pos),
    range(range), damage(damage),
    cooldown(1.0f), specialChance(0.0f), cost(100)
{
    sprite.setOrigin(tex.getSize().x/2.f, tex.getSize().y/2.f);
    sprite.setPosition(position);
    attackCooldown.restart();
}

Enemy* Tower::tryFire(const std::vector<Enemy*>& enemies) {
    if (attackCooldown.getElapsedTime().asSeconds() < cooldown)
        return nullptr;
    if (Enemy* tgt = findTarget(enemies)) {
        attack(tgt);
        attackCooldown.restart();
        return tgt;
    }
    return nullptr;
}

void Tower::attack(Enemy* target) {
    if (target) target->takeDamage(damage);
}

bool Tower::canAttack() const {
    return attackCooldown.getElapsedTime().asSeconds() >= cooldown;
}

Enemy* Tower::findTarget(const std::vector<Enemy*>& enemies) const {
    Enemy* nearest = nullptr;
    float  minSq = range*range;
    for (auto* e : enemies) {
        auto p = e->getSprite().getPosition();
        float dx = p.x-position.x, dy = p.y-position.y;
        float dsq = dx*dx+dy*dy;
        if (dsq<=minSq) {
            nearest = e;
            minSq = dsq;
        }
    }
    return nearest;
}

void Tower::draw(sf::RenderWindow& w) const {
    w.draw(sprite);
}

bool Tower::upgrade(int& playerGold) {
    if (upgradeLevel>=MAX_UPGRADES) return false;
    int uc = upgradeCosts[upgradeLevel];
    if (playerGold < uc)          return false;
    playerGold -= uc;

    // aplicar multiplicadores
    damage        = int(damage * damageMults[upgradeLevel]);
    cooldown     *= cooldownMults[upgradeLevel];
    specialChance+= specialChanceAdd[upgradeLevel];

    ++upgradeLevel;
    return true;
}

int Tower::getNextUpgradeCost() const {
    return (upgradeLevel<MAX_UPGRADES
            ? upgradeCosts[upgradeLevel]
            : 0);
}
