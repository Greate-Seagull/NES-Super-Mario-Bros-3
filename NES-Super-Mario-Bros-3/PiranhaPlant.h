#pragma once

#include "Creature.h"

#define PIRANHA_VX 0.0f
#define PIRANHA_VY 0.06f

#define PIRANHA_AX 0.0f
#define PIRANHA_AY 0.0f

#define PIRANHA_SHARP true
#define PIRANHA_LIFE 1.0f

#define PIRANHA_DIG_DISTANCE 30.0f
#define PIRANHA_SHOOT_MAX_DISTANCE 135.0f
#define PIRANHA_SHOOT_MIN_DISTANCE 118.0f

#define PIRANHA_BBOX_HEIGHT 32
#define PIRANHA_BBOX_WIDTH 16

#define PIRANHA_NUMBER_STATE 4
#define PIRANHA_STATE_EMERGE 0
#define PIRANHA_STATE_SHOOT 1
#define PIRANHA_STATE_DIG 2
#define PIRANHA_STATE_RELOAD 3

#define PIRANHA_SHOOT_TIME 3000
#define PIRANHA_RELOAD_TIME 2000

#define ANI_ID_PIRANHA 6000
//Actions
#define ANI_ID_PIRANHA_LOOK 0
#define ANI_ID_PIRANHA_SHOOT 100
//Vertical direction
#define ANI_ID_PIRANHA_DOWN 0	
#define ANI_ID_PIRANHA_UP 10	
//Horizontal direction
//left 0
//right 1

class CPiranhaPlant : public CCreature
{
	float origin_x;
	float origin_y;
	
	DWORD start_behavior_time;

	int aniID;
public:
	CPiranhaPlant(float x, float y);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void DetermineState();
	virtual void SetState(int state);
	virtual void Render();
	virtual void ChangeAnimation();
	virtual void UnderAttack();
};