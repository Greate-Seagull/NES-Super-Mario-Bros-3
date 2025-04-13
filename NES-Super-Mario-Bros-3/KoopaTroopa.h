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
#define KOOPA_POP_VX 0.0625f

#define KOOPA_HIDE_TIME 7000
#define KOOPA_POP_TIME 2000
#define KOOPA_POP_MAX_FRAME_TIME 500

#define KOOPA_STATE_HIDE 1
#define KOOPA_STATE_ROLL 2
#define KOOPA_STATE_POP 3

#define ANI_ID_KOOPA 5100
#define ANI_ID_KOOPA_WALK 0
#define ANI_ID_KOOPA_HIDE 10
#define ANI_ID_KOOPA_ROLL 20
#define ANI_ID_KOOPA_POP 30
#define ANI_ID_KOOPA_LEFT 0
#define ANI_ID_KOOPA_RIGHT 1

class CKoopaTroopa : public CCreature
{
	int start_time;
	int remaining_time;

	int aniID;
public:
	CKoopaTroopa(float x, float y);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual int IsCollidable() { return 1; };
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithPlatform(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithHarmfulObject(LPCOLLISIONEVENT e);
	void Patrol();
	virtual void Render();
	virtual void ChangeAnimation();
	virtual void SetState(int state);

	virtual void Reaction(CGameObject* by_another, int action);
	virtual void Reaction_LivingState(CGameObject* by_another, int action);
	virtual void Reaction_RollingState(CGameObject* by_another, int action);
	virtual void Reaction_HidingState(CGameObject* by_another, int action);

	virtual void UnderAttack(CGameObject* by_another);
	virtual void Recover();
	virtual void LookForMario();

	virtual const char* ToString();
};
