#pragma once
#include "Enemy.h"
#include "Wings.h"

#define GOOMBA_VX 0.03125f
#define GOOMBA_VY 0.0f

#define GOOMBA_LIFE 1.0f

#define GOOMBA_BBOX_WIDTH 16.0f
#define GOOMBA_BBOX_HEIGHT 14.0f
#define GOOMBA_BBOX_HEIGHT_DIE 8.0f

#define GOOMBA_DIE_TIMEOUT 240

//WINGS COMPONENT ----------------------------
#define WINGS_DISTANCE_BETWEEN 12.0f
#define WINGS_Y_OFFSET -9.0f

#define CHASING_VY -0.1f
#define WINGS_JUMP_VY -0.3f
#define CHASING_DURATION 30 * 16
#define CHASING_MAX_MOMENTUM 4
//--------------------------------------------

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

	//WINGS COMPONENT ----------------------------
	bool bornWithWings;
	CWing* wings;

	//Chasing
	float start_y;
	int momentum;
	DWORD start_momentum;
	//--------------------------------------------
public:
	CGoomba(float x, float y, bool haveWings = false);
	
	virtual void Prepare(DWORD dt);
	virtual void DefaultPrepare(DWORD dt) {};

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void DefaultUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};

	virtual int GetObjectAniID() { return ANI_ID_GOOMBA; }
	virtual void ChangeAnimation();
	virtual void Render();

	virtual int IsCollidable() { return life; };
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

	virtual void Refresh();

	//WINGS COMPONENT ----------------------------
	virtual void GrowWings();
	virtual void LoseWings();
	virtual void Flutter();

	virtual void ChaseMario(DWORD dt);
	virtual void SetMomentum(int m);
	//--------------------------------------------
};