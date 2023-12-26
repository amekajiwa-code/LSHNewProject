#include "pch.h"

#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"

#include <string>

char sendData[] = "Hello World";

class ServerSession : public Session
{
public:
	~ServerSession()
	{
		cout << "~ServerSession" << endl;
	}

	//������ �ڵ忡�� ������ �ϰ�����
	/*���ʷ� ������ �������� OnConnected���� Send�� �ϰ�
	�������� OnRecv�� �ѵ� ���ڷ�*/
	virtual void OnConnected() override
	{
		cout << "Connected To Server!" << endl;

		SendBufferRef sendBuffer = MakeShared<SendBuffer>(4096); //4kb
		sendBuffer->CopyData(sendData, sizeof(sendData));
		Send(sendBuffer);
	}

	virtual void OnDisconnected() override
	{
		cout << "Disconnected..." << endl;
	}

	virtual int32 OnRecv(BYTE* buffer, int32 len) override
	{
		//Echo
		cout << "OnRecv Len = " << len << endl;
		cout << buffer << endl;

		this_thread::sleep_for(1s);

		SendBufferRef sendBuffer = MakeShared<SendBuffer>(4096); //4kb
		sendBuffer->CopyData(sendData, sizeof(sendData));
		Send(sendBuffer);

		return len;
	}

	virtual void OnSend(int32 len) override
	{
		cout << "OnSend Len = " << len << endl;
	}
};

void InputData()
{
	cout << "�Է� : ";
	cin >> sendData;
}

int main()
{
	this_thread::sleep_for(1s);

	ClientServiceRef service = MakeShared<ClientService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<ServerSession>,
		1);

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