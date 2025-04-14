#pragma once
#include "Creature.h"

#define GOOMBA_VX 0.03125f
#define GOOMBA_VY 0.0f

#define GOOMBA_LIFE 1.0f

#define GOOMBA_BBOX_WIDTH 16.0f
#define GOOMBA_BBOX_HEIGHT 14.0f
#define GOOMBA_BBOX_HEIGHT_DIE 8.0f

#define GOOMBA_DIE_TIMEOUT 240

//animations
//object
#define ANI_ID_GOOMBA 5000
//actions
#define ANI_ID_GOOMBA_WALKING 0
#define ANI_ID_GOOMBA_DIE 1

class CGoomba : public CCreature
{
protected:
	ULONGLONG die_start;

public:
	CGoomba(float x, float y);
	
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);

	virtual void ChangeAnimation();
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	//virtual int IsBlocking() { return 0; }
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);	
	virtual void OnCollisionWithPlatform(LPCOLLISIONEVENT e);

	virtual void Reaction(CGameObject* by_another, int action);
	virtual void Reaction_LivingState(CGameObject* by_another, int action);
	
	virtual void SetState(int state);
	virtual void ToStateLiving();
	virtual void ToStateDying();

	virtual void InPhase(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void InPhaseLiving(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};