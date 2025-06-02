#pragma once

#include "Enemy.h"
#include "Fireball.h"

#define PIRANHA_VX 0.0f
#define PIRANHA_VY 0.06f

#define PIRANHA_AX 0.0f
#define PIRANHA_AY 0.0f

#define PIRANHA_LIFE 1.0f

#define PIRANHA_BBOX_HEIGHT 32.0f
#define PIRANHA_BBOX_WIDTH 16.0f

#define PIRANHA_DISTANCE_HIDE 30.0f

#define PIRANHA_NUMBER_STATE 4
#define PIRANHA_STATE_EMERGE 0
#define PIRANHA_STATE_ATTACK 1
#define PIRANHA_STATE_DIG 2
#define PIRANHA_STATE_HIDE 3

#define PIRANHA_ATTACK_TIME 1500
#define PIRANHA_HIDE_TIME 1500
#define PIRANHA_DIE_TIME 350

#define ANI_ID_PIRANHA 24000
//Actions
#define ANI_ID_PIRANHA_BITE 0
#define ANI_ID_PIRANHA_DIE 999

class CPiranhaPlant : public CEnemy
{
protected:
	//pot
	CGameObject* pot;

	//emerge and dig
	float start_y;	
	DWORD start_action_time;

	//attack
	float target_dx, target_dy;

	//die
	DWORD die_time;
public:
	CPiranhaPlant(float x, float y);

	virtual void SetPosition(float x, float y);

	virtual void Prepare(DWORD dt);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	virtual int IsCollidable() { return state != PIRANHA_STATE_HIDE && state != STATE_DIE; };
	virtual int IsLinkedTo(CGameObject* obj) { return pot == obj || dynamic_cast<CFireball*>(obj); }
	virtual int IsMoving() { return 1; }

	virtual void Render();
	virtual void ChangeAnimation();

	virtual void Emerging(DWORD dt);
	virtual void Attacking(DWORD dt);
	virtual void Digging(DWORD dt);
	virtual void Hiding(DWORD dt);
	virtual void Dying(DWORD dt);

	virtual void SetState(int state);
	virtual void ToStateEmerge();
	virtual void ToStateAttack();
	virtual void ToStateDig();
	virtual void ToStateHide();
	virtual void ToStateDie();

	virtual void OnReactionToCarrying(LPCOLLISIONEVENT e);
	virtual void OnReactionToTouching(LPCOLLISIONEVENT e);
	virtual void OnReactionToAttack1(LPCOLLISIONEVENT e);
	virtual void OnReactionToAttack2(LPCOLLISIONEVENT e);
	virtual void OnReactionToAttack3(LPCOLLISIONEVENT e);

	virtual void LookForMario();

	virtual void Refresh();
};