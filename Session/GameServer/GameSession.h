#pragma once
#include "Session.h"

struct float3
{
	float _posX = 0;
	float _posY = 0;
	float _posZ = 0;
};

class GameSession : public PacketSession
{
public:
	~GameSession()
	{
		cout << "~GameSession" << endl;
	}

	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;

	uint64 GetSessionId() { return _sessionId; }
	void SetSessionId(uint64 sessionId) { _sessionId = sessionId; }
	float3 GetPlayerPos() { return _playerPos; }
	void SetPlayerPos(float posX, float posY, float posZ) { _playerPos._posX = posX; _playerPos._posY = posY; _playerPos._posZ = posZ; }

private:
	uint64 _sessionId = 0;
	float3 _playerPos { 0, 0, 0 };

};

