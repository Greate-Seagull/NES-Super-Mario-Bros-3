#pragma once

#include "Enemy.h"
#include "Wings.h"

#define KOOPA_LIFE 2.0f

#define KOOPA_BBOX_HEIGHT_LIVE			26.0f
#define KOOPA_BBOX_HEIGHT_HIDE			12.0f
#define KOOPA_BBOX_WIDTH_LIVE			8.0f//15.0f
#define KOOPA_BBOX_WIDTH_HIDE			14.0f//15.0f

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

//WINGS COMPONENT ----------------------------
#define WINGS_X_OFFSET 4.0f
#define WINGS_Y_OFFSET -8.0f

#define WINGS_VX 0.04f
#define WINGS_JUMP_VY -0.3f
//--------------------------------------------

#define ANI_ID_KOOPA 22000
#define ANI_ID_KOOPA_WALK 0
#define ANI_ID_KOOPA_HIDE 10
#define ANI_ID_KOOPA_ROLL 30
#define ANI_ID_KOOPA_POP 50

class CKoopaTroopa : public CEnemy
{
protected:
	//recovering
	int recovering_time;

	//
	float on_ground_y;
	float on_ground_x;

	//WINGS COMPONENT ----------------------------
	bool bornWithWings;
	CWing* wings;
	//--------------------------------------------
public:
	CKoopaTroopa(float x, float y, bool haveWings = false);

	virtual void Prepare(DWORD dt);
	virtual void LivingPrepare(DWORD dt);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Living(DWORD dt);
	virtual void Hiding(DWORD dt);
	virtual void Rolling(DWORD dt);
	virtual void Poping(DWORD dt);

	//virtual int IsBlocking() { return state == KOOPA_STATE_HIDE || state == KOOPA_STATE_POP; }
	virtual int IsCollidable() { return life; };	
	virtual int IsLinkedTo(CGameObject* obj) { return obj == carrier; }

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
	virtual int GetObjectAniID();

	virtual void SetState(int state);

	virtual void UnderAttack(CGameObject* by_another);

	virtual void Bounce();
	virtual void Hide(DWORD dt);
	virtual void Pop(DWORD dt);

	void Patrol();

	virtual void Refresh();

	//WINGS COMPONENT ----------------------------
	virtual void GrowWings();
	virtual void LoseWings();
	virtual void Flutter();
	//--------------------------------------------
};
