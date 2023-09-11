#pragma once
#include "Std.h"
#include "fmod.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#pragma comment( lib, "fmod_vc.lib")

class Sound
{
public:
	wstring mName;
	wstring mPath;
	FMOD::System* mSystem;
	FMOD::Sound* mSound;
	FMOD::Channel* mChannel;

	void Set(FMOD::System* system);
	void SetName(wstring key);
	void SetSystem(FMOD::System* system);

	bool Load(wstring fileName);
	void Play(bool isLoop);
	void PlayEffect();
	void Stop();
	void Paused();
	void Volume();
	void VolumeUp();
	void VolumeDown();

	bool Init();
	bool Frame();
	bool Release();
};

class SoundManager
{
private:
	FMOD::System* mSystem;
	unordered_map<wstring, unique_ptr<Sound>> soundMap;
	SoundManager();
public:
	virtual ~SoundManager();

	static SoundManager& GetInstance()
	{
		static SoundManager instance;
		return instance;
	}

	bool Init();
	bool Frame();
	bool Release();
	Sound* Load(wstring filePath);
	Sound* GetPtr(wstring key);
	bool Get(wstring key, Sound& ret);
	unordered_map<string, Sound*> playListMap;
};

