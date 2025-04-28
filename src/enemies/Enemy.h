// src/enemies/Enemy.h
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class Enemy {
public:
    Enemy(float maxHp, float speed, float frameDuration);
    virtual ~Enemy() = default;

    void setPath(const std::vector<sf::Vector2i>& path, int tileSize);
    void update(float deltaTime);
    virtual void draw(sf::RenderWindow& window);

    void takeDamage(float amount);
    virtual int getRewardGold() const { return static_cast<int>(maxHp * 0.1f); }

    // Setters para GA
    void setMaxHp(float m)   { maxHp = m; hp = m; }
    void setSpeed(float s)   { speed = s; }

    // Consultas
    bool isDead()    const { return hp <= 0.f && !escaped; }
    bool hasEscaped()const { return escaped; }
    bool isFinished()const { return isDead() || escaped; }
    float getHp()    const { return hp; }
    float getMaxHp() const { return maxHp; }
    int   getId()    const { return id; }
    int   getWaypointsReached() const { return waypointIndex; }

    const sf::Sprite& getSprite() const { return sprite; }

protected:
    virtual void loadTextures() = 0;

    std::vector<sf::Texture> textures;
    sf::Sprite               sprite;
    std::vector<sf::Vector2f> waypoints;
    int                      waypointIndex = 0;

    float hp, maxHp;
    float speed;
    float frameDuration;
    float animationTimer = 0.f;
    int   currentFrame   = 0;

private:
    int id;
    bool escaped = false;
    static int nextId;
};