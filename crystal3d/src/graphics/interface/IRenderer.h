#pragma once
#include "scene\Scene.h"
#include "graphics\interface\RendererContext.h"
#include "graphics\interface\IRenderable.h"
#include "graphics\interface\IResourceFactory.h"

namespace Graphics
{
	class IRenderer
	{
	public:
		virtual ~IRenderer() {}
		virtual void Initialize(CrRendererContext& a_RendererContext) = 0;
		virtual void Render(Scene::CrScene* a_Scene) = 0;
		virtual void LoadAssets(Scene::CrScene* a_Scene) = 0;
		virtual IResourceFactory* CreateFactory() = 0;
	};
}