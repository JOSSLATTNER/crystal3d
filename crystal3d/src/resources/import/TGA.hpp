#pragma once
#include "Import.hpp"
#include "core\Core.h"

namespace Resources
{
	namespace Import
	{
		namespace TGA
		{
			enum ImageType 
				: CrByte
			{
				NO_IMAGE_DATA = 0,
				INDEXED_UNCOMPRESSED = 1,
				RGB24_UNCOMPRESSED = 2,
				MONO_UNCOMPRESSED = 3,
				INDEXED_RLE = 9,
				RGB24_RLE = 10,
				MONO_RLE = 11
			};


#pragma pack(push)
#pragma pack(1)
			struct Header
			{
				CrByte idLength;
				CrByte paletteType;
				ImageType type;
				CrWord paletteStart;
				CrWord paletteLength;
				CrByte paletteEntrySize;
				CrWord originCoordX;
				CrWord originCoordY;
				CrWord width;
				CrWord height;
				CrByte bitCount;
				CrByte attributeByte;
			};
#pragma pack(pop)


			struct ImageInfo
			{
				unsigned int width;
				unsigned int height;
				bool hasAlpha;
			};

			static constexpr size_t kHeaderSize = sizeof(char);

			//http://paulbourke.net/dataformats/tga/
			inline void DecompressRLE(unsigned int bytePerPixel, std::vector<char>& a_In, std::vector<char>& a_Out)
			{
				a_Out.resize(a_In.size());
				
				for (size_t i = 0; i < a_In.size();)
				{
					char cc = a_In[i];

					char type = (cc & 0xff) >> 7;
					char count = cc & 7;

					if (type == 1)
					{
						//For the run length packet, the header is followed by
						//a single color value, which is assumed to be repeated
						//the number of times specified in the header.

						char* packetStart = &cc + kHeaderSize;
						char* packetEnd = packetStart + bytePerPixel;

						//Insert packets into output buffer
						for (size_t i = 0; i < count; i++)
						{
							a_Out.insert(a_Out.end(), packetStart, packetEnd);
						}

						//Jump to next header
						i += (kHeaderSize + bytePerPixel);
					}
					else
					{
						//For the raw packet, the header is followed by
						//the number of color values specified in the header.

						char* packetsStart = &cc + kHeaderSize;
						char* packetsEnd = packetsStart + count * bytePerPixel;

						//Insert packets into output buffer
						a_Out.insert(a_Out.end(), packetsStart, packetsEnd);

						//Jump to next header
						i += (kHeaderSize + count * bytePerPixel);
					}
				}
			}

			inline void Import(const std::string& a_Filename, std::vector<char>& a_Buffer, ImageInfo& a_Info)
			{
				std::ifstream ifs(a_Filename, std::ifstream::in | std::ifstream::binary);

				if (!ifs.good())
					throw CrImportException("Failed to read file.");

				//Read Header
				Header hd;
				ifs.read(reinterpret_cast<char*>(&hd), sizeof(Header));
				if (hd.type != RGB24_UNCOMPRESSED
						&& hd.type != MONO_UNCOMPRESSED
						&& hd.type != RGB24_RLE
						&& hd.type != MONO_RLE)
					throw CrImportException("RGB24_UNCOMPRESSED, RGB24_RLE, MONO_UNCOMPRESSED, MONO_RLE supported.");

				//Find Pixel Format: 3 = BGR, 4 = BGRA
				unsigned int bytePerPixel = hd.bitCount / 8;
				if (bytePerPixel != 3 && bytePerPixel != 4)
					throw CrImportException("Invalid bitCount!");

				//Fill image info
				a_Info.hasAlpha = bytePerPixel == 4;
				a_Info.height = hd.height;
				a_Info.width = hd.width;

				size_t imageSize = hd.width * hd.height * bytePerPixel;

				//Read pixel data
				a_Buffer.resize(imageSize);
				ifs.read(a_Buffer.data(), imageSize);

				//Decompress
				if (hd.type == RGB24_RLE || hd.type == MONO_RLE)
				{
					std::vector<char> decompressed;
					DecompressRLE(bytePerPixel, a_Buffer, decompressed);

					//Swap decompressed data into output buffer 
					a_Buffer.swap(decompressed);
				}

				////Adjust Format (RGB/BGR)
				//if (format == ReadFormat::RGB)
				//{
				//	unsigned char colorSwap;
				//	for (int imageIdx = 0; imageIdx < a_Buffer.size(); imageIdx += bytePerPixel)
				//	{
				//		colorSwap = a_Buffer[imageIdx];
				//		a_Buffer[imageIdx] = a_Buffer[imageIdx + 2];
				//		a_Buffer[imageIdx + 2] = colorSwap;
				//	}
				//}
			}

		}
	}
}