#include "EnemySanta.h"
#include <iostream>
#include <filesystem>
#include <cmath>

namespace fs = std::filesystem;

EnemySanta::EnemySanta() {
    fs::path basePath = fs::current_path().parent_path();
    std::string spriteDir = basePath.string() + "/assets/sprites/enemies/santa/";

    for (int i = 1; i <= 13; ++i) {
        sf::Texture tex;
        std::string filename = spriteDir + "Walk (" + std::to_string(i) + ").png";
        if (tex.loadFromFile(filename)) {
            textures.push_back(tex);
        } else {
            std::cerr << "❌ Falló: " << filename << "\n";
        }
    }

    if (!textures.empty()) {
        sprite.setTexture(textures[0]);
        sprite.setScale(0.07f, 0.07f); // más pequeño
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    }
}

void EnemySanta::setPath(const std::vector<sf::Vector2i>& path, int tileSize) {
    waypoints.clear();
    for (const auto& tile : path) {
        sf::Vector2f pos(
            tile.x * tileSize + tileSize / 2.f,
            tile.y * tileSize + 80 + tileSize / 2.f
        );
                waypoints.push_back(pos);
    }

    if (!waypoints.empty()) {
        sprite.setPosition(waypoints[0]);
    }
}

void EnemySanta::update(float deltaTime) {
    if (textures.empty() || waypoints.empty() || currentTarget >= waypoints.size()) return;

    animationTimer += deltaTime;
    if (animationTimer >= frameDuration) {
        animationTimer = 0.f;
        currentFrame = (currentFrame + 1) % textures.size();
        sprite.setTexture(textures[currentFrame]);
    }

    sf::Vector2f pos = sprite.getPosition();
    sf::Vector2f dir = waypoints[currentTarget] - pos;
    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (dist < 2.0f) {
        currentTarget++;
    } else {
        sf::Vector2f norm = dir / dist;
        sprite.move(norm * speed * deltaTime);
    }
}

void EnemySanta::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
