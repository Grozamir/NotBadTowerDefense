#pragma once
#include "entt.hpp"

void updateTowerTargetSelection(entt::registry& reg);
void updateTowerLookAtEnemy( entt::registry& reg );
void updateTowerAttack( entt::registry& reg, const double deltaTime );

