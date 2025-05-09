#pragma once

#include "Creature.h"
#include "Block.h";

#define RACOON_TAIL_BBOX_WIDTH 9.0f
#define RACOON_TAIL_BBOX_HEIGHT 5.0f

class CRacoonTail : public CHarmfulObject
{
public:
	CRacoonTail(float x, float y);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();

	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithCreature(LPCOLLISIONEVENT e);
	void OnCollisionWithBlock(LPCOLLISIONEVENT e);
};