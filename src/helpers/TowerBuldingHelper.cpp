#include "TowerBuldingHelper.hpp"

#include "../components/RenderComponents.hpp"
#include "../components/GameContextComponents.hpp"
#include "../components/PhysicsComponents.hpp"
#include "../components/GameComponents.hpp"

#include "../resources/Sprites.hpp"

void TryBuildTower( entt::registry& reg, const SDL_Event* event ) {
	const auto& offset = reg.ctx().get<wlCenteringOffset>();
	auto& levelState = reg.ctx().get<wlLevelState>();
	const float mousePositionX = event->button.x - offset.value.x;
	const float mousePositionY = event->button.y - offset.value.y;

	const int32_t indexCellX = mousePositionX / levelState.offsetCell;
	const int32_t indexCellY = mousePositionY / levelState.offsetCell;

	const auto& sourceMap = levelState.GetSourceMap();
	if ( sourceMap.size() > indexCellY && sourceMap.at( indexCellY ).size() > indexCellX ) {
		if ( sourceMap[indexCellY][indexCellX] == cellType_t::WALL &&
			 std::ranges::find( levelState.posTowers, std::pair{ indexCellY, indexCellX } ) == levelState.posTowers.end() ) {
			levelState.currentMap[indexCellY][indexCellX] = cellType_t::TOWER;

			const auto towerEnt = reg.create();
			reg.emplace<wlPosition>( towerEnt, wlVec2{ levelState.offsetCell * indexCellX + levelState.offsetCell / 4.0f,
													   levelState.offsetCell * indexCellY + levelState.offsetCell / 4.0f } );
			reg.emplace<wlTower>( towerEnt, 0.2f );
			auto& sprite = reg.emplace<wlSprite>( towerEnt );
			sprite.texture = wlSprites::gameAtlas.texture;
			sprite.srcRect = wlSprites::gameAtlas.GetSpriteData( "base_tower" ).srcRect;
			sprite.isSimpleSprite = false;
			sprite.scale = 6.0f;

			levelState.posTowers.push_back( { indexCellY, indexCellX } );
		}
	}
}
