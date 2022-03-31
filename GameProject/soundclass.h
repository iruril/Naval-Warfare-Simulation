///////////////////////////////////////////////////////////////////////////////
// Filename: soundclass.h
///////////////////////////////////////////////////////////////////////////////
#ifndef _SOUNDCLASS_H_
#define _SOUNDCLASS_H_


/////////////
// LINKING //
/////////////
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")


//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>


///////////////////////////////////////////////////////////////////////////////
// Class name: SoundClass
///////////////////////////////////////////////////////////////////////////////
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

	bool Initialize(HWND, char*, char*, char*, char*, char*, char*, char*, char*, char*, char*, char*, char*);
	void Shutdown();

	bool playBGM();

	bool playFlag();
	bool playContainer();
	bool playPitch();
	bool playCollision();
	bool playSE_Engine();
	bool playSE_Wave();

	bool volumeSE_Engine(int);
	bool volumeSE_Wave(int);

	bool playSuccess();
	bool playFail();

	bool playUIOn();
	bool playUIOff();

	bool playFire(int);
	
	void stopFile(int);

private:
	bool InitializeDirectSound(HWND);
	void ShutdownDirectSound();

	bool LoadWaveFile(char*, IDirectSoundBuffer8**);
	void ShutdownWaveFile(IDirectSoundBuffer8**);
	

private:
	IDirectSound8* m_DirectSound;
	IDirectSoundBuffer* m_primaryBuffer;
	IDirectSoundBuffer8* m_secondaryBuffer1;//bgm
	IDirectSoundBuffer8* m_secondaryBuffer2;//flag
	IDirectSoundBuffer8* m_secondaryBuffer3;//container
	IDirectSoundBuffer8* m_secondaryBuffer4;//pitch
	IDirectSoundBuffer8* m_secondaryBuffer5;//collision
	IDirectSoundBuffer8* m_secondaryBuffer6;//engine
	IDirectSoundBuffer8* m_secondaryBuffer7;//wave
	IDirectSoundBuffer8* m_secondaryBuffer8;//success
	IDirectSoundBuffer8* m_secondaryBuffer9;//fail
	IDirectSoundBuffer8* m_secondaryBuffer10;//ui on
	IDirectSoundBuffer8* m_secondaryBuffer11;//ui off
	IDirectSoundBuffer8* m_secondaryBuffer12;//fire
};

#endif