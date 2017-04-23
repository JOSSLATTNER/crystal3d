#pragma once

namespace Graphics
{
	enum class EShaderType
	{
		VertexShader,
		FragmentShader,
		GeometryShader
	};

	class IShader
	{
	public:
		virtual ~IShader() = default;

	};
}