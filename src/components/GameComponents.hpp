#pragma once


struct wlEnemy {
	float					radiusCollision = 1.0f;
};

struct wlTower {
	float					fireRate{ 0.1f };
	float					radiusAttack{ 250.0f };
	float					currentTimeForFire{ 0.0f };

	entt::entity			targetEnemy{ entt::null };
};

struct wlBullet {
	float					damage{ 1.0f };

	float					radiusCollision{ 1.0f };

	entt::entity			targetEnemy{ entt::null };
};

struct wlHealth {
	float					currentHealth{ 1.0f };
	float					maxHealth{ 1.0f };

	wlHealth( const float t_maxHealth ) : 
		currentHealth	{ t_maxHealth },
		maxHealth		{ t_maxHealth } {
	}
};

