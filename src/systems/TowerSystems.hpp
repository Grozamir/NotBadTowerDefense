#pragma once
#include "entt.hpp"

void UpdateTowerTargetSelection(entt::registry& reg);
void UpdateTowerLookAtEnemy( entt::registry& reg );
void UpdateTowerAttack( entt::registry& reg, const double deltaTime );
