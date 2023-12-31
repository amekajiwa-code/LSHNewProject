#pragma once

class GameSession;

using GameSessionRef = std::shared_ptr<GameSession>;

class GameSessionManager
{
public:
	void Add(GameSessionRef session);
	void Remove(GameSessionRef session);
	void Broadcast(SendBufferRef sendBuffer);

	Set<GameSessionRef> GetSessionsRef() { return _sessions; }
private:
	USE_LOCK;
	Set<GameSessionRef> _sessions;
private:
	uint64 sessionIdCount = 0;
};

extern GameSessionManager GSessionManager;
