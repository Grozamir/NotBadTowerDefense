#pragma once

#include "SDL3/SDL_stdinc.h"

struct SDL_Window;
struct SDL_Renderer;
union  SDL_Event;

class wlSceneManager;

struct wlAppState {
	SDL_Window *			window = nullptr;
	SDL_Renderer *			renderer = nullptr;

	int32_t					currentWidthScreen{1280};
	int32_t					currentHeightScreen{ 720 };
};

extern wlAppState* appState;

class wlApplication {
public:
	wlApplication();
	~wlApplication();

	int						Init();
	void					OnUpdate();
	void					OnEvent( SDL_Event* event ) const;

	wlAppState *			GetState() const { return state; }
	wlSceneManager*			GetSceneMgr() const { return sceneMgr; }

private:
	wlAppState *			state = nullptr;

	wlSceneManager *		sceneMgr = nullptr;

	Uint64					oldTime = 0;
	double					deltaTime = 0.0;
};