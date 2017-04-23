#include "Crystal3D.h"

void main()
{
	try
	{
		Core::CrEngine engine;
		engine.Initialize();

		//Create game Scene
		Scene::CrScene* gameScene = new Scene::CrScene("Scripts\\Scene.lua");

		engine.SetScene(gameScene);
		engine.Run();
	}
	catch (const CrException& e)
	{
		CrAssert(0, e.what());
	}

}