#include "Application.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include "SDL3_ttf/SDL_ttf.h"

#include <iostream>

#include "resources/Resources.hpp"

#include "scenes/SceneManager.hpp"

wlApplication::wlApplication() : 
							state	{ new wlAppState },
							sceneMgr{ new wlSceneManager } {
	appState = state;
	appState->app = this;
	oldTime = SDL_GetPerformanceCounter();
}

wlApplication::~wlApplication() {
	wlResources::Unload();

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

	if ( !TTF_Init() ) {
		std::cerr << "TTF_Init failed: " << SDL_GetError() << std::endl;
		return SDL_APP_FAILURE;
	}

	state->textEngine = TTF_CreateRendererTextEngine( state->renderer );

	wlResources::Load();

	sceneMgr->ChangeScene( sceneType_t::MAIN_MENU );

	return SDL_APP_CONTINUE;
}

void wlApplication::OnUpdate() {
	SDL_SetRenderDrawColor( state->renderer, 0, 0, 255, 255 );
	SDL_RenderClear( state->renderer );

	const Uint64 currentTime = SDL_GetPerformanceCounter();
	deltaTime = static_cast<double>( currentTime - oldTime ) / static_cast<double>(SDL_GetPerformanceFrequency());
	oldTime = currentTime;
	
	sceneMgr->OnUpdate( deltaTime);

	SDL_RenderPresent( state->renderer );
}

void wlApplication::OnEvent( SDL_Event* event ) const {
	if (event->type == SDL_EVENT_WINDOW_RESIZED) {
		state->currentWidthScreen	= event->window.data1;
		state->currentHeightScreen	= event->window.data2;
	}
	sceneMgr->OnEvent( event );
}
