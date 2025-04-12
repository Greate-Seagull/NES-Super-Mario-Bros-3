#pragma once

#include "Creature.h"
#include "Fireball.h"

#define PIRANHA_VX 0.0f
#define PIRANHA_VY 0.06f

#define PIRANHA_AX 0.0f
#define PIRANHA_AY 0.0f

#define PIRANHA_MAG_SIZE 1
#define PIRANHA_LIFE 1.0f

#define PIRANHA_DISTANCE_HIDE 30.0f
#define PIRANHA_LOOK_MAX_DISTANCE 145.0f
#define PIRANHA_SHOOT_MIN_DISTANCE 115.0f

#define PIRANHA_BBOX_HEIGHT 32
#define PIRANHA_BBOX_WIDTH 16

#define PIRANHA_NUMBER_STATE 4
#define PIRANHA_STATE_EMERGE 0
#define PIRANHA_STATE_SHOOT 1
#define PIRANHA_STATE_DIG 2
#define PIRANHA_STATE_RELOAD 3

#define PIRANHA_SHOOT_TIME 1500
#define PIRANHA_RELOAD_TIME 1500

#define ANI_ID_PIRANHA 6000
//Actions
#define ANI_ID_PIRANHA_LOOK 0
#define ANI_ID_PIRANHA_SHOOT 100
//Vertical direction
#define ANI_ID_PIRANHA_DOWN 0	
#define ANI_ID_PIRANHA_UP 10	
//Horizontal direction
#define ANI_ID_PIRANHA_LEFT 0	
#define ANI_ID_PIRANHA_RIGHT 1

class CPiranhaPlant : public CCreature
{
	float start_y;		
	DWORD start_action_time;

	bool assault_mode;
	int mag_size;
	CFireball* boomerang;

	int aniID;
public:
	CPiranhaPlant(float x, float y);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void ChangeState();
	virtual void SetState(int state);
	virtual void Render();
	virtual void ChangeAnimation();
	virtual void UnderAttack(CHarmfulObject* by_another);
	virtual void LookforMario();
	virtual void Reaction(CHarmfulObject* by_another, int action = ACTION_TOUCH);
	virtual void Shoot();
	virtual void Reload();
	//Collision
	void OnCollisionWith(LPCOLLISIONEVENT e);
};