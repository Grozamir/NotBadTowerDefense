#pragma once
#include "Atlas.hpp"

namespace wlSprites {

inline wlAtlas gameAtlas;
inline wlAtlas uiAtlas;

inline void Load() {
	gameAtlas.ParseAtlas( "./assets/", "game_atlas.json" );
	uiAtlas.ParseAtlas( "./assets/", "ui_atlas.json" );
}

inline void Unload() {
	gameAtlas.Unload();
	uiAtlas.Unload();
}

}