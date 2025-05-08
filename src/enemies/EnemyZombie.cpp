#include "EnemyZombie.h"
#include <filesystem>
namespace fs = std::filesystem;

/*  Zombie  ─────────────────────────────────────────────────
    • Lento y débil a magia
*/
EnemyZombie::EnemyZombie()
    : Enemy(
        /*HP*/   100.f,
        /*spd*/   35.f,
        /*anim*/  0.15f,
        /*res*/  {0.8f, 0.6f, 1.2f}
      )
{
    loadTextures();

    float desired = 48.f;
    float scale   = desired / textures[0].getSize().x;
    sprite.setScale(scale, scale);

    sprite.setOrigin(textures[0].getSize().x / 2.f,
                     textures[0].getSize().y / 2.f);
}

void EnemyZombie::loadTextures()
{
    fs::path dir = fs::current_path().parent_path() /
                   "assets/sprites/enemies/zombie/";
    for (int i = 1; i <= 10; ++i) {
        sf::Texture t;
        if (t.loadFromFile(dir / ("Walk (" + std::to_string(i) + ").png")))
            textures.push_back(t);
    }
    if (!textures.empty())
        sprite.setTexture(textures[0]);
}
