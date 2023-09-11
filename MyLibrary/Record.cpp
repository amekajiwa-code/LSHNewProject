#include "Record.h"
#include "Input.h"

void Record::RecordPlayer(Player* player)
{
	PlayerSnapshot pss;
	pss.position = player->m_vPos;
	pss.state = player->GetPlayerState();
	pss.flip = player->GetFlip();
	pss.RadianAngle = player->GetRadianAngle();
	playerSnap.push_back(pss);
}

void Record::RecordNPC(Npc* npc)
{
	NpcSnapshot nss;
	nss.position = npc->m_vPos;
	nss.state = npc->GetNPCState();
	npcSnap.push_back(nss);
}

bool Record::RewindPlayer(Player* player)
{
	if (playerSnap.empty())
	{
		Timer::GetInstance().mTimeScale = 1.0f;
		Timer::GetInstance().mGameTimer = 0.0f;
		player->Setinvincible(false);
		return false;
	}
	else
	{
		PlayerSnapshot pss = playerSnap.back();
		playerSnap.pop_back();
		player->m_vPos = pss.position;
		player->SetPlayerState(pss.state);
		player->SetFlip(pss.flip);
		player->SetRadianAngle(pss.RadianAngle);
		player->m_bDead = false;
		player->Setinvincible(true);
		return true;
	}
}

bool Record::RewindNPC(Npc* npc)
{
	if (npcSnap.empty())
	{
		npc->Setinvincible(false);
		return false;
	}
	else
	{
		NpcSnapshot nss = npcSnap.back();
		npcSnap.pop_back();
		npc->m_vPos = nss.position;
		npc->SetNPCState(nss.state);
		npc->m_bDead = false;
		npc->Setinvincible(true);
		return true;
	}
}

bool Record::ReplayPlayer(Player* player)
{
	if (playerSnap.empty())
	{
		Timer::GetInstance().mTimeScale = 0.0f;
		return false;
	}
	else
	{
		PlayerSnapshot pss = playerSnap.front();
		playerSnap.pop_front();
		player->m_vPos = pss.position;
		player->SetPlayerState(pss.state);
		player->SetFlip(pss.flip);
		player->SetRadianAngle(pss.RadianAngle);
		player->Setinvincible(true);
		return true;
	}
}

bool Record::ReplayNPC(Npc* npc)
{
	if (npcSnap.empty())
	{
		return false;
	}
	else
	{
		NpcSnapshot nss = npcSnap.front();
		npcSnap.pop_front();
		npc->m_vPos = nss.position;
		npc->SetNPCState(nss.state);
		npc->Setinvincible(true);
		return true;
	}
}