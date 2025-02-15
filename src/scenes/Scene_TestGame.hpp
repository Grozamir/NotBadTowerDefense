#pragma once

#include "Scene_Base.hpp"


class wlScene_TestGame : public wlScene_Base {
public:
	void					Start() override;
	void					OnUpdate( const double deltaTime ) override;
	void					OnEvent(SDL_Event* event) override;
};