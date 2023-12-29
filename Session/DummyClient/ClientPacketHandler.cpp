#include "pch.h"
#include "ClientPacketHandler.h"
#include "BufferReader.h"

void ClientPacketHandler::HandlePacket(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	switch (header.id)
	{
	case S_TEST:
		Handle_S_TEST(buffer, len);
		break;
	default:
		break;
	}
}

// ��Ŷ ���� : �ӽñ���ü��

struct BuffData
{
	uint64 buffId;
	float remainTime;
};

struct S_TEST
{
	uint64 id;
	uint32 hp;
	uint16 attack;
	//���� ������
	// 1) ���ڿ� (ex : name)
	// 2) ����Ʈ �迭 (ex : ��� �̹���)
	// 3) �Ϲ� ����Ʈ
	vector<int64> buffs; //������ �������
};

struct USER_INFO
{
	uint64 id;
	//wstring name;
	//uint32 hp;
	//uint16 atk;
	//uint16 move_speed;
	//uint16 attack_speed;
	//uint16 attack_range;
	
	//_41 _42 _43�� ������
	float positionX; 
	float positionY;
	float positionZ;
};

void ClientPacketHandler::Handle_S_TEST(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	uint64 id;
	uint32 hp;
	uint16 attack;
	br >> id >> hp >> attack;
	cout << "ID: " << id << "HP: " << hp << "ATK : " << attack << endl;

	vector<BuffData> buffs;
	uint16 buffCount;
	br >> buffCount;
	//�������� �о����
	buffs.resize(buffCount);
	for (int32 i = 0; i < buffCount; ++i)
	{
		br >> buffs[i].buffId >> buffs[i].remainTime;
	}
	//�����������
	cout << "BufCount : " << buffCount << endl;
	for (int32 i = 0; i < buffCount; ++i)
	{
		cout << "BuffInfo : " << buffs[i].buffId << buffs[i].remainTime << endl;
	}
}
