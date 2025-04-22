#include "EnemyZombie.h"
#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;

EnemyZombie::EnemyZombie()
    : Enemy(50.f, 0.15f)  // más lento aún, fotograma más lento
{
    fs::path base = fs::current_path().parent_path() 
                  / "assets/sprites/enemies/zombie/";
    for (int i = 1; i <= 10; ++i) {
        sf::Texture tex;
        auto file = (base / ("Walk (" + std::to_string(i) + ").png")).string();
        if (tex.loadFromFile(file)) textures.push_back(tex);
        else std::cerr << "❌ Falta sprite: " << file << "\n";
    }
    if (!textures.empty()) {
        sprite.setTexture(textures[0]);
        sprite.setScale(0.07f, 0.07f);
        auto b = sprite.getLocalBounds();
        sprite.setOrigin(b.width/2.f, b.height/2.f);
    }
}
