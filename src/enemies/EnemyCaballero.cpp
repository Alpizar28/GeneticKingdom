#include "EnemyCaballero.h"
#include <filesystem>

namespace fs = std::filesystem;

/*  Caballero  ──────────────────────────────────────────────
    • Muy blindado frente a flechas (1.4× menos daño)
    • Algo resistente a artillería (0.8×)
    • Ligeramente débil a magia    (0.9× = 1.1× daño)
*/
EnemyCaballero::EnemyCaballero()
    : Enemy(
        /*HP*/   220.f,
        /*spd*/   45.f,
        /*anim*/  0.12f,
        /*res*/  {1.4f, 0.9f, 0.8f}
      )
{
    loadTextures();

    /* Escala apropiada a tu tile (ej. ~48 px de ancho) */
    float desired = 48.f;
    float scale   = desired / textures[0].getSize().x;
    sprite.setScale(scale, scale);

    /* Origen centrado */
    sprite.setOrigin(textures[0].getSize().x / 2.f,
                     textures[0].getSize().y / 2.f);
}

void EnemyCaballero::loadTextures()
{
    fs::path dir = fs::current_path().parent_path() /
                   "assets/sprites/enemies/caballero/";
    for (int i = 1; i <= 10; ++i) {
        sf::Texture t;
        if (t.loadFromFile(dir / ("Walk (" + std::to_string(i) + ").png")))
            textures.push_back(t);
    }
    if (!textures.empty())
        sprite.setTexture(textures[0]);
}
