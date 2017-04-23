#include "GLResourceFactory.h"

namespace Graphics
{
	namespace OpenGL
	{
		GLResourceFactory::GLResourceFactory()
		{
		}

		GLResourceFactory::~GLResourceFactory()
		{
		}

		ITexture * GLResourceFactory::LoadTexture(const std::vector<CrByte>& a_Data, unsigned int a_Width, unsigned int a_Height, unsigned int a_BitCount)
		{
			const GLvoid* data = reinterpret_cast<const GLvoid*>(a_Data.data());
			bool hasAlpha = (a_BitCount / 8) == 4;
		
			GLTextureContext context;
			context.aniso = false;
			context.height = a_Height;
			context.width = a_Width;
			context.sourceFormat = hasAlpha ? GL_BGRA : GL_BGR; //TODO: REVERSE
			context.internalFormat = hasAlpha ? GL_RGBA : GL_RGB;
			context.type = GL_UNSIGNED_BYTE;

			return new GLTexture(data, context);
		}
		IShader * GLResourceFactory::LoadShader(const std::string& a_Source, EShaderType a_Type)
		{
			const GLchar* source = reinterpret_cast<const GLchar*>(a_Source.c_str());
			GLenum type = GL_INVALID_ENUM;

			switch (a_Type)
			{
				case EShaderType::VertexShader:
				{
					type = GL_VERTEX_SHADER;
					break;
				}
				case EShaderType::FragmentShader:
				{
					type = GL_FRAGMENT_SHADER;
					break;
				}
				case EShaderType::GeometryShader:
				{
					type = GL_GEOMETRY_SHADER;
					break;
				}
				default:
				{
					throw CrException("Unknown shader type.");
				}
			}

			return new GLShader(source, type);
		}
	}
}