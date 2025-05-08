#pragma once
#include "Enemy.h"

class EnemyZombie : public Enemy {
public:
    EnemyZombie();
protected:
    void loadTextures() override;
};
