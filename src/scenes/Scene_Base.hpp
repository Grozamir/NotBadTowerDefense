#pragma once

#include <entt.hpp>

#include "SDL3/SDL_events.h"

class wlScene_Base {
public:
	entt::registry			reg;

	virtual void			Start() = 0;
	virtual void			OnUpdate( double deltaTime ) = 0;
	virtual void			OnEvent( SDL_Event* event ) = 0;

	virtual					~wlScene_Base() = default;
	
};