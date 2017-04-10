#pragma once
#include "audio\interface\IAudioSystem.h"
#include "AL.h"

namespace Audio
{
	class ALAudioSystem :
		public IAudioSystem
	{
	public:
		ALAudioSystem();

		//Inherited via IAudioSystem
		~ALAudioSystem() override;
		void Initialize() override;
		void Play(CrAudioFile* a_File) override;

	};
}