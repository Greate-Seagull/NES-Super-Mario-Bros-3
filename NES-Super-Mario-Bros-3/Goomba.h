#pragma once
#include "Enemy.h"

#define GOOMBA_VX 0.03125f
#define GOOMBA_VY 0.0f

#define GOOMBA_LIFE 1.0f

#define GOOMBA_BBOX_WIDTH 16.0f
#define GOOMBA_BBOX_HEIGHT 14.0f
#define GOOMBA_BBOX_HEIGHT_DIE 8.0f

#define GOOMBA_DIE_TIMEOUT 240

//animations
//object
#define ANI_ID_GOOMBA 20000
//actions
#define ANI_ID_GOOMBA_WALK 0
#define ANI_ID_GOOMBA_DIE 100

class CGoomba : public CEnemy
{
protected:
	int die_start;

public:
	CGoomba(float x, float y);
	
	virtual void Prepare(DWORD dt);
	virtual void DefaultPrepare(DWORD dt) {};

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void DefaultUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};

	virtual void ChangeAnimation();
	virtual void Render();

	virtual int IsCollidable() { return state == STATE_LIVE; };
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithPlatform(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithBlock(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithMario(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithEnemy(LPCOLLISIONEVENT e);

	virtual void OnReactionToTouching(LPCOLLISIONEVENT e);
	virtual void OnReactionToAttack1(LPCOLLISIONEVENT e);
	virtual void OnReactionToAttack2(LPCOLLISIONEVENT e);
	virtual void OnReactionToAttack3(LPCOLLISIONEVENT e);
	
	virtual void SetState(int state);
	virtual void ToStateLiving();
	virtual void ToStateDying();
	virtual void ToDefaultState() {};

	virtual void Living(DWORD dt);
	virtual void Dying(DWORD dt);
};