#include "SceneManager.hpp"

#include "Scene_MainMenu.hpp"
#include "Scene_TestGame.hpp"

void wlSceneManager::ChangeScene( const sceneType_t sceneType ) {
	nextScene = sceneType;
	needChangeScene = true;
}

void wlSceneManager::OnUpdate( const double deltaTime) {
	if ( currentScene ) {
		currentScene->OnUpdate( deltaTime );
	}

	if (needChangeScene) {
		LoadNextScene();
	}
}

void wlSceneManager::OnEvent( SDL_Event* event ) {
	if ( currentScene ) {
		currentScene->OnEvent( event );
	}
}

void wlSceneManager::LoadNextScene() {
	if (!needChangeScene) {
		return;
	}

	switch ( nextScene ) {
		case sceneType_t::MAIN_MENU:
			currentScene = std::make_shared<wlScene_MainMenu>();
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
	needChangeScene = false;
}
