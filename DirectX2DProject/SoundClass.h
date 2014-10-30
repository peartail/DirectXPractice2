#pragma once

#ifndef __SOUNDCLASS_H__
#define __SOUNDCLASS_H__

#pragma comment(lib,"dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib,"winmm.lib")

#include <Windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

#include <map>

using namespace std;

class SoundClass
{
private:
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};
public:
	SoundClass();
	SoundClass(const SoundClass&);
	~SoundClass();

	bool Initialize(HWND);
	void Shutdown();

	bool SoundClass::PlayWaveFileofKey(std::string key);
private:
	bool InitializeDirectSound(HWND);
	void ShutdownDirectSound();

	bool LoadWaveFile(const char*, IDirectSoundBuffer8**);
	void ShutdownWaveFile(IDirectSoundBuffer8**);

	bool PlayWaveFile();
	
private:
	IDirectSound8* _directSound;
	IDirectSoundBuffer* _primaryBuffer;

	IDirectSoundBuffer8* _secondaryBuffer1;

	map<std::string,IDirectSoundBuffer8*> _waveBuffers;
};


#endif