#pragma once
#include <SFML/Graphics.hpp>
#include "enemies/Enemy.h"   // base Enemy
#include <vector>

class Tower {
public:
    Tower(sf::Vector2f position, float range, int damage);
    void update(const std::vector<Enemy*>& enemies);
    void draw(sf::RenderWindow& window);
private:
    sf::Sprite sprite;
    sf::Texture texture;
    float range;
    int damage;
    sf::Vector2f position;
    sf::Clock attackCooldown;
    bool canAttack() const;
};
