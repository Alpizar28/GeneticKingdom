#include "Tower.h"
#include <cmath>

Tower::Tower(const sf::Texture& tex,
             sf::Vector2f   pos,
             float          range,
             int            damage)
  : range(range),
    damage(damage),
    position(pos)
{
    sprite.setTexture(tex);
    sprite.setOrigin(tex.getSize().x/2.f, tex.getSize().y/2.f);
    sprite.setPosition(pos);
    attackCooldown.restart();
}

bool Tower::canAttack() const {
    return attackCooldown.getElapsedTime().asSeconds() >= 1.0f;
}

Enemy* Tower::tryFire(const std::vector<Enemy*>& enemies) {
    if (!canAttack()) return nullptr;

    Enemy* nearest = nullptr;
    float minDistSq = range * range;

    // Buscar enemigo dentro de range y con distancia mínima
    for (auto* e : enemies) {
        const auto& pos = e->getSprite().getPosition();
        float dx = pos.x - position.x;
        float dy = pos.y - position.y;
        float distSq = dx*dx + dy*dy;
        if (distSq <= minDistSq) {
            nearest   = e;
            minDistSq = distSq;
        }
    }

    if (nearest) {
        attackCooldown.restart();
    }
    return nearest;
}


void Tower::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}
