#include "Crystal3D.h"

void main()
{
	auto engine = Core::CrEngine::SharedInstance();
	engine->Initialize();

	Scene::CrScene* gameScene = new Scene::CrScene("Scripts\\Scene.lua");

	engine->SetScene(gameScene);
	engine->Run();

	engine->Release();
}