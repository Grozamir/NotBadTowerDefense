#pragma once

#include "entt.hpp"

void updateBulletTracking(entt::registry& reg);

void updateEnemyHealthOnHit( entt::registry& reg );

void updateDestroyBulletOnInvalidTarget( entt::registry& reg );
