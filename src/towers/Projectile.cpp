#include "Projectile.h"
#include <cmath>

Projectile::Projectile(const sf::Texture& tex,
                       sf::Vector2f   start,
                       Enemy*         target,
                       float          speed,
                       int            damage)
  : target(target)
  , damage(damage)
  , alive(true)
{
    sprite.setTexture(tex);
    sprite.setOrigin(tex.getSize().x/2.f, tex.getSize().y/2.f);

    // escalamos a un ancho deseado (p.ej. 16 px)
    float desiredWidth = 16.f;
    float s = desiredWidth / tex.getSize().x;
    sprite.setScale(s, s);

    // guardamos el radio para colisión (mitad del ancho en px)
    radius = (tex.getSize().x * s) * 0.5f;

    // calculamos el punto de spawn (se añade un offset en punta del cañón)
    sf::Vector2f dir = (target->getSprite().getPosition() - start);
    float len = std::hypot(dir.x, dir.y);
    sf::Vector2f norm = (len>0 ? dir/len : sf::Vector2f{0,0});
    float towerTipOffset = tex.getSize().x * s * 0.5f; 
    sprite.setPosition(start + norm * towerTipOffset);

    // velocidad
    velocity = norm * speed;
}

void Projectile::update(float dt) {
    if (!alive) return;
    sprite.move(velocity * dt);

    // Centros de proyectil y enemigo
    auto p = sprite.getPosition();
    auto e = target->getSprite().getPosition();
    float dx = e.x - p.x;
    float dy = e.y - p.y;
    float dist = std::hypot(dx, dy);

    // Radios aproximados (ancho/2)
    float rBullet = sprite.getGlobalBounds().width  / 2.f;
    float rEnemy  = target->getSprite().getGlobalBounds().width / 2.f;

    if (dist <= rBullet + rEnemy) {
        target->takeDamage(damage);
        alive = false;
    }
}


void Projectile::draw(sf::RenderWindow& window) const {
    if (!alive) return;
    window.draw(sprite);
}

bool Projectile::isAlive() const {
    return alive;
}
