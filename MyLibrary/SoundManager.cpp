#include "SoundManager.h"
#include <tchar.h>
#include "Writer.h"

bool Sound::Init()
{
	return false;
}

bool Sound::Frame()
{
	//TCHAR mBuffer[256] = { 0, };
	//unsigned int ms = 0; //밀리초
	//unsigned int total = 0;
	//mChannel->getPosition(&ms, FMOD_TIMEUNIT_MS);
	//mSound->getLength(&total, FMOD_TIMEUNIT_MS);
	//// 버퍼에 문자열 저장 오버플로우 방지
	//_stprintf_s(mBuffer, _T(" 경과시간[%02d:%02d:%02d], 파일크기[%02d:%02d:%02d]"),
	//	ms / 1000 / 60,
	//	ms / 1000 % 60,
	//	ms / 10 % 60,
	//	total / 1000 / 60,
	//	total / 1000 % 60,
	//	total / 10 % 60);
	//Writer::GetInstance().AddText(mBuffer, 50, 50, 100);
	return true;
}

bool Sound::Release()
{
	if (mSound) mSound->release();

	return true;
}

void Sound::Set(FMOD::System* system)
{
	mSystem = system;
}

void Sound::SetName(wstring key)
{
	mName = key;
}

void Sound::SetSystem(FMOD::System* system)
{
	mSystem = system;
}

bool Sound::Load(wstring fileName)
{
	wstring_convert<codecvt_utf8_utf16<wchar_t>> conv;
	string file = conv.to_bytes(fileName);
	FMOD_RESULT hr = mSystem->createSound(file.c_str(), FMOD_DEFAULT, 0, &mSound);

	return true;
}

void Sound::Play(bool isLoop)
{
	if (mChannel == nullptr)
	{
		if (isLoop)
		{
			mSound->setMode(FMOD_LOOP_NORMAL);
		}
		else
		{
			mSound->setMode(FMOD_LOOP_OFF);
		}
		mSystem->playSound(mSound, nullptr, false, &mChannel);
	}
	else
	{
		bool isPlay;
		mChannel->isPlaying(&isPlay);
		if (isPlay == false)
		{
			if (isLoop)
			{
				mSound->setMode(FMOD_LOOP_NORMAL);
			}
			else
			{
				mSound->setMode(FMOD_LOOP_OFF);
			}
			mSystem->playSound(mSound, nullptr, false, &mChannel);
		}
	}
}

void Sound::PlayEffect()
{
	mSystem->playSound(mSound, nullptr, false, &mChannel);
}

void Sound::Stop()
{
	mChannel->stop();
}

void Sound::Paused()
{
	bool isPaused;
	mChannel->getPaused(&isPaused);
	mChannel->setPaused(!isPaused);
}

void Sound::Volume()
{
	//
}

void Sound::VolumeUp()
{
	float volume;
	mChannel->getVolume(&volume);
	volume += 0.25f * g_SecondPerFrame;
	volume = min(1.0f, volume);
	mChannel->setVolume(volume);
}

void Sound::VolumeDown()
{
	float volume;
	mChannel->getVolume(&volume);
	volume -= 0.40f;
	volume = max(0.0f, volume);
	mChannel->setVolume(volume);
}

SoundManager::SoundManager()
{
	FMOD::System_Create(&mSystem);
	mSystem->init(32, FMOD_INIT_NORMAL, 0);
}

SoundManager::~SoundManager()
{
	Release();
}

bool SoundManager::Init()
{
	return false;
}

bool SoundManager::Frame()
{
	mSystem->update();
	for (auto& data : soundMap)
	{
		data.second->Frame();
	}

	return true;
}

bool SoundManager::Release()
{
	FMOD::System_Create(&mSystem);
	mSystem->init(32, FMOD_INIT_NORMAL, 0);

	return true;
}

Sound* SoundManager::Load(wstring filePath)
{
	size_t found = filePath.find_last_of(L"/");
	wstring path = filePath.substr(0, found + 1);
	wstring key = filePath.substr(found + 1);
	Sound* data = GetPtr(key);

	if (data != nullptr)
	{
		return data;
	}

	unique_ptr<Sound> newData = make_unique<Sound>();
	newData->Set(mSystem);
	newData->SetName(key);

	if (newData->Load(filePath))
	{
		soundMap.insert(make_pair(key, move(newData))); // newData의 소유권을 soundMap의 값에 넘김
		return GetPtr(key);
	}

	return nullptr;
}

Sound* SoundManager::GetPtr(wstring key)
{
	auto iter = soundMap.find(key);
	if (soundMap.end() == iter)
	{
		return nullptr;
	}
	return iter->second.get();
}

bool SoundManager::Get(wstring key, Sound& ret)
{
	auto iter = soundMap.find(key);
	if (soundMap.end() == iter)
	{
		return false;
	}
	ret = *(iter->second);
	return true;
}