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

// CoreGlobal GCoreGlobal �ѰͰ� ��ϵڿ� ���̴°��� ������ ����.;