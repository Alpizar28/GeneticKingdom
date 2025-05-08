#include "Enemy.h"
#include <cmath>
#include <algorithm>

int Enemy::nextId = 1;

/* ---------- ctor ---------- */
Enemy::Enemy(float maxHp_, float speed_, float frameDur_,
             Resistances resists)
    : hp(maxHp_), maxHp(maxHp_),
      speed(speed_), res(resists),
      frameDuration(frameDur_),
      id(nextId++)
{}

/* ---------- setters para GA ---------- */
void Enemy::setMaxHp(float newMaxHp)
{
    maxHp = std::max(1.f, newMaxHp);
    hp    = std::min(hp, maxHp);
}
void Enemy::setSpeed(float newSpeed)
{
    speed = std::max(10.f, newSpeed);
}

/* ---------- asignar ruta ---------- */
void Enemy::setPath(const std::vector<sf::Vector2i>& path, int tile)
{
    waypoints.clear();
    for (auto& t : path)
        waypoints.emplace_back(
            t.x * tile + tile / 2.f,
            t.y * tile + 80.f + tile / 2.f
        );
    waypointIndex    = 0;
    waypointsReached = 0;
    if (!waypoints.empty())
        sprite.setPosition(waypoints[0]);
}

/* ---------- update ---------- */
void Enemy::update(float dt)
{
    if (textures.empty() || waypoints.empty() || isFinished())
        return;

    /* animación -------------------------------------------------------------- */
    animTimer += dt;
    if (animTimer >= frameDuration) {
        animTimer   = 0.f;
        currentFrame = (currentFrame + 1) %
                       static_cast<int>(textures.size());

        sprite.setTexture(textures[currentFrame], true);

        /* mantener el origen centrado en cada fotograma */
        auto& tex = textures[currentFrame];
        sprite.setOrigin(tex.getSize().x / 2.f, tex.getSize().y / 2.f);
    }

    /* movimiento ------------------------------------------------------------- */
    sf::Vector2f pos = sprite.getPosition();
    sf::Vector2f tgt = waypoints[waypointIndex];
    sf::Vector2f dir = tgt - pos;
    float dist = std::hypot(dir.x, dir.y);

    if (dist < 2.f) {
        ++waypointsReached;
        if (++waypointIndex >= static_cast<int>(waypoints.size()))
            escaped = true;
    } else {
        sprite.move((dir / dist) * speed * dt);
    }
}

/* ---------- draw ---------- */
void Enemy::draw(sf::RenderWindow& w)
{
    w.draw(sprite);

    sf::RectangleShape back({40.f, 6.f});
    back.setFillColor({50, 50, 50, 200});
    back.setPosition(sprite.getPosition().x - 20.f,
                     sprite.getPosition().y - 30.f);
    w.draw(back);

    float ratio = hp / maxHp;
    sf::RectangleShape front({40.f * ratio, 6.f});
    front.setFillColor(sf::Color::Green);
    front.setPosition(back.getPosition());
    w.draw(front);
}

/* ---------- daño ---------- */
void Enemy::applyDamage(float amount, DamageType t)
{
    float mult = 1.f;
    switch (t) {
        case DamageType::Arrow:     mult = res.arrow;     break;
        case DamageType::Magic:     mult = res.magic;     break;
        case DamageType::Artillery: mult = res.artillery; break;
    }
    if (mult == 0.f) return;          // inmune
    hp -= amount * (1.f / mult);      // >1 ⇒ menos daño
    if (hp < 0.f) hp = 0.f;
}
