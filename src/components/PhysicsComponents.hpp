#pragma once

#include "../math/Vector.h"

struct wlVec2;

struct wlPosition {
	wlVec2			value;
};

struct wlVelocity {
	wlVec2			direction;
	float			speed = 0.0f;
};
