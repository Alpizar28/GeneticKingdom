#include "Enemy.h"
#include <cmath>
#include <iostream>

Enemy::Enemy(float speed, float frameDuration)
    : speed(speed), frameDuration(frameDuration)
{
    std::cout << "👾 Enemigo base creado\n";
}

void Enemy::setPath(const std::vector<sf::Vector2i>& path, int tileSize) {
    waypoints.clear();
    for (auto& t : path) {
        waypoints.push_back({
            t.x * tileSize + tileSize/2.f,
            t.y * tileSize + 80 + tileSize/2.f
        });
    }
    if (!waypoints.empty())
        sprite.setPosition(waypoints[0]);
}

void Enemy::update(float dt) {
    if (textures.empty() || waypoints.empty() || isFinished()) return;

    // Animación
    animationTimer += dt;
    if (animationTimer >= frameDuration) {
        animationTimer = 0.f;
        currentFrame = (currentFrame + 1) % textures.size();
        sprite.setTexture(textures[currentFrame]);
    }

    // Movimiento
    sf::Vector2f pos = sprite.getPosition();
    sf::Vector2f dir = waypoints[currentTarget] - pos;
    float dist = std::sqrt(dir.x*dir.x + dir.y*dir.y);
    if (dist < 2.0f) {
        ++currentTarget;
    } else {
        sprite.move((dir / dist) * speed * dt);
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
