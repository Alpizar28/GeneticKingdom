// src/enemies/Enemy.h
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class Enemy {
public:
    Enemy(float maxHp, float speed, float frameDuration);
    virtual ~Enemy() = default;

    // Define la ruta a seguir
    void setPath(const std::vector<sf::Vector2i>& path, int tileSize);

    // Actualiza animación y posición
    void update(float deltaTime);

    // Dibuja sprite y barra de vida
    virtual void draw(sf::RenderWindow& window);

    // Inflige daño y loggea
    void takeDamage(float amount);

    // Recompensa en oro al morir (10% de la vida máxima por defecto)
    virtual int getRewardGold() const;

    // Accesores
    const sf::Sprite& getSprite() const { return sprite; }
    bool               isFinished() const { return hp <= 0; }
    float              getHp()       const { return hp; }
    int                getId()       const { return id; }

protected:
    // Cada subclase carga sus propias texturas
    virtual void loadTextures() = 0;

    std::vector<sf::Texture> textures;
    sf::Sprite               sprite;
    std::vector<sf::Vector2f> waypoints;
    size_t                   currentTarget = 0;

    float hp;
    float maxHp;
    float speed;
    float frameDuration;
    float animationTimer = 0.f;
    int   currentFrame   = 0;

private:
    int id;
    static int nextId;
};
