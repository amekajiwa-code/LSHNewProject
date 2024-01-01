#pragma once

// 임시 enum
enum
{
	S_TEST = 1,
	USER_INFO = 2
};

struct USER_INFO
{
	uint64 id;
	//uint32 hp;
	//uint16 atk;
	//wstring name;
	//uint16 move_speed;
	//uint16 attack_speed;
	//uint16 attack_range;

	//_41 _42 _43이 포지션
	float posX;
	float posY;
	float posZ;
};

class ClientPacketHandler
{
public:
	static void HandlePacket(BYTE* buffer, int32 len);
	static void Handle_S_TEST(BYTE* buffer, int32 len); //임시함수
	static void Handle_USER_INFO(BYTE* buffer, int32 len);

	static SendBufferRef Make_USER_INFO(float posX, float posY, float posZ);
};

