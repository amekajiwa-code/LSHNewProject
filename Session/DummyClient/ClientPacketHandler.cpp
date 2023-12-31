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
	case USER_INFO:
		Handle_USER_INFO(buffer, len);
		break;
	default:
		break;
	}
}

// 패킷 설계 : 임시구조체임

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
	//가변 데이터
	// 1) 문자열 (ex : name)
	// 2) 바이트 배열 (ex : 길드 이미지)
	// 3) 일반 리스트
	vector<int64> buffs; //버프나 디버프들
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
	//버프정보 읽어오기
	buffs.resize(buffCount);
	for (int32 i = 0; i < buffCount; ++i)
	{
		br >> buffs[i].buffId >> buffs[i].remainTime;
	}
	//버프정보출력
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

	uint64 id;
	float posX;
	float posY;
	float posZ;
	
	br >> id >> posX >> posY >> posZ;
	cout << "id : " << id << " / position : ( " << posX << ", " << posY << ", " << posZ << " )" << endl;
}
