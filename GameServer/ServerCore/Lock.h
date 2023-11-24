#pragma once
#include "Types.h"

#pragma region RW SpinLock

//32��Ʈ Ȱ��
// ���� 16��Ʈ WriteFlag ���� ȹ���ϰ� �ִ� ������ ID����
// ���� 16��Ʈ ReadȽ�� ,ReadFlasg �����ϰ� �ִ� Read Count ����

class Lock
{
	enum : uint32
	{
		ACQUIRE_TIMEOUT_TICK = 10000,
		MAX_SPIN_COUNT = 5000,
		WRITE_THREAD_MASK = 0xFFFF'0000, // ���� 16��Ʈ ǥ��
 		READ_COUNT_MASK = 0x0000'FFFF, // ���� 16��Ʈ ǥ��
		EMPTY_FLAG = 0x0000'0000 // ��� ��Ʈ 0 ����� �÷���
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
////RAII �������� Lock�� ���ڷ� �ް� ����� _lock�� ���ڸ� ������ ReadLock ����
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