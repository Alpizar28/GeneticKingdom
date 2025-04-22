#include "EnemyCaballero.h"
#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;

EnemyCaballero::EnemyCaballero()
    : Enemy(60.f, 0.12f)  // velocidad algo menor, fotograma un poco más lento
{
    fs::path base = fs::current_path().parent_path() 
                  / "assets/sprites/enemies/caballero/";
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
