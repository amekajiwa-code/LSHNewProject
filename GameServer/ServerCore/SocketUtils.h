#pragma once
#include "NetAddress.h"

// SocketUtils

class SocketUtils
{
public:
	static LPFN_CONNECTEX ConnectEx;
	static LPFN_DISCONNECTEX DisconnectEx; 
	static LPFN_ACCEPTEX AcceptEx;
public:
	static void init();
	static void Clear();
};