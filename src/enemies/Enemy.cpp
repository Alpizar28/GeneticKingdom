// src/enemies/Enemy.cpp
#include "Enemy.h"
#include <cmath>

int Enemy::nextId = 1;

Enemy::Enemy(float maxHp_, float speed_, float frameDuration_)
  : hp(maxHp_), maxHp(maxHp_), speed(speed_), frameDuration(frameDuration_), id(nextId++)
{
}

void Enemy::setPath(const std::vector<sf::Vector2i>& path, int tileSize) {
    waypoints.clear();
    for (auto& t : path)
        waypoints.emplace_back(
          t.x * tileSize + tileSize/2.f,
          t.y * tileSize + 80 + tileSize/2.f
        );
    waypointIndex = 0;
    if (!waypoints.empty())
        sprite.setPosition(waypoints[0]);
}

void Enemy::update(float dt) {
    if (textures.empty() || waypoints.empty() || isFinished())
        return;

    // Animación
    animationTimer += dt;
    if (animationTimer >= frameDuration) {
        animationTimer = 0.f;
        currentFrame = (currentFrame + 1) % textures.size();
        sprite.setTexture(textures[currentFrame]);
    }

    // Movimiento
    auto pos = sprite.getPosition();
    auto targetPos = waypoints[waypointIndex];
    sf::Vector2f dir = targetPos - pos;
    float dist = std::hypot(dir.x, dir.y);
    if (dist < 2.f) {
        if (++waypointIndex >= static_cast<int>(waypoints.size())) {
            escaped = true;
        }
    } else {
        sprite.move((dir / dist) * speed * dt);
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    // Barra de vida
    sf::RectangleShape back({40,6});
    back.setFillColor({50,50,50,200});
    back.setPosition(sprite.getPosition().x-20, sprite.getPosition().y-30);
    window.draw(back);

    float ratio = hp / maxHp;
    sf::RectangleShape front({40*ratio,6});
    front.setFillColor(sf::Color::Green);
    front.setPosition(back.getPosition());
    window.draw(front);
}

void Enemy::takeDamage(float amount) {
    if (escaped) return;
    hp -= amount;
    if (hp < 0) hp = 0;
}
