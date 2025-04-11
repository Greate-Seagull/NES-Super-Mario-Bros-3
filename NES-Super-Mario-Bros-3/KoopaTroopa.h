#pragma once

#include "Creature.h"

#define KOOPA_LIFE 2.0f

#define KOOPA_BBOX_HEIGHT_FOOT_1	26
#define KOOPA_BBOX_HEIGHT_FOOT_2	27
#define KOOPA_BBOX_HEIGHT_HIDE		16
#define KOOPA_BBOX_WIDTH 15

#define KOOPA_VX 0.03125f
#define KOOPA_VY 0.0f
#define KOOPA_ROLL_VX 0.225f

#define KOOPA_STATE_HIDE 1
#define KOOPA_STATE_ROLL 2

#define ANI_ID_KOOPA 5100
#define ANI_ID_KOOPA_WALK 0
#define ANI_ID_KOOPA_HIDE 10
#define ANI_ID_KOOPA_ROLL 20
#define ANI_ID_KOOPA_LEFT 0
#define ANI_ID_KOOPA_RIGHT 1

class CKoopaTroopa : public CCreature
{
	int aniID;
public:
	CKoopaTroopa(float x, float y);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual int IsCollidable() { return 1; };
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithPlatform(LPCOLLISIONEVENT e);
	void Patrol();
	virtual void Render();
	virtual void ChangeAnimation();
	virtual void SetState(int state);
	virtual void UnderAttack(CHarmfulObject* by_another);
};
