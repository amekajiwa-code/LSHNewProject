#include "pch.h"
#include "ClientPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"

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
	case USER_INFO:
		Handle_USER_INFO(buffer, len);
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

void ClientPacketHandler::Handle_USER_INFO(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	float posX;
	float posY;
	float posZ;
	
	br >> posX >> posY >> posZ;
	cout << " / position : ( " << posX << ", " << posY << ", " << posZ << " )" << endl;
}

SendBufferRef ClientPacketHandler::Make_USER_INFO(float posX, float posY, float posZ)
{
	SendBufferRef sendBuffer = GSendBufferManager->Open(4096); //4kb
	BufferWriter bw(sendBuffer->Buffer(), sendBuffer->AllocSize());
	PacketHeader* header = bw.Reserve<PacketHeader>();

	// uint64 id uint32 ü�� uint16 ���ݷ�
	bw << posX << posY << posZ;

	header->size = bw.WriteSize();
	header->id = 2; // 2: User Info

	sendBuffer->Close(bw.WriteSize()); //����� ���̸�ŭ �ݾ���

	return sendBuffer;
}
