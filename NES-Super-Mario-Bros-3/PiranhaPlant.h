#pragma once

#include "Creature.h"
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

#define ANI_ID_PIRANHA 24000
//Actions
#define ANI_ID_PIRANHA_BITE 0

class CPiranhaPlant : public CCreature
{
protected:
	//Behavior sensor
	float start_y;		
	DWORD start_action_time;

	//attack sensor
	float target_dx, target_dy;
public:
	CPiranhaPlant(float x, float y);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	virtual int IsCollidable() { return 1; };

	virtual void Render();
	virtual void ChangeAnimation();

	virtual void InPhase(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void InPhaseEmerge();
	virtual void InPhaseAttack(DWORD dt);
	virtual void InPhaseDig();
	virtual void InPhaseHide(DWORD dt);

	virtual void SetState(int state);
	virtual void ToStateEmerge();
	virtual void ToStateAttack();
	virtual void ToStateDig();
	virtual void ToStateHide();

	virtual void LookforMario();

	virtual void Reaction(CGameObject* by_another, int action);
};