#include "EnemySanta.h"
#include <filesystem>

namespace fs = std::filesystem;

EnemySanta::EnemySanta()
  : Enemy(100.f, 80.f, 0.1f)
{
    loadTextures();
    if (!textures.empty()) {
        sprite.setTexture(textures[0]);
        sprite.setScale(0.07f, 0.07f);
        auto b = sprite.getLocalBounds();
        sprite.setOrigin(b.width/2, b.height/2);
    }
}

void EnemySanta::loadTextures() {
    fs::path base = fs::current_path().parent_path();
    std::string dir = base.string() + "/assets/sprites/enemies/santa/";
    for (int i = 1; i <= 13; ++i) {
        sf::Texture tex;
        if (tex.loadFromFile(dir + "Walk (" + std::to_string(i) + ").png")) {
            textures.push_back(tex);
        }
    }
}
