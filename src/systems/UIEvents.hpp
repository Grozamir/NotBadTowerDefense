#pragma once

#include <entt.hpp>

#include "SDL3/SDL_events.h"

bool HandleUIMouseEvent( entt::registry& reg, const SDL_Event* event );