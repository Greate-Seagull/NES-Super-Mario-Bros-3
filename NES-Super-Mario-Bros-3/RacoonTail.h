#pragma once

#include "Creature.h"
#include "Block.h"

#define RACOON_TAIL_BBOX_WIDTH 9.0f
#define RACOON_TAIL_BBOX_HEIGHT 5.0f

class CMario;

class CRacoonTail : public CHarmfulObject
{
	float dx, dy;
	CGameObject* owner;
public:
	CRacoonTail(float x, float y);

	void SetMoveDistance(float dx, float dy) { this->dx = dx; this->dy = dy; }

	void Prepare(DWORD dt);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();

	int IsLinkedTo(CGameObject* obj);
	int IsSoft() { return 1; }

	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithCreature(LPCOLLISIONEVENT e);
	void OnCollisionWithBlock(LPCOLLISIONEVENT e);

	void AcceptOwner(CGameObject* owner);

	void Refresh();
};