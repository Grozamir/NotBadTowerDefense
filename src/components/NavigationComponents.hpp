#pragma once

#include "GameContextComponents.hpp"

struct wlPathFollower {
	std::vector<wlCellPos> path;
	std::size_t currentTargetPointIndex = 1;

	void NextPoint() {
		if ( path.size() - 1 > currentTargetPointIndex + 1 ) {
			++currentTargetPointIndex;
		}
	}
};
