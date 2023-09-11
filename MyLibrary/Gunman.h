#pragma once
#include "NPC.h"

class Gunman : public Npc
{
public:
	virtual bool Frame() override;

	virtual bool DetectPlayer() override;
	virtual void Attack() override;
};

