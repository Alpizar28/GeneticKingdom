// src/towers/Tower.h
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "../enemies/Enemy.h"
#include <SFML/System/Clock.hpp>

enum class TowerType { None, Archer, Mage, Artillery };

class Tower {
public:
    static constexpr int MAX_UPGRADES = 3;

    Tower(const sf::Texture& tex, sf::Vector2f pos, float range, int damage);
    virtual ~Tower() = default;

    // dispara si puede, devuelve el enemigo impactado
    Enemy* tryFire(const std::vector<Enemy*>& enemies);

    void draw(sf::RenderWindow& window) const;

    // --- upgrade API ---
    /** Intenta mejorar la torre. Devuelve true si hubo upgrade y descontó oro. */
    bool upgrade(int& playerGold);

    /** Costo del próximo upgrade (0 si está al máximo). */
    int  getNextUpgradeCost() const;

    /** Nivel actual de upgrade [0..MAX_UPGRADES] */
    int  getUpgradeLevel() const { return upgradeLevel; }

    sf::Vector2f getPosition() const  { return position; }
    int          getDamage()   const  { return damage; }

    sf::FloatRect getBounds() const {
        return sprite.getGlobalBounds();
    }

protected:
    bool   canAttack() const;
    Enemy* findTarget(const std::vector<Enemy*>& enemies) const;
    void   attack(Enemy* target);

    sf::Sprite    sprite;
    sf::Vector2f  position;
    float         range;
    int           damage;
    float         cooldown;
    float         specialChance;
    int           cost;
    sf::Clock     attackCooldown;

private:
    int upgradeLevel = 0;
    // arrays constexpr: costos y multiplicadores de cada nivel
    static constexpr int   upgradeCosts     [MAX_UPGRADES] = {100, 200, 400};
    static constexpr float damageMults      [MAX_UPGRADES] = {1.25f, 1.25f, 1.50f};
    static constexpr float cooldownMults    [MAX_UPGRADES] = {0.90f, 0.90f, 0.80f};
    static constexpr float specialChanceAdd[MAX_UPGRADES] = {0.05f, 0.05f, 0.10f};
};
