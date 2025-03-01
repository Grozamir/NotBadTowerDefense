#pragma once

#include "entt.hpp"

void UpdateBulletTracking(entt::registry& reg);

void UpdateEnemyHealthOnHit( entt::registry& reg );

void UpdateDestroyBulletOnInvalidTarget( entt::registry& reg );
