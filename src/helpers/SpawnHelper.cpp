#include "SpawnHelper.hpp"

#include "../math/Vector.hpp"

#include "../components/GameComponents.hpp"
#include "../components/PhysicsComponents.hpp"
#include "../components/RenderComponents.hpp"
#include "../components/NavigationComponents.hpp"

#include "../resources/Sprites.hpp"


entt::entity spawnBaseBullet( entt::registry& reg, entt::entity targetEnemy, wlVec2 pos, float damage, float radiusCollision, float speed ) {
	const auto bulletEnt = reg.create();
	reg.emplace<wlBullet>( bulletEnt, damage, radiusCollision, targetEnemy );
	reg.emplace<wlPosition>( bulletEnt, pos );
	reg.emplace<wlVelocity>( bulletEnt, wlVec2{ 0.0f, 0.0f }, speed );
	auto& sprite = reg.emplace<wlSprite>( bulletEnt );
	sprite.texture = wlSprites::gameAtlas.texture;
	sprite.srcRect = wlSprites::gameAtlas.GetSpriteData( "bullet_01" ).srcRect;
	sprite.scale = 4.0f;
	return bulletEnt;
}

entt::entity spawnBaseEnemy( entt::registry& reg, wlVec2 pos ) {
	auto& levelState = reg.ctx().get<wlLevelState>();
	const auto enemyEnt = reg.create();
	reg.emplace<wlEnemy>( enemyEnt, 25.0f );
	reg.emplace<wlHealth>( enemyEnt, 10.0f );
	reg.emplace<wlPosition>( enemyEnt, pos);
	reg.emplace<wlVelocity>( enemyEnt, wlVec2{ 0.0f, 0.0f }, 100.0f );
	reg.emplace<wlPathFollower>( enemyEnt, levelState.pathForEnemy );
	auto& sprite = reg.emplace<wlSprite>( enemyEnt );
	sprite.texture = wlSprites::gameAtlas.texture;
	sprite.srcRect = wlSprites::gameAtlas.GetSpriteData( "base_enemy" ).srcRect;
	sprite.scale = 4.0f;
	return enemyEnt;
}
