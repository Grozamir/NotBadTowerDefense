#pragma once
#include "entt.hpp"

struct wlVec2;

entt::entity SpawnBaseBullet( entt::registry& reg, entt::entity targetEnemy, wlVec2 pos, float damage, float radiusCollision, float speed );
entt::entity SpawnBaseEnemy( entt::registry& reg, wlVec2 pos );
