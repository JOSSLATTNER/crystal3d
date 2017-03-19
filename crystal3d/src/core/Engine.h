#pragma once
#include "Core.h"
#include "Platform.h"
#include "GameTimer.hpp"
#include "profile\Benchmark.hpp"

#include "window\interface\IWindow.h"
#include "graphics\interface\IRenderer.h"
#include "input\interface\IInputManager.h"

#include "scene\Scene.h"
#include "resources\ResourceManager.h"

#define SEngine Core::CrEngine::s_SharedInstance

#define MAX_FPS 120
#define ENGINE_WINDOW_WIDTH 1920
#define ENGINE_WINDOW_HEIGHT 1080
#define ENGINE_WINDOW_TITLE "Crystal3D"

namespace Core
{
	class CrEngine
	{
	public:
		CrEngine();
		~CrEngine();

		static CrEngine* s_SharedInstance;

	public:
		bool Initialize();
		void Run();
		void SetScene(Scene::CrScene* a_GameScene);
		void Quit();

	public:
		Window::IWindow* GetMainWindow();
		Scene::CrScene* GetCurrentScene();
		Core::CrGameTimer* GetGameTimer();
		Resources::CrResourceManager* GetResourceManager();
		Input::IInputManager* GetInputManager();
		Graphics::IRenderer* GetRenderer();

	private:
		Window::IWindow* m_MainWindow;
		Scene::CrScene* m_ActiveScene;
		Core::CrGameTimer* m_GameTimer;
		Resources::CrResourceManager* m_ResourceManager;
		Input::IInputManager* m_InputManager;
		Graphics::IRenderer* m_Renderer;

	private:
		void Update(const float& delta) const;
		void Render();
	
	private:
		bool m_IsRunning;

	};
}

