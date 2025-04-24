// Tower.cpp
#include "Tower.h"
#include <cmath>
#include <iostream>

// Constructor (igual que antes)
Tower::Tower(sf::Vector2f position, float range, int damage) 
    : position(position), range(range), damage(damage) {
    if (!texture.loadFromFile("../../assets/sprites/towers/cannon.png")) {
        std::cerr << "Error cargando torre: cannon.png\n";
    }
    sprite.setTexture(texture);
    sprite.setPosition(position);
    sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
}

// Actualizar para usar EnemySanta*
// Tower.cpp (en update())

void Tower::update(const std::vector<Enemy*>& enemies) {
    for (auto enemy : enemies) {
        float dx = enemy->getSprite().getPosition().x - position.x;
        float dy = enemy->getSprite().getPosition().y - position.y;
        float distance = std::hypot(dx, dy);
        if (distance <= range && canAttack()) {
            enemy->takeDamage(damage);
            attackCooldown.restart();
        }
    }
}

// Resto del código (draw y canAttack permanecen igual)
void Tower::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

bool Tower::canAttack() const {
    return attackCooldown.getElapsedTime().asSeconds() >= 1.0f;
}