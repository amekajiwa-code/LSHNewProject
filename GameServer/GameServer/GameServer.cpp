#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>

void HelloThread()
{
	std::cout << "Hello thread" << std::endl;
}

int main()
{
	std::thread t(HelloThread);

	std::cout << "Hello main" << std::endl;

	t.join();
}