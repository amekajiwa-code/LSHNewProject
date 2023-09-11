#pragma once
#include <deque>
#include "Player.h"
#include "NPC.h"
#include "Timer.h"

struct PlayerSnapshot {
	Vector3 position;
	PlayerState state;
	bool flip;
	float RadianAngle;
};

struct NpcSnapshot {
	Vector3 position;
	NpcState state;
};

class Record
{
private:
	deque<PlayerSnapshot> playerSnap;
	deque<NpcSnapshot> npcSnap;
public:
	void RecordPlayer(Player* player);
	bool RewindPlayer(Player* player);
	bool ReplayPlayer(Player* player);
	void RecordNPC(Npc* npc);
	bool RewindNPC(Npc* npc);
	bool ReplayNPC(Npc* npc);
};