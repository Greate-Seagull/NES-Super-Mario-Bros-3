#pragma once

#include "HelpfulObject.h"

#define MUSHROOM_VX 0.05f
#define MUSHROOM_VY -0.025f

#define MUSHROOM_BBOX_HEIGHT 16.0f
#define MUSHROOM_BBOX_WIDTH 16.0f

#define MUSHROOM_WAKEUP_TIME 500 //the origin's is 250

#define MUSHROOM_STATE_SLEEP 10
#define MUSHROOM_STATE_WAKEUP 11
#define MUSHROOM_STATE_EMERGE 12
#define MUSHROOM_STATE_RUN 13

#define ANI_ID_MUSHROOM 30000

class CSuperMushroom : public CHelpfulObject
{
	//Emerge
	float start_y;	
	int wakeup_time;

public:
	CSuperMushroom(float x, float y);

	int IsCollidable() { return 1; }

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void InPhase(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void SetState(int state);
	void ToStateSleeping();
	void ToStateWakingUp();
	void ToStateEmerging();
	void ToStateRunning();

	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithPlatform(LPCOLLISIONEVENT e);
	void OnCollisionWithBlock(LPCOLLISIONEVENT e);
	void OnCollisionWithMario(LPCOLLISIONEVENT e);

	void Reaction(CGameObject* by_another, int action);
	void ReactionInSleepingState(CGameObject* by_another, int action);
	void ReactionInRunningState(CGameObject* by_another, int action);

	virtual void Render();

	void LookAwayFromMario();
	void WakingUp(DWORD dt);
};
