#pragma once

#include "HarmfulObject.h"

#define FIREBALL_SHORT_VX 0.04375f
#define FIREBALL_LONG_VX 0.0625f
#define FIREBALL_SHORT_VY 0.04375f
#define FIREBALL_LONG_VY 0.02f

#define FIREBALL_RANGE_SHORT 1
#define FIREBALL_RANGE_LONG 2

#define ANI_ID_FIREBALL 100

class CFireball : public CHarmfulObject
{
public:
	CFireball(float x, float y, int shoot_range = FIREBALL_RANGE_SHORT, int nx = 1, int ny = 1);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void ApplyRange(int range);
	virtual void ApplyDirection(int nx, int ny);
	virtual void MeleeAttack(CHarmfulObject* another);
};