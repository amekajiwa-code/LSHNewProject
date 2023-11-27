#include "pch.h"
#include "CorePch.h"

#include <iostream>

#include <thread>
#include <windows.h>
#include <future>

#include "ThreadManager.h"

class TestLock
{
	USE_LOCK;

public:
	int32 TestRead()
	{
		READ_LOCK;

		if (_queue.empty())
		{
			return -1;
		}
		else
		{
			return _queue.front();
		}
	}

	void TestPush()
	{
		WRITE_LOCK;

		_queue.push(rand() % 100);
	}

	void TestPop()
	{
		WRITE_LOCK;

		if (_queue.empty() == false)
		{
			_queue.pop();
		}
	}
private:
	queue<int32> _queue;
};

TestLock testLock;

void ThreadWrite()
{
	while (true)
	{
		testLock.TestPush();
		this_thread::sleep_for(1ms);
		testLock.TestPop();
	}
}

void ThreadRead()
{
	while (true)
	{
		int32 value = testLock.TestRead();
		cout << value << endl;
		this_thread::sleep_for(1ms);
	}
}

//int main()
//{
//	for (int32 i = 0; i < 5; ++i)
//	{
//		cout << i << endl;
//	}
//	/*for (int32 i = 0; i < 2; ++i)
//	{
//		GThreadManager->Launch(ThreadWrite);
//	}
//
//	for (int32 i = 0; i < 5; ++i)
//	{
//		GThreadManager->Launch(ThreadRead);
//	}
//
//	GThreadManager->Join();*/
//}

#include <iostream>
#include <mutex>
#include <shared_mutex>

std::shared_mutex mySharedMutex;

void ReadData()
{
	std::shared_lock<std::shared_mutex> lock(mySharedMutex);
	// 여러 스레드에서 동시에 읽기 수행
	std::cout << "Reading data..." << std::endl;
}

void WriteData()
{
	std::unique_lock<std::shared_mutex> lock(mySharedMutex);
	// 쓰기 작업 수행
	std::cout << "Writing data..." << std::endl;
}

int main()
{
	// 여러 스레드가 동시에 읽고 쓰는 예제
	std::thread reader1(ReadData);
	std::thread reader2(ReadData);
	std::thread writer(WriteData);

	reader1.join();
	reader2.join();
	writer.join();

	return 0;
}
