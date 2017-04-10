#pragma once
#include "AudioFile.h"

namespace Audio
{
	class IAudioSystem
	{
	public:
		virtual ~IAudioSystem() = default;
		virtual void Initialize() = 0;
		virtual void Play(CrAudioFile* a_File) = 0;

	};
}