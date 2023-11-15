#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>

mutex m;
queue<int32> q;
HANDLE handle;

condition_variable cv; // 유저모드에서 작동

void Producer()
{
	while (true)
	{
		{
			unique_lock<mutex> lock(m);
			q.push(100);
		}

		cv.notify_one(); // wait중인 하나의 스레드를 깨움

		//::SetEvent(handle);

		this_thread::sleep_for(100ms);
	}
}

void Consumer()
{
	while (true)
	{
		unique_lock<mutex> lock(m);
		//::WaitForSingleObject(handle, INFINITE);
		cv.wait(lock, []() { return q.empty() == false; }); 
		// lock을 먼저 잡고있다가 조건을 만족하지 않으면 lock을 풀고 대기상태로 들어간다
		// wait 깨어나는 조건 : queue가 비어있지 않을때
		int32 data = q.front();
		q.pop();
		cout << q.size() << endl;
	}
}

int main()
{
	handle = ::CreateEvent(NULL, FALSE, FALSE, NULL); // 1. 보안속성 2. 메뉴얼리셋여부 3. 상태

	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();

	::CloseHandle(handle);
}