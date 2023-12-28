#include "pch.h"

#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "BufferReader.h"

char sendData[] = "Hello World";

class ServerSession : public PacketSession
{
public:
	~ServerSession()
	{
		cout << "~ServerSession" << endl;
	}

	//콘텐츠 코드에서 재정의 하고있음
	/*최초로 연결이 되있을떄 OnConnected에서 Send를 하고
	서버에서 OnRecv를 한뒤 에코로*/
	virtual void OnConnected() override
	{
		// cout << "Connected To Server!" << endl;
	}

	virtual int32 OnRecvPacket(BYTE* buffer, int32 len) override
	{
		BufferReader br(buffer, len);

		PacketHeader header;
		br >> header;

		uint64 id;
		uint32 hp;
		uint16 attack;
		br >> id >> hp >> attack;
		cout << "ID: " << id << "HP: " << hp << "ATK : " << attack << endl;

		char recvBuffer[4096];
		//받은 데이터 크기만큼 빼줌
		br.Read(recvBuffer, header.size - sizeof(PacketHeader) - 8 - 4 - 2);
		cout << recvBuffer << endl;

		return len;
	}

	virtual void OnSend(int32 len) override
	{
		//cout << "OnSend Len = " << len << endl;
	}

	virtual void OnDisconnected() override
	{
		//cout << "Disconnected..." << endl;
	}
};

int main()
{
	this_thread::sleep_for(1s);

	ClientServiceRef service = MakeShared<ClientService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<ServerSession>,
		5);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 2; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}

	GThreadManager->Join();
}