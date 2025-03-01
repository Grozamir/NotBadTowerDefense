#pragma once

#include "Vector.hpp"

namespace wlMath2D {

inline bool PointInRect( const wlVec2 point, const SDL_FRect rect ) {
	return ( point.x >= rect.x && point.x <= rect.x + rect.w &&
			 point.y >= rect.y && point.y <= rect.y + rect.h );
}

}