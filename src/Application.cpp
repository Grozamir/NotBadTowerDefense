#include "application.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>

#include <iostream>

#include "scenes/SceneManager.hpp"

wlApplication::wlApplication() : 
							state	{ new wlAppState },
							sceneMgr{ new wlSceneManager } {
	appState = state;
	oldTime = SDL_GetPerformanceCounter();
}

wlApplication::~wlApplication() {
	if ( state ) {
		if ( state->renderer ) {
			SDL_DestroyRenderer( state->renderer );
		}
		if ( state->window ) {
			SDL_DestroyWindow( state->window );
		}
		state = nullptr;
	}
	if ( sceneMgr ) {
		delete sceneMgr;
	}
}

int wlApplication::Init() {
	if ( !SDL_Init( SDL_INIT_VIDEO ) ) {
		std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
		return SDL_APP_FAILURE;
	}

	state->window = SDL_CreateWindow( "Not Bad Tower Defense", 1280, 720, SDL_WINDOW_RESIZABLE );
	if ( !state->window ) {
		std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
		return SDL_APP_FAILURE;
	}

	state->renderer = SDL_CreateRenderer( state->window, nullptr );
	if ( !state->renderer ) {
		std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
		return SDL_APP_FAILURE;
	}

	sceneMgr->ChangeScene( sceneType_t::TEST_GAME );

	return SDL_APP_CONTINUE;
}

void wlApplication::OnUpdate() {
	SDL_SetRenderDrawColor( state->renderer, 0, 0, 255, 255 );
	SDL_RenderClear( state->renderer );

	Uint64 currentTime = SDL_GetPerformanceCounter();
	deltaTime = static_cast<double>( currentTime - oldTime ) / static_cast<double>(SDL_GetPerformanceFrequency());
	oldTime = currentTime;
	
	sceneMgr->OnUpdate( deltaTime);

	SDL_RenderPresent( state->renderer );
}

void wlApplication::OnEvent( SDL_Event* event ) const {
	sceneMgr->OnEvent( event );
}
