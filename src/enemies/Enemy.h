#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Enemy {
public:
    Enemy(float speed = 80.f, float frameDuration = 0.1f);
    virtual ~Enemy() = default;

    virtual void setPath(const std::vector<sf::Vector2i>& path, int tileSize);
    virtual void update(float deltaTime);
    virtual void draw(sf::RenderWindow& window);

    bool isFinished() const { return currentTarget >= waypoints.size(); }

protected:
    std::vector<sf::Texture> textures;
    sf::Sprite sprite;
    std::vector<sf::Vector2f> waypoints;
    size_t currentTarget = 0;

    float speed;
    float frameDuration;
    float animationTimer = 0.f;
    int currentFrame = 0;
};
