#pragma once

#include <vector>

#include "../math/Vector.hpp"

enum class cellType_t : uint8_t {
	WALL	= 0,
	FLOOR	= 1,
	START	= 2,
	END		= 3,
	TOWER	= 4
};

using wlLevelGrid		=	std::vector<std::vector<cellType_t>>;
using wlCellPos			=	std::pair<int32_t, int32_t>;

struct wlLevelState {
	const wlLevelGrid *		sourceMap = nullptr;

	// For save position tower
	wlLevelGrid				currentMap;

	std::vector<wlCellPos>	pathForEnemy;

	std::vector<wlCellPos>	posTowers;

	float					offsetCell{ 150.0f };

	const wlLevelGrid& GetSourceMap() const {
		return *sourceMap;
	}
};

struct wlCenteringOffset {
	wlVec2					value{};
};
