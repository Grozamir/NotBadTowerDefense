#pragma once
#include "entt.hpp"

struct wlVec2;

entt::entity spawnBaseBullet( entt::registry& reg, entt::entity targetEnemy, wlVec2 pos, float damage, float radiusCollision, float speed );