#include "pch.h"
#include "CorePch.h"

#include <iostream>

#include <thread>
#include <windows.h>
#include <future>

#include "CoreMacro.h"
#include "ThreadManager.h"

CoreGlobal Core;

void ThreadMain()
{
	while (true)
	{
		cout << "Hello !! am thread..." << LThreadId << endl;
		this_thread::sleep_for(1s);
	}
}

int main()
{
	/*for (int32 i = 0; i < 5; ++i)
	{
		GThreadManager->Launch(ThreadMain);
	}

	GThreadManager->Join();*/

	auto add = [](int a, int b) { return a + b; };

	cout << add(3, 4) << endl;
}