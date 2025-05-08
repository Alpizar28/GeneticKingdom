#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

/* ---------- Tipos de daño ---------- */
enum class DamageType { Arrow, Magic, Artillery };

/* ---------- Resistencias ---------- */
struct Resistances {
    float arrow     = 1.0f;   // >1 ⇒ resistente, <1 ⇒ débil, 0 ⇒ inmune
    float magic     = 1.0f;
    float artillery = 1.0f;
};

class Enemy {
public:
    Enemy(float maxHp, float speed, float frameDuration,
          Resistances resists = {});
    virtual ~Enemy() = default;

    /* ---- ciclo de vida ---- */
    void setPath(const std::vector<sf::Vector2i>& path, int tileSize);
    void update(float dt);
    void draw(sf::RenderWindow& window);

    /* ---- daño ---- */
    void applyDamage(float amount, DamageType t);
    void takeDamage(float amount) { applyDamage(amount, DamageType::Arrow); }

    /* ---- setters para GA ---- */
    void setMaxHp(float newMaxHp);
    void setSpeed(float newSpeed);

    /* ---- getters ---- */
    bool  isDead()      const { return hp <= 0 && !escaped; }
    bool  hasEscaped()  const { return escaped; }
    bool  isFinished()  const { return escaped || isDead(); }

    float getHp()       const { return hp; }
    float getMaxHp()    const { return maxHp; }
    int   getRewardGold() const { return static_cast<int>(maxHp * 0.1f); }

    int   getWaypointsReached() const { return waypointsReached; }   // ← NUEVO

    const sf::Sprite& getSprite() const { return sprite; }

protected:
    virtual void loadTextures() = 0;

    /* ---- animación ---- */
    std::vector<sf::Texture> textures;
    sf::Sprite               sprite;

    /* ---- ruta ---- */
    std::vector<sf::Vector2f> waypoints;
    int   waypointIndex   = 0;
    int   waypointsReached= 0;           // ← NUEVO

    /* ---- stats ---- */
    float hp        = 0.f;
    float maxHp     = 0.f;
    float speed     = 0.f;
    Resistances res;

    /* ---- anim ---- */
    float frameDuration = 0.f;
    float animTimer     = 0.f;
    int   currentFrame  = 0;

private:
    bool escaped = false;
    int  id;
    static int nextId;
};
