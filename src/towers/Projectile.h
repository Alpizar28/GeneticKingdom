#pragma once
#include <SFML/Graphics.hpp>
#include "../enemies/Enemy.h"

class Projectile {
public:
    Projectile(const sf::Texture& tex,
               sf::Vector2f   start,
               Enemy*         target,
               float          speed,
               int            damage);

    void update(float dt);
    void draw(sf::RenderWindow& window) const;
    bool isAlive() const;

private:
    sf::Sprite    sprite;
    sf::Vector2f  velocity;
    Enemy*        target;
    int           damage;
    bool          alive;

    float         radius;    // para colisión circular
};
