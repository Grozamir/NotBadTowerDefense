#pragma once
#include "Atlas.hpp"

namespace sprites {

inline wlAtlas gameAtlas;

inline void load() {
	gameAtlas.ParseAtlas( "./assets/", "game_atlas.json" );
}

inline void unload() {
	
}

}