#pragma once

#include "GameContextComponents.hpp"

struct wlPathFollower {
	std::vector<wlCellPos> path;
	std::size_t currentTargetPointIndex = 1;

	/**
	 * @return if false, then it means that the path has ended
	 */
	bool NextPoint() {
		if ( path.size() - 1 > currentTargetPointIndex + 1 ) {
			++currentTargetPointIndex;
			return true;
		}
		return false;
	}
};
