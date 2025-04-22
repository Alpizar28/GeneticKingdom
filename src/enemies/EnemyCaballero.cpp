#include "EnemyCaballero.h"
#include <filesystem>

namespace fs = std::filesystem;

EnemyCaballero::EnemyCaballero()
  : Enemy(150.f, 50.f, 0.1f)  // ej. HP=150, speed=50
{
    loadTextures();
    if (!textures.empty()) {
        sprite.setTexture(textures[0]);
        sprite.setScale(0.07f, 0.07f);
        auto b = sprite.getLocalBounds();
        sprite.setOrigin(b.width/2, b.height/2);
    }
}

void EnemyCaballero::loadTextures() {
    fs::path base = fs::current_path().parent_path();
    std::string dir = base.string() + "/assets/sprites/enemies/caballero/";
    for (int i = 1; i <= 10; ++i) {
        sf::Texture tex;
        if (tex.loadFromFile(dir + "Walk (" + std::to_string(i) + ").png")) {
            textures.push_back(tex);
        }
    }
}
