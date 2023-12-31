#pragma once

// юс╫ц enum
enum
{
	S_TEST = 1,
	USER_INFO = 2
};

struct BuffData
{
	uint64 buffId;
	float remainTime;
};

class ServerPacketHandler
{
public:
	static void HandlePacket(BYTE* buffer, int32 len);

	static SendBufferRef Make_S_TEST(uint64 id, uint32 hp, uint16 attack, vector<BuffData> buffs);
	static SendBufferRef Make_USER_INFO(uint64 id, float posX, float posY, float posZ);
};
