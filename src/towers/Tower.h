#pragma once
#include <SFML/Graphics.hpp>
#include "../enemies/Enemy.h"
#include <vector>

class Tower {
public:
    Tower(const sf::Texture& tex,
          sf::Vector2f   pos,
          float          range,
          int            damage);

    // Intenta disparar; si el cooldown ha pasado y hay un enemigo en rango,
    // reinicia el cooldown y devuelve puntero al Enemy, sino nullptr.
    Enemy* tryFire(const std::vector<Enemy*>& enemies);

    // Sólo dibuja el sprite
    void draw(sf::RenderWindow& window) const;

    // Para GameManager
    sf::Vector2f getPosition() const { return position; }
    int          getDamage()   const { return damage;   }

private:
    sf::Sprite    sprite;
    float         range;
    int           damage;
    sf::Vector2f  position;
    sf::Clock     attackCooldown;

    bool canAttack() const;
};
