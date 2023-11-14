#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>

class SpinLock
{
public:
	void lock()
	{
		// CAS (Compare-And_Swap)
		bool expected = false;
		bool desired = true;

		while (_locked.compare_exchange_strong(expected, desired) == false)
		{
			expected = false;

			//this_thread::sleep_for(std::chrono::milliseconds(100));
			this_thread::sleep_for(100ms); // 위에 chrono와 ms와 똑같다
			//this_thread::yield(); // 타임 슬라이스 스킵
		}
	}

	void unlock()
	{
		_locked.store(false);
	}
private:
	atomic<bool> _locked = false; 
	// volatile 변수가 여러 스레드, 외부에 의해 변경될수 있으므로 컴파일러 최적화 하지 않음
};

int32 sum = 0;
mutex m;
SpinLock spinLock;

void Add()
{
	for (int32 i = 0; i < 10'0000; ++i)
	{
		lock_guard<SpinLock> guard(spinLock);
		sum++;
	}
}

void Sub()
{
	for (int32 i = 0; i < 10'0000; ++i)
	{
		lock_guard<SpinLock> guard(spinLock);
		sum--;
	}
}

int main()
{
	thread t1(Add);
	thread t2(Sub);

	t1.join();
	t2.join();

	cout << sum << endl;
}