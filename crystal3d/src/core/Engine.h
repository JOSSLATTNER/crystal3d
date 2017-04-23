#pragma once
#include "Core.h"
#include "Platform.h"
#include "GameTimer.hpp"

#include "Configuration.h"
#include "core\allocator\StackAllocator.hpp"

#include "window\interface\IWindow.h"
#include "graphics\interface\IRenderer.h"
#include "input\interface\IInputManager.h"
#include "resources\ResourceManager.h"
#include "scene\Scene.h"

//Global Instances
#define SEngine Core::CrEngine::s_SharedInstance
#define SGameTimer SEngine->GetGameTimer()
#define SEngineConfig SEngine->GetConfiguration()
#define SResourceManager SEngine->GetResourceManager()

namespace Core
{
	class CrEngine
	{
	public:
		CrEngine();
		~CrEngine();

		static CrEngine* s_SharedInstance;

	public:
		void Initialize();
		void Run();
		void SetScene(Scene::CrScene* a_GameScene);
		void Quit();

	public:
		Core::CrConfiguration* GetConfiguration();

		Window::IWindow* GetMainWindow();
		Scene::CrScene* GetCurrentScene();
		Core::CrGameTimer* GetGameTimer();
		Input::IInputManager* GetInputManager();
		Graphics::IRenderer* GetRenderer();
		Resources::CrResourceManager* GetResourceManager();

	private:
		Window::IWindow* m_MainWindow;
		Scene::CrScene* m_ActiveScene;
		Core::CrGameTimer* m_GameTimer;
		Input::IInputManager* m_InputManager;
		Graphics::IRenderer* m_Renderer;
		Resources::CrResourceManager* m_ResourceManager;

	private:
		void Update(const float a_Delta) const;
		void Render();
	
	private:
		CrConfiguration m_Config;
		bool m_IsRunning;

	};
}

