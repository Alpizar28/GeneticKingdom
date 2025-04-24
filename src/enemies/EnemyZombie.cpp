#include "EnemyZombie.h"
#include <filesystem>

namespace fs = std::filesystem;

EnemyZombie::EnemyZombie()
  : Enemy(120.f, 60.f, 0.1f)  // por ejemplo HP=120, speed=60
{
    loadTextures();
    if (!textures.empty()) {
        sprite.setTexture(textures[0]);
        sprite.setScale(0.07f, 0.07f);
        auto b = sprite.getLocalBounds();
        sprite.setOrigin(b.width/2, b.height/2);
    }
}

void EnemyZombie::loadTextures() {
    fs::path base = fs::current_path().parent_path();
    std::string dir = base.string() + "/assets/sprites/enemies/zombie/";
    for (int i = 1; i <= 10; ++i) {
        sf::Texture tex;
        if (tex.loadFromFile(dir + "Walk (" + std::to_string(i) + ").png")) {
            textures.push_back(tex);
        }
    }
}
