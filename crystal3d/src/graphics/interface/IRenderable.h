#pragma once
#include "graphics\Material.h"
#include "graphics\Mesh.h"
#include "math\AABB.h"
#include "math\Transform.h"
#include "graphics\interface\RenderMode.h"

namespace Graphics
{
	class IRenderable
	{
	public:
		virtual ~IRenderable() {}
		virtual Graphics::CrMaterial * GetMaterial() = 0;
		virtual Graphics::CrMesh * GetMesh() = 0;
		virtual Graphics::ERenderMode GetRenderMode() = 0;
		virtual Math::AABB* GetBoundingBox() = 0;
		virtual Scene::CrTransform* GetTransform() = 0;
	};
}