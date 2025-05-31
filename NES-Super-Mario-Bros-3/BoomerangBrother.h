#pragma once

#include "Enemy.h"
#include "Boomerang.h"

#define BOOMERANG_BROTHER_FREQUENCY BOOMERANG_FREQUENCY
#define BOOMERANG_BROTHER_INITIAL_PHASE 0.0f
#define BOOMERANG_BROTHER_AMPLITUDE M_PI / 75.0f

#define BOOMERANG_BROTHER_RANDOM_VY -0.3f

#define BOOMERANG_BROTHER_MAGSIZE 2

#define BOOMERANG_BROTHER_LIFE 1.0f

#define BOOMERANG_HOLD_OFFSET_X -8.0f
#define BOOMERANG_HOLD_OFFSET_Y -8.0f

#define BOOMERANG_BROTHER_BBOX_WIDTH 16.0f
#define BOOMERANG_BROTHER_BBOX_HEIGHT 23.0f

#define ANI_ID_BOOMERANG_BROTHER 27000
#define ANI_ID_BOOMERANG_BROTHER_TOSH 0
#define ANI_ID_BOOMERANG_BROTHER_SWING 100
#define ANI_ID_BOOMERANG_BROTHER_RELOAD 200

class CBoomerangBrother: public CEnemy
{
	float oscillate_phase;

	int special_action;	

	//
	vector<CBoomerang*> boomerang;
	int magSize;
	bool isFiring;
public:
	CBoomerangBrother(float x, float y);

	virtual void Prepare(DWORD dt);

	virtual void SetSpecialAction(int action);
	virtual void Tosh();
	virtual void Swing();
	virtual void Reload();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void DoAction();
	virtual void Toshing();
	virtual void Swinging();
	virtual void Reloading();

	virtual int GetObjectAniID() { return ANI_ID_BOOMERANG_BROTHER; }
	virtual void ChangeAnimation();
	virtual void Render();

	virtual int IsCollidable() { return life; };

	virtual void OnNoCollisionWithBlocking(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithPlatform(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithBlock(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithBoomerang(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithMario(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithEnemy(LPCOLLISIONEVENT e);

	virtual void OnReactionToAttack1(LPCOLLISIONEVENT e);
	virtual void OnReactionToAttack2(LPCOLLISIONEVENT e);
	virtual void OnReactionToAttack3(LPCOLLISIONEVENT e);

	virtual void Refresh();

	virtual void TakeBoomerangs();
	virtual vector<CBoomerang*> GetBoomerangs() { return boomerang; }
};

