#include "Crystal3D.h"

void main()
{
	try
	{
		Core::CrEngine engine;

		//Initialize Engine
		Core::CrEngineContext ctx;
		engine.Initialize(ctx);

		//Create game Scene
		Scene::CrScene* gameScene = new Scene::CrScene("Scripts\\Scene.lua");

		engine.SetScene(gameScene);
		engine.Run();
	}
	catch (const CrException& e)
	{
		std::cout << e.what() << std::endl;
	}

}