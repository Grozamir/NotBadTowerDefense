#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

#include <iostream>

#include "Application.hpp"

wlApplication app;

SDL_AppResult SDL_AppInit( void **appstate, int argc, char **argv ) {
	if ( app.Init() == SDL_APP_FAILURE ) {
		return SDL_APP_FAILURE;
	}
	*appstate = app.GetState();

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate( void *appstate ) {
	app.OnUpdate();
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent( void *appstate, SDL_Event *event ) {
	if ( event ) {
		app.OnEvent( event );
	}

	if ( event && event->type == SDL_EVENT_QUIT ) {
		return SDL_APP_SUCCESS;
	}
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit( void *appstate, SDL_AppResult result ) {
	auto *const state = static_cast<wlAppState *>( appstate );
	if ( state ) {
		if ( state->renderer ) {
			SDL_DestroyRenderer( state->renderer );
		}
		if ( state->window ) {
			SDL_DestroyWindow( state->window );
		}
		delete state;
	}
}
