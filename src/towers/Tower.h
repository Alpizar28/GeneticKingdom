#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "../enemies/Enemy.h"          // DamageType y Enemy
#include <SFML/System/Clock.hpp>

enum class TowerType { None, Archer, Mage, Artillery };

class Tower {
public:
    static constexpr int MAX_UPGRADES = 3;

    Tower(const sf::Texture& tex, sf::Vector2f pos,
          float range, int damage);
    virtual ~Tower() = default;

    /* ciclo principal de disparo */
    Enemy* tryFire(const std::vector<Enemy*>& enemies);

    void draw(sf::RenderWindow& w) const;

    /* upgrades */
    bool upgrade(int& playerGold);
    int  getNextUpgradeCost() const;
    int  getUpgradeLevel()    const { return upgradeLevel; }

    /* misc */
    sf::Vector2f getPosition() const { return position; }
    int          getDamage()   const { return damage; }
    int          getCost()     const { return cost; }

    // ← Nuevos getters para tooltip
    float        getRange()    const { return range; }
    float        getCooldown() const { return cooldown; }

    /* tipo de daño de la torre */
    virtual DamageType getDamageType() const = 0;

    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }

protected:
    bool   canAttack() const;
    Enemy* findTarget(const std::vector<Enemy*>& enemies) const;

    /* virtual: las subclases podrán añadir efectos especiales */
    virtual void attack(Enemy* target);

    sf::Sprite    sprite;
    sf::Vector2f  position;
    float         range;
    int           damage;
    float         cooldown;
    float         specialChance;
    int           cost;
    sf::Clock     atkTimer;

private:
    int upgradeLevel = 0;

    /* tablas de upgrades */
    static constexpr int   upgradeCosts     [MAX_UPGRADES] = {100,200,400};
    static constexpr float damageMults      [MAX_UPGRADES] = {1.25f,1.30f,1.50f};
    static constexpr float cooldownMults    [MAX_UPGRADES] = {0.90f,0.90f,0.80f};
    static constexpr float specialChanceAdd [MAX_UPGRADES] = {0.05f,0.08f,0.10f};
};
