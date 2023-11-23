#pragma once
#include "Types.h"

#pragma region RW SpinLock

//32비트 활용
// 상위 16비트 WriteFlag 현재 획득하고 있는 스레드 ID담음
// 하위 16비트 ReadFlasg 공유하고 있는 Read Count 담음

class Lock
{
	enum : uint32
	{
		ACQUIRE_TIMEOUT_TICK = 10000,
		MAX_SPIN_COUNT = 5000,
		WRITE_THREAD_MASK = 0xFFFF'0000,
		READ_COUNT_MASK = 0x0000'FFFF,
		EMPTY_FLAG = 0x0000'0000
	};
public:
	void WriteLock();
	void WriteUnLock();
	void ReadLock();
	void ReadUnLock();
private:
	Atomic<uint32> _lockFlag = EMPTY_FLAG;
	uint16 _writeCount = 0;
};
#pragma endregion

#pragma region LockGuard
//RAII
class ReadLockGuard
{
public:
	ReadLockGuard(Lock& lock) : _lock(lock) { _lock.ReadLock(); }
	~ReadLockGuard() { _lock.ReadUnLock(); }
private:
	Lock& _lock;
};

class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock) : _lock(lock) { _lock.WriteLock(); }
	~WriteLockGuard() { _lock.WriteUnLock(); }
private:
	Lock& _lock;
};
#pragma endregion