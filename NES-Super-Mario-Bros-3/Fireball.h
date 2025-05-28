#pragma once

#include "HarmfulObject.h"
#include "debug.h"

#define FIREBALL_BBOX_HEIGHT 8.0f
#define FIREBALL_BBOX_WIDTH 8.0f

#define FIREBALL_SHORT_VX 0.04375f
#define FIREBALL_LONG_VX 0.0625f
#define FIREBALL_SHORT_VY 0.04375f
#define FIREBALL_LONG_VY 0.02f

#define FIREBALL_RANGE_DISARM 0
#define FIREBALL_RANGE_SHORT 1
#define FIREBALL_RANGE_LONG 2

#define ANI_ID_FIREBALL 26000

class CFireball : public CHarmfulObject
{
public:
	CFireball(float x, float y, int shoot_range = FIREBALL_RANGE_DISARM, int nx = 1, int ny = 1);

	void Prepare(DWORD dt) { };
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();

	virtual void ApplyRange(int range);
	virtual void ApplyDirection(int nx, int ny);

	//void OnCollisionWith(LPCOLLISIONEVENT e);
	//virtual void OnCollisionWithMario(LPCOLLISIONEVENT e);
	void OnReactionTo(LPCOLLISIONEVENT e, int action);

	int IsCollidable();
};