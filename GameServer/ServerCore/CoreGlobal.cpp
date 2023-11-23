#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"

ThreadManager* GThreadManager = nullptr;

class CoreGlobal
{
public:
	CoreGlobal()
	{
		GThreadManager = new ThreadManager();
	}
	~CoreGlobal()
	{
		delete GThreadManager;
	}
private:
} GCoreGlobal;

// CoreGlobal GCoreGlobal 한것과 블록뒤에 붙이는것이 문법이 같다.;