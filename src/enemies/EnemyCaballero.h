#pragma once
#include "Enemy.h"

class EnemyCaballero : public Enemy {
public:
    EnemyCaballero();
protected:
    void loadTextures() override;
};
