#pragma once

#include "Scene_Base.hpp"

class wlScene_MainMenu final : public wlScene_Base {
public:
	void Start() override;
	void OnUpdate( double deltaTime ) override;
	void OnEvent( SDL_Event* event ) override;
};
