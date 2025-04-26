#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

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

    // Inflige daño
    void takeDamage(float amount);
    const sf::Sprite& getSprite() const { return sprite; }  // Añade esto

    // Consultas
    bool isFinished() const;
    float getHp() const;

protected:
    // Carga las texturas específicas de cada enemigo
    virtual void loadTextures() = 0;

    std::vector<sf::Texture> textures;
    sf::Sprite               sprite;
    std::vector<sf::Vector2f> waypoints;
    size_t                   currentTarget   = 0;
    

    float hp;
    float maxHp;
    float speed;
    float frameDuration;
    float animationTimer    = 0.f;
    int   currentFrame      = 0;
};