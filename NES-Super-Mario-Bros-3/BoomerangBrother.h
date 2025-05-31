#pragma once

#include "Enemy.h"

#define BOOMERANG_BROTHER_LIFE 1.0f

#define BOOMERANG_BROTHER_BBOX_WIDTH 16.0f
#define BOOMERANG_BROTHER_BBOX_HEIGHT 23.0f

#define ANI_ID_BOOMERANG_BROTHER 27000
#define ANI_ID_BOOMERANG_BROTHER_TOSH 0
#define ANI_ID_BOOMERANG_BROTHER_SWING 100

class CBoomerangBrother: public CEnemy
{
	float oscillate_phase;

	int special_action;	
public:
	CBoomerangBrother(float x, float y);

	virtual void Prepare(DWORD dt);

	virtual void SetSpecialAction(int action);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void DoAction();
	virtual void Toshing();
	virtual void Swinging();

	virtual int GetObjectAniID() { return ANI_ID_BOOMERANG_BROTHER; }
	virtual void ChangeAnimation();
	virtual void Render();

	virtual int IsCollidable() { return life; };

	virtual void OnNoCollisionWithBlocking(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithPlatform(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithBlock(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithMario(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithEnemy(LPCOLLISIONEVENT e);

	virtual void OnReactionToAttack1(LPCOLLISIONEVENT e);
	virtual void OnReactionToAttack2(LPCOLLISIONEVENT e);
	virtual void OnReactionToAttack3(LPCOLLISIONEVENT e);

	virtual void Refresh();
};

