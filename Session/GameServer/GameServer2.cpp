#include "pch.h"
#include <iostream>

#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWriter.h"
#include "ServerPacketHandler.h"

int main()
{
	ServerServiceRef service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<GameSession>, // TODO : SessionManager 등
		100);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}

	// TODO : 클라가 pos 이동할때 변화값 서버한테 패킷보내고 id별로 구분해서 서버가 세션 목록 출력하기
	char sendData[] = "Hello World";

	while (true)
	{
		cout << "SessionCount : " << service->GetCurrentSessionCount() << endl;

		for (GameSessionRef player : GSessionManager.GetSessionsRef())
		{
			SendBufferRef sendBuffer = ServerPacketHandler::Make_USER_INFO(player->GetSessionId(), 1.0f, 1.5f, -1.0f);
			GSessionManager.Broadcast(sendBuffer);
		}		

		this_thread::sleep_for(1000ms);
	}     

	GThreadManager->Join();
}