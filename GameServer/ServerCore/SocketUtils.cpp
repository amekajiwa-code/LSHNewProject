#include "pch.h"
#include "SocketUtils.h"

// SocketUtils

LPFN_CONNECTEX SocketUtils::ConnectEx = nullptr;
LPFN_DISCONNECTEX SocketUtils::DisconnectEx = nullptr;
LPFN_ACCEPTEX SocketUtils::AcceptEx = nullptr;

void SocketUtils::init()
{
	//TODO
}

void SocketUtils::Clear()
{
	//TODO For ~
	ASSERT_CRASH(true);
}
