#include "Crystal3D.h"

void main()
{
	Core::CrEngine engine;
	engine.Initialize();

	Scene::CrScene* gameScene = new Scene::CrScene("Scripts\\Scene.lua");

	engine.SetScene(gameScene);
	engine.Run();
}