#include "pch.h"
#include "Session.h"
#include "SocketUtils.h"
#include "Service.h"

/*--------------
	Session
---------------*/

Session::Session()
{
	_socket = SocketUtils::CreateSocket();
}

Session::~Session()
{
	SocketUtils::Close(_socket);
}

void Session::Disconnect(const WCHAR* cause)
{
	if (_connected.exchange(false) == false)
	{
		return;
	}

	wcout << "Disconnect : " << cause << endl;

	OnDisconnected();
	SocketUtils::Close(_socket);
	GetService()->ReleaseSession(GetSessionRef());
}

HANDLE Session::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Session::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	//이벤트의 사유에 따라 함수를 호출
	switch (iocpEvent->eventType)
	{
	case EventType::Connect:
		ProcessConnect();
		break;
	case EventType::Recv:
		ProcessRecv(numOfBytes);
		break;
	case EventType::Send:
		ProcessSend(numOfBytes);
		break;
	default:
		break;
	}
}

void Session::RegisterConnect()
{
}

void Session::RegisterRecv()
{
	if (IsConnected() == false)
	{
		//연결이 끊기면
		return;
	}

	_recvEvent.Init();
	_recvEvent.owner = shared_from_this(); //레퍼런스 카운트 1 늘림

	WSABUF wsaBuf;
	wsaBuf.buf = reinterpret_cast<char*>(_recvBuffer);
	wsaBuf.len = len32(_recvBuffer);

	DWORD numOfBytes = 0;
	DWORD flags = 0;
	if (SOCKET_ERROR == ::WSARecv(_socket, &wsaBuf, 1, OUT &numOfBytes,
		OUT &flags, &_recvEvent, nullptr))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			//진문상 : HandleError 보내준 뒤 _recvEvent.owner 초기화
			HandleError(errorCode);
			_recvEvent.owner = nullptr;
		}
	}
}

void Session::RegisterSend()
{
}

void Session::ProcessConnect()
{
	_connected.store(true); //연결됨

	//세션 등록
	GetService()->AddSession(GetSessionRef());

	//컨텐츠 코드에서 오버로딩
	OnConnected();

	//수신 등록
	RegisterRecv();
}

void Session::ProcessRecv(int32 numOfBytes)
{
	_recvEvent.owner = nullptr;

	//연결 끊김
	if (numOfBytes == 0)
	{
		Disconnect(L"Recv 0");
		return;
	}

	// TODO
	cout << "Recv Data Len" << numOfBytes << endl;

	// 수신 등록
	RegisterRecv();
}

void Session::ProcessSend(int32 numOfBytes)
{
}

void Session::HandleError(int32 errorCode)
{
	switch (errorCode)
	{
	case WSAECONNRESET:
		break;
	case WSAECONNABORTED:
		Disconnect(L"HandleError");
		break;
	default:
		//TODO : Log
		cout << "Handle Error : " << errorCode << endl;
		break;

	}
}
