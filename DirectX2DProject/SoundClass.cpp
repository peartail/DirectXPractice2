#include "SoundClass.h"

#include <ios>
#include <cstdio>


SoundClass::SoundClass()
{
	_directSound = NULL;
	_primaryBuffer = NULL;
	_secondaryBuffer1 = NULL;

	_waveBuffers.clear();
}

SoundClass::SoundClass(const SoundClass& other)
{

}

SoundClass::~SoundClass()
{
}

bool SoundClass::Initialize(HWND hwnd)
{
	bool result;

	result = InitializeDirectSound(hwnd);
	if (!result)
	{
		return false;
	}

	result = LoadWaveFile("./Sound/f1.wav", &_secondaryBuffer1);
	if (!result)
	{
		return false;
	}

	std::string file1 = "catapult_w.wav";

	std::string path = "./Sound/";

	_waveBuffers[file1] = NULL;

	result = LoadWaveFile((path + file1).c_str(), &_waveBuffers[file1]);
	if (!result)
	{
		return false;
	}
	//result = LoadWaveFile()

	result = PlayWaveFile();
	if (!result)
	{
		return false;
	}

	return true;
}

void SoundClass::Shutdown()
{
	for (auto it = _waveBuffers.begin(); it != _waveBuffers.end(); ++it)
	{
		auto obj = (*it).second;
		if (obj)
		{
			ShutdownWaveFile(&obj);
		}
	}
	_waveBuffers.clear();

	ShutdownWaveFile(&_secondaryBuffer1);

	ShutdownDirectSound();
}

bool SoundClass::InitializeDirectSound(HWND hwnd)
{
	HRESULT result;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;

	result = DirectSoundCreate8(NULL, &_directSound, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = _directSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if (FAILED(result))
	{
		return false;
	}

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	result = _directSound->CreateSoundBuffer(&bufferDesc, &_primaryBuffer, NULL);
	if (FAILED(result))
	{
		return false;
	}

	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	result = _primaryBuffer->SetFormat(&waveFormat);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void SoundClass::ShutdownDirectSound()
{
	if (_primaryBuffer)
	{
		_primaryBuffer->Release();
		_primaryBuffer = NULL;
	}

	if(_directSound)
	{
		_directSound->Release();
		_directSound = NULL;
	}
}

bool SoundClass::LoadWaveFile(const char* filename, IDirectSoundBuffer8** secondaryBuffer)
{
	int err;
	FILE* filePtr;
	unsigned int count;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;

	err = fopen_s(&filePtr, filename, "rb");
	if (err != 0)
	{
		return false;
	}

	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// RIFF 포맷인지 체크(근데 이게 뭐야?)
	if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') || (waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
	{
		return false;
	}


	if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
	{
		return false;
	}

	if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
	{
		return false;
	}

	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		return false;
	}

	if (waveFileHeader.numChannels != 2)
	{
		return false;
	}

	if (waveFileHeader.sampleRate != 44100)
	{
		return false;
	}

	if (waveFileHeader.bitsPerSample != 16)
	{
		return false;
	}

	if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a')
		|| (waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
	{
		return false;
	}

	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8)*waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	result = _directSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer);
	if (FAILED(result))
	{
		return false;
	}

	tempBuffer->Release();
	tempBuffer = NULL;

	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData)
	{
		return false;
	}

	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize)
	{
		return false;
	}

	err = fclose(filePtr);
	if (err)
	{
		return false;
	}

	result = (*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if (FAILED(result))
	{
		return false;
	}

	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	result = (*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if (FAILED(result))
	{
		return false;
	}

	delete[] waveData;
	waveData = NULL;

	return true;

}

void SoundClass::ShutdownWaveFile(IDirectSoundBuffer8** seBuffer)
{

	if (*seBuffer)
	{
		(*seBuffer)->Release();
		*seBuffer = NULL;
	}
}

bool SoundClass::PlayWaveFile()
{
	HRESULT result;

	result = _secondaryBuffer1->SetCurrentPosition(0);
	if (FAILED(result))
	{
		return false;
	}

	result = _secondaryBuffer1->SetVolume(-2000);
	if (FAILED(result))
	{
		return false;
	}

	result = _secondaryBuffer1->Play(0, 0, DSBPLAY_LOOPING);
	if (FAILED(result))
	{
		return false;
	}
	
	return true;
}

bool SoundClass::PlayWaveFileofKey(std::string key)
{
	HRESULT result;
	
	

	auto value = _waveBuffers[key];

	if (value)
	{
		
		/*	result = value->SetCurrentPosition(0);
			if (FAILED(result))
			{
				return false;
			}
		}
		else*/
		{
			result = value->SetCurrentPosition(0);
			if (FAILED(result))
			{
				return false;
			}

			result = value->SetVolume(-2000);
			if (FAILED(result))
			{
				return false;
			}

			result = value->Play(0, 0, 0);
			if (FAILED(result))
			{
				return false;
			}
		}
		return true;
	}
	return false;
}