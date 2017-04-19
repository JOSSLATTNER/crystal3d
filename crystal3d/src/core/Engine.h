#pragma once
#include "Core.h"
#include "Platform.h"
#include "GameTimer.hpp"

#include "window\interface\IWindow.h"
#include "graphics\interface\IRenderer.h"
#include "input\interface\IInputManager.h"

#include "scene\Scene.h"

//Global engine instance
#define SEngine Core::CrEngine::s_SharedInstance

namespace Core
{
	struct CrEngineContext
	{
		uint32_t maxFps = 100U;
		std::string windowTitle = "sandbox|60fps";
		glm::ivec2 windowDimensions = {1920, 1080};
		bool windowFullscreen = false;
	};

	class CrEngine
	{
	public:
		CrEngine();
		~CrEngine();

		static CrEngine* s_SharedInstance;

	public:
		void Initialize(CrEngineContext& a_Context);
		void Run();
		void SetScene(Scene::CrScene* a_GameScene);
		void Quit();

	public:
		Window::IWindow* GetMainWindow();
		Scene::CrScene* GetCurrentScene();
		Core::CrGameTimer* GetGameTimer();
		Input::IInputManager* GetInputManager();
		Graphics::IRenderer* GetRenderer();

	private:
		Window::IWindow* m_MainWindow;
		Scene::CrScene* m_ActiveScene;
		Core::CrGameTimer* m_GameTimer;
		Input::IInputManager* m_InputManager;
		Graphics::IRenderer* m_Renderer;

	private:
		void Update(const float delta) const;
		void Render();
	
	private:
		CrEngineContext m_Context;
		bool m_IsRunning;

	};
}

