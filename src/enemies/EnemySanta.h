#pragma once
#include "Enemy.h"

class EnemySanta : public Enemy {
public:
    EnemySanta();
protected:
    void loadTextures() override;
};
