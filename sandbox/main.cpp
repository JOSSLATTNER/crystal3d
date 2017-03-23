#include "Crystal3D.h"

void main()
{
	try
	{
		Core::CrEngine engine;
		Core::CrEngineContext ctx;

		engine.Initialize(ctx);

		Scene::CrScene* gameScene = new Scene::CrScene("Scripts\\Scene.lua");

		engine.SetScene(gameScene);
		engine.Run();
	}
	catch (const CrException& e)
	{
		std::cout << e.what() << std::endl;
	}

}