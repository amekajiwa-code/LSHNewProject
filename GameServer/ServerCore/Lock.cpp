#include "pch.h"
#include "Lock.h"
#include "DeadLockProfiler.h"

void Lock::WriteLock(const char* name)
{
#if _DEBUG
	GDeadLockProfiler->PushLock(name);
#endif
	// ������ �����尡 �����ϰ� �ִٸ� ������ ����.
	// ������ ����16��Ʈ�� Write�� ����ϰ� �ִ°�, ������ ����16��Ʈ Read ����ϴ°�
	// ��Ʈ����Ʈ�� ���� 16��Ʈ ä��� ���� 16��Ʈ ���
	const uint32 lockThreadId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId)
	{
		_writeCount++;
		return;
	}

	// �ƹ��� ����,�����ϰ� ���� ������ �����ؼ� ������ ����

	const int64 beginTick = ::GetTickCount64(); // �ð� ����
	//���� 16��Ʈ�� �������� ��Ʈ����Ʈ �ϰ� ��Ʈ����ũ�� ������ ���� 16��Ʈ ���� ���� 16��Ʈ LThreadId �Է�
	const uint32 desired = ((LThreadId << 16) & WRITE_THREAD_MASK);
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; ++spinCount)
		{
			uint32 expected = EMPTY_FLAG;
			//_lockFlag�� EMPTY_FALG�� �ɶ����� ��ٷȴٰ� ������� desired�� ���� ��ȯ�� _writeCount++
			if (_lockFlag.compare_exchange_strong(OUT expected, desired))
			{
				_writeCount++;
				return;
			}
		}
		//�ð��ʰ��� ũ����
		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
		{
			CRASH("LOCK_TIMEOUT");
		}

		this_thread::yield();
	}
}

void Lock::WriteUnLock(const char* name)
{
#if _DEBUG
	GDeadLockProfiler->PopLock(name);
#endif
	// ReadLock �� Ǯ�� ���� WriteUnlock �Ұ���
	if ((_lockFlag.load() & READ_COUNT_MASK) != 0)
	{
		CRASH("INVALID_UNLOCK_ORDER");
	}

	const int32 lockCount = --_writeCount;
	if (lockCount == 0)
	{
		_lockFlag.store(EMPTY_FLAG);
	}
}

void Lock::ReadLock(const char* name)
{
#if _DEBUG
	GDeadLockProfiler->PushLock(name);
#endif
	// ������ �����尡 ���� ���� �����ϰ� �ִٸ� ������ ����
	const uint32 lockThreadId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId)
	{
		_lockFlag.fetch_add(1);
		return;
	}

	// �ƹ��� �����ϰ� ���� ������ �����ؼ� ���� ī��Ʈ �ø�

	const int64 beginTick = ::GetTickCount64();

	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; ++spinCount)
		{
			//���� 16��Ʈ�� READ_COUNT ��Ʈ����ũ
			uint32 expected = (_lockFlag.load() & READ_COUNT_MASK);
			if (_lockFlag.compare_exchange_strong(OUT expected, expected + 1))
			{
				return;
			}
		}

		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
		{
			CRASH("LOCK_TIMEOUT");
		}

		this_thread::yield();
	}
}

void Lock::ReadUnLock(const char* name)
{
#if _DEBUG
	GDeadLockProfiler->PopLock(name);
#endif
	if ((_lockFlag.fetch_sub(1) & READ_COUNT_MASK) == 0)
	{
		CRASH("MULTIPLE_UNLOCK");
	}
}
