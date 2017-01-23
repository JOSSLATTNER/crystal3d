#pragma once
#include "Core.h"
#include "Platform.h"
#include "GameTimer.hpp"

#include "window\interface\IWindow.h"
#include "graphics\interface\IRenderer.h"
#include "input\interface\IInputManager.h"

#include "scene\Scene.h"
#include "resources\ResourceManager.h"

#define SEngine Core::CrEngine::SharedInstance()

namespace Core
{
	class CrEngine final 
		: public Core::CrSingleton<CrEngine>
	{
	public:
		CrEngine();
		~CrEngine();

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
		void Update(float& delta) const;
		void Render();
	
	private:
		bool m_IsRunning;

	};
}

