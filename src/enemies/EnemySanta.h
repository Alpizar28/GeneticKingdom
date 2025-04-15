#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class EnemySanta {
public:
    EnemySanta();
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void setPath(const std::vector<sf::Vector2i>& path, int tileSize);

private:
    std::vector<sf::Texture> textures;
    sf::Sprite sprite;
    int currentFrame = 0;
    float animationTimer = 0.f;
    const float frameDuration = 0.1f;

    std::vector<sf::Vector2f> waypoints;
    int currentTarget = 0;
    float speed = 80.f;
};
