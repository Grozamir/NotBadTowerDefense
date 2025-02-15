#include "SceneManager.hpp"

#include "Scene_TestGame.hpp"

void wlSceneManager::ChangeScene( const sceneType_t sceneType ) {

	switch ( sceneType ) {
		case sceneType_t::MAIN_MENU:
			break;
		case sceneType_t::TEST_GAME:
			currentScene = std::make_shared<wlScene_TestGame>();
			break;
		default:
			break;
	}

	if ( currentScene ) {
		currentScene->Start();
	}
	
}

void wlSceneManager::OnUpdate( const double deltaTime) {
	if ( currentScene ) {
		currentScene->OnUpdate( deltaTime );
	}
}

void wlSceneManager::OnEvent( SDL_Event* event ) {
	if ( currentScene ) {
		currentScene->OnEvent( event );
	}
}
