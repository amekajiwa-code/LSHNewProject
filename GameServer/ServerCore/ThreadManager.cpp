#include "pch.h"
#include "ThreadManager.h"
#include "CoreTLS.h"
#include "CoreGlobal.h"

#pragma region ThreadManager

ThreadManager::ThreadManager()
{
	InitTLS();
}

ThreadManager::~ThreadManager()
{
	Join();
}

void ThreadManager::Launch(function<void(void)> callback)
{
	LockGuard guard(_lock);

	_threads.push_back(thread([=]()
		{
			InitTLS();
			callback();
			DestroyTLS();
		}
	));
}

void ThreadManager::Join()
{
	for (thread& t : _threads)
	{
		if (t.joinable())
		{
			t.join();
		}
		_threads.clear();
	}
}

void ThreadManager::InitTLS()
{
	static Atomic<uint32> SThreadId = 1;
	LThreadId = SThreadId.fetch_add(1);
	//정적 변수로 Id 순차적으로 발급
}

void ThreadManager::DestroyTLS()
{

}

#pragma endregion