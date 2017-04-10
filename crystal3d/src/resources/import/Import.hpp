#pragma once
#include "Common.h"
#include "TGA.hpp"
#include "OBJ.hpp"

namespace Resources
{
	namespace Import
	{
		inline const std::string _GetExtension(const std::string& a_Filename)
		{
			auto idx = a_Filename.find('.');
			if (idx != std::string::npos)
			{
				return a_Filename.substr(idx + 1);
			}
			return "?";
		}

		inline void ImportTexture(const std::string& a_Filename, std::vector<CrByte>& a_Buffer, CrImageInfo& a_Info)
		{
			const std::string extension = _GetExtension(a_Filename);

			if (extension == "tga")
			{
				TGA::Import(a_Filename, a_Buffer, a_Info);
			}
			else
			{
				throw CrImportException("Unknown file extension.");
			}
		}

		inline Graphics::CrModel* ImportModel(const std::string& a_Filename)
		{
			const std::string extension = _GetExtension(a_Filename);

			if (extension == "obj")
			{
				return OBJ::Import(a_Filename);
			}
			else
			{
				throw CrImportException("Unknown file extension.");
			}

		}

	}
}