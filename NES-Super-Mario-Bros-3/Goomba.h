#pragma once
#include "Creature.h"

#define GOOMBA_VX 0.03125f
#define GOOMBA_VY 0.0f

#define GOOMBA_AX 0.0f
#define GOOMBA_AY 0.002f

#define GOOMBA_SHARP false;
#define GOOMBA_LIFE 1.0f

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 14
#define GOOMBA_BBOX_HEIGHT_DIE 8

#define GOOMBA_DIE_TIMEOUT 240

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200


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

	void Reaction(CGameObject* by_another, int action) {};
	
	virtual void SetState(int state);
	virtual void ProcessState();
};