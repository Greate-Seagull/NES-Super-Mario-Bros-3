#pragma once

#include "Enemy.h"

#define KOOPA_LIFE 2.0f

#define KOOPA_BBOX_HEIGHT_FOOT_1	26.0f
#define KOOPA_BBOX_HEIGHT_FOOT_2	27.0f
#define KOOPA_BBOX_HEIGHT_HIDE		16.0f
#define KOOPA_BBOX_WIDTH_LIVE			8.0f//15.0f
#define KOOPA_BBOX_WIDTH_HIDE			16.0f//15.0f

#define KOOPA_VX 0.03125f
#define KOOPA_VY 0.0f
#define KOOPA_ROLL_VX 0.225f
#define KOOPA_POP_VX 0.0625f
#define KOOPA_BOUNCE_VY 0.08f
#define KOOPA_FALL

#define KOOPA_HIDE_TIME 7000
#define KOOPA_POP_TIME 2000
#define KOOPA_POP_MAX_FRAME_TIME 500

#define KOOPA_STATE_HIDE 1
#define KOOPA_STATE_ROLL 2
#define KOOPA_STATE_POP 3

#define ANI_ID_KOOPA 24000
#define ANI_ID_KOOPA_WALK 0
#define ANI_ID_KOOPA_HIDE 10
#define ANI_ID_KOOPA_ROLL 30
#define ANI_ID_KOOPA_POP 50

#define ANI_ID_KOOPA_LEFT 0
#define ANI_ID_KOOPA_RIGHT 1
#define ANI_ID_KOOPA_DOWN 0
#define ANI_ID_KOOPA_UP 10

class CKoopaTroopa : public CEnemy
{
	//recovering
	int recovering_time;

	//
	float on_ground_y;
	float on_ground_x;
public:
	CKoopaTroopa(float x, float y);

	virtual void Prepare(DWORD dt);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Living(DWORD dt);
	virtual void Hiding(DWORD dt);
	virtual void Rolling(DWORD dt);
	virtual void Poping(DWORD dt);

	//virtual int IsBlocking() { return state == KOOPA_STATE_HIDE || state == KOOPA_STATE_POP; }
	virtual int IsCollidable() { return state != STATE_DIE; };	

	virtual void OnNoCollisionWithBlocking(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithPlatform(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithBlock(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithMario(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithEnemy(LPCOLLISIONEVENT e);

	virtual void OnReactionToCarrying(LPCOLLISIONEVENT e);
	virtual void OnReactionToTouching(LPCOLLISIONEVENT e);
	virtual void OnReactionToAttack1(LPCOLLISIONEVENT e);
	virtual void OnReactionToAttack2(LPCOLLISIONEVENT e);
	virtual void OnReactionToAttack3(LPCOLLISIONEVENT e);

	virtual void Render();
	virtual void ChangeAnimation();

	virtual void SetState(int state);

	virtual void UnderAttack(CGameObject* by_another);

	virtual void Bounce();
	virtual void Hide(DWORD dt);
	virtual void Pop(DWORD dt);

	void Patrol();

	virtual void LookForMario();
};
