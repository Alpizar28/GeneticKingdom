#include "EnemySanta.h"
#include <filesystem>
namespace fs = std::filesystem;

/*  Santa  ───────────────────────────────────────────────────
    • Estadísticas balanceadas, sin resistencias especiales.
*/
EnemySanta::EnemySanta()
    : Enemy(
        /*HP*/   150.f,
        /*spd*/   60.f,
        /*anim*/  0.10f,
        /*res*/  {1.0f, 1.0f, 1.0f}
      )
{
    loadTextures();

    /* --- ajuste de escala usando la ALTURA ---------------------------- */
    const float desiredHeight = 48.f;                 // píxeles que quieres ocupar
    float scale = desiredHeight / textures[0].getSize().y;
    sprite.setScale(scale, scale);

    /* origen centrado para todos los fotogramas */
    sprite.setOrigin(textures[0].getSize().x / 2.f,
                     textures[0].getSize().y / 2.f);
}

void EnemySanta::loadTextures()
{
    fs::path dir = fs::current_path().parent_path() /
                   "assets/sprites/enemies/santa/";
    for (int i = 1; i <= 10; ++i) {
        sf::Texture t;
        if (t.loadFromFile(dir / ("Walk (" + std::to_string(i) + ").png")))
            textures.push_back(t);
    }
    if (!textures.empty())
        sprite.setTexture(textures[0]);
}
