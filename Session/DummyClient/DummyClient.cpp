//#include "pch.h"
//
//#include "ThreadManager.h"
//#include "Service.h"
//#include "Session.h"
//
//char sendData[] = "Hello World";
//
//class ServerSession : public Session
//{
//public:
//	~ServerSession()
//	{
//		cout << "~ServerSession" << endl;
//	}
//
//	//콘텐츠 코드에서 재정의 하고있음
//	/*최초로 연결이 되있을떄 OnConnected에서 Send를 하고
//	서버에서 OnRecv를 한뒤 에코로*/
//	virtual void OnConnected() override
//	{
//		cout << "Connected To Server!" << endl;
//
//		SendBufferRef sendBuffer = MakeShared<SendBuffer>(4096); //4kb
//		sendBuffer->CopyData(sendData, sizeof(sendData));
//		Send(sendBuffer);
//	}
//
//	virtual void OnDisconnected() override
//	{
//		cout << "Disconnected..." << endl;
//	}
//
//	virtual int32 OnRecv(BYTE* buffer, int32 len) override
//	{
//		//Echo
//		cout << "OnRecv Len = " << len <<endl;
//
//		this_thread::sleep_for(1s);
//
//		SendBufferRef sendBuffer = MakeShared<SendBuffer>(4096); //4kb
//		sendBuffer->CopyData(sendData, sizeof(sendData));
//		Send(sendBuffer);
//
//		return len;
//	}
//
//	virtual void OnSend(int32 len) override
//	{
//		cout << "OnSend Len = " << len << endl;
//	}
//};
//
//int main()
//{
//	this_thread::sleep_for(1s);
//
//	ClientServiceRef service = MakeShared<ClientService>(
//		NetAddress(L"127.0.0.1", 7777),
//		MakeShared<IocpCore>(),
//		MakeShared<ServerSession>,
//		1);
//
//	ASSERT_CRASH(service->Start());
//
//	for (int32 i = 0; i < 2; i++)
//	{
//		GThreadManager->Launch([=]()
//			{
//				while (true)
//				{
//					service->GetIocpCore()->Dispatch();
//				}
//			});
//	}
//
//	GThreadManager->Join();
//}