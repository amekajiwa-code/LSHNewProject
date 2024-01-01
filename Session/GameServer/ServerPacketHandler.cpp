#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"
#include "GameSession.h"
#include "GameSessionManager.h"

void ServerPacketHandler::HandlePacket(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br.Peek(&header);

	switch (header.id)
	{
	case USER_INFO:
		Handle_USER_INFO(buffer, len);
		break;
	default:
		break;
	}
}

void ServerPacketHandler::Handle_USER_INFO(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	float posX;
	float posY;
	float posZ;

	br >> posX >> posY >> posZ;
	cout << "position : (" << posX << ", " << posY << ", " << posZ << ")" << endl;

	SendBufferRef sendBuffer = ServerPacketHandler::Make_USER_INFO(posX, posY, posZ);
	
	for (GameSessionRef player : GSessionManager.GetSessionsRef())
	{
		GSessionManager.Broadcast(sendBuffer);
	}
}

SendBufferRef ServerPacketHandler::Make_S_TEST(uint64 id, uint32 hp, uint16 attack, vector<BuffData> buffs)
{
	SendBufferRef sendBuffer = GSendBufferManager->Open(4096); //4kb

	BufferWriter bw(sendBuffer->Buffer(), sendBuffer->AllocSize());

	PacketHeader* header = bw.Reserve<PacketHeader>();

	// uint64 id uint32 ü�� uint16 ���ݷ�
	bw << id << hp << attack;

	// ���� ������
	bw << (uint16)buffs.size();

	for (BuffData& buff : buffs)
	{
		bw << buff.buffId << buff.remainTime;
	}

	header->size = bw.WriteSize();
	header->id = 1; // 1 : Test Msg

	sendBuffer->Close(bw.WriteSize()); //����� ���̸�ŭ �ݾ���

	return sendBuffer;
}

SendBufferRef ServerPacketHandler::Make_USER_INFO(float posX, float posY, float posZ)
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
