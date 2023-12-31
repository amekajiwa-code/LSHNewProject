#pragma once
#include "Types.h"

#pragma region RW SpinLock

//32비트 활용
// 상위 16비트 WriteFlag 현재 획득하고 있는 스레드 ID담음
// 하위 16비트 Read횟수 ,ReadFlasg 공유하고 있는 Read Count 담음

class Lock
{
	enum : uint32
	{
		ACQUIRE_TIMEOUT_TICK = 10000,
		MAX_SPIN_COUNT = 5000,
		WRITE_THREAD_MASK = 0xFFFF'0000, // 상위 16비트 표현
 		READ_COUNT_MASK = 0x0000'FFFF, // 하위 16비트 표현
		EMPTY_FLAG = 0x0000'0000 // 모든 비트 0 빈상태 플래그
	};
public:
	void WriteLock(const char* name);
	void WriteUnLock(const char* name);
	void ReadLock(const char* name);
	void ReadUnLock(const char* name);
private:
	Atomic<uint32> _lockFlag = EMPTY_FLAG;
	uint16 _writeCount = 0;
};
#pragma endregion

#pragma region LockGuard
////RAII 패턴으로 Lock을 인자로 받고 멤버의 _lock에 인자를 넣은뒤 ReadLock 실행
class ReadLockGuard
{
public:
	ReadLockGuard(Lock& lock, const char* name) : _lock(lock), _name(name) { _lock.ReadLock(name); }
	~ReadLockGuard() { _lock.ReadUnLock(_name); }
private:
	Lock& _lock;
	const char* _name;
};

class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock, const char* name) : _lock(lock), _name(name) { _lock.WriteLock(name); }
	~WriteLockGuard() { _lock.WriteUnLock(_name); }
private:
	Lock& _lock;
	const char* _name;
};
#pragma endregion