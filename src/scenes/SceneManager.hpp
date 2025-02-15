#pragma once

#include <memory>

#include "SDL3/SDL_events.h"

class wlScene_Base;

enum class sceneType_t : uint8_t {
	MAIN_MENU,
	TEST_GAME,
};

class wlSceneManager {
public:
	void					ChangeScene( sceneType_t sceneType);
	void					OnUpdate( double deltaTime );
	void					OnEvent( SDL_Event* event );

private:
	std::shared_ptr<wlScene_Base> currentScene = nullptr;
};