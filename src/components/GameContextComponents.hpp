#pragma once

#include <vector>

#include <cstdint>

#include "../math/Vector.hpp"

struct TTF_Text;

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

struct wlGameState {
	int32_t					money{ 0 };


	TTF_Text *			txtMoney{ nullptr };

	void AddMoney( const int32_t addedMoney) {
		money += addedMoney;
		UpdateTxtMoney();
	}

	void SetMoney( const int32_t newMoney ) {
		money = newMoney;
		UpdateTxtMoney();
	}

	void UpdateTxtMoney() const;
};



inline constexpr int32_t priceBaseTower = 10;
inline constexpr int32_t incomeForKillBaseEnemy = 1;
