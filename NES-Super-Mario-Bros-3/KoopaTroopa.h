#pragma once

#include "Creature.h"

#define KOOPA_LIFE 2.0f

#define KOOPA_BBOX_HEIGHT_FOOT_1	26.0f
#define KOOPA_BBOX_HEIGHT_FOOT_2	27.0f
#define KOOPA_BBOX_HEIGHT_HIDE		16.0f
#define KOOPA_BBOX_WIDTH 15.0f

#define KOOPA_VX 0.03125f
#define KOOPA_VY 0.0f
#define KOOPA_ROLL_VX 0.225f
#define KOOPA_POP_VX 0.0625f
#define KOOPA_BOUNCE_VY -0.08f
#define KOOPA_FALL

#define KOOPA_HIDE_TIME 7000
#define KOOPA_POP_TIME 2000
#define KOOPA_POP_MAX_FRAME_TIME 500

#define KOOPA_STATE_HIDE 1
#define KOOPA_STATE_ROLL 2
#define KOOPA_STATE_POP 3

#define ANI_ID_KOOPA 5100
#define ANI_ID_KOOPA_WALK 0
#define ANI_ID_KOOPA_HIDE 10
#define ANI_ID_KOOPA_ROLL 30
#define ANI_ID_KOOPA_POP 50

#define ANI_ID_KOOPA_LEFT 0
#define ANI_ID_KOOPA_RIGHT 1
#define ANI_ID_KOOPA_DOWN 0
#define ANI_ID_KOOPA_UP 10

class CKoopaTroopa : public CCreature
{
	//fall sensor
	bool start_falling;
	bool is_falling;

	//recovering
	int recovering_time;
public:
	CKoopaTroopa(float x, float y);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void InPhase(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void InPhaseLivingState(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void InPhaseHidingState(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void InPhaseRollingState(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void InPhasePopingState(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	virtual int IsCollidable() { return 1; };
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithPlatform(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithHarmfulObject(LPCOLLISIONEVENT e);

	virtual void Reaction(CGameObject* by_another, int action);
	virtual void ReactionInLivingState(CGameObject* by_another, int action);
	virtual void ReactionInHidingState(CGameObject* by_another, int action);
	virtual void ReactionInRollingState(CGameObject* by_another, int action);

	virtual void Render();
	virtual void ChangeAnimation();

	virtual void SetState(int state);

	virtual void UnderAttack(CGameObject* by_another);
	virtual void UnderDestructrion(CGameObject* by_another);

	virtual void Bounce();
	virtual void Hide(DWORD dt);
	virtual void Pop(DWORD dt);

	void InitiateFallSensor();
	void ThinkOfFalling();
	void AwareOfNotFalling();
	void Falling();
	bool IsGoingToFall();
	void Patrol();

	virtual void LookForMario();
};
