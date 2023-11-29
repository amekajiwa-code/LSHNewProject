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
////RAII �������� Lock�� ���ڷ� �ް� ����� _lock�� ���ڸ� ������ ReadLock ����
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