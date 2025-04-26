#include "Enemy.h"
#include <cmath>

Enemy::Enemy(float maxHp, float speed, float frameDuration)
  : hp(maxHp)
  , maxHp(maxHp)
  , speed(speed)
  , frameDuration(frameDuration)
{
}

void Enemy::setPath(const std::vector<sf::Vector2i>& path, int tileSize) {
    waypoints.clear();
    for (auto& t : path) {
        waypoints.emplace_back(
            t.x * tileSize + tileSize/2.f,
            t.y * tileSize + 80 + tileSize/2.f
        );
    }
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
    sf::Vector2f pos = sprite.getPosition();
    sf::Vector2f dir = waypoints[currentTarget] - pos;
    float dist = std::sqrt(dir.x*dir.x + dir.y*dir.y);
    if (dist < 2.f) {
        if (++currentTarget >= waypoints.size())
            hp = 0;  // marca como terminado
    } else {
        sprite.move((dir / dist) * speed * dt);
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    // barra de vida
    sf::RectangleShape back({40.f, 6.f});
    back.setFillColor({50,50,50,200});
    back.setPosition(sprite.getPosition().x - 20, sprite.getPosition().y - 30);
    window.draw(back);

    float ratio = hp / maxHp;
    sf::RectangleShape front({40.f * ratio, 6.f});
    front.setFillColor(sf::Color::Green);
    front.setPosition(back.getPosition());
    window.draw(front);
}

void Enemy::takeDamage(float amount) {
    hp -= amount;
    if (hp <= 0) {
        hp = 0;
    }
}

bool Enemy::isFinished() const {
    return hp <= 0;
}

float Enemy::getHp() const {
    return hp;
}