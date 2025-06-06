#pragma once

#include "HelpfulObject.h"

#define MUSHROOM_VX 0.05f
#define MUSHROOM_VY -0.025f

#define MUSHROOM_BBOX_HEIGHT 14.0f
#define MUSHROOM_BBOX_WIDTH 14.0f

#define MUSHROOM_WAKEUP_TIME 1000 //the origin's is 250

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

	int IsCollidable() { return state > MUSHROOM_STATE_EMERGE; }

	virtual void Prepare(DWORD dt);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void SetState(int state);
	void ToStateSleeping();
	void ToStateWakingUp();
	void ToStateEmerging();
	void ToStateRunning();

	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithPlatform(LPCOLLISIONEVENT e);
	void OnCollisionWithBlock(LPCOLLISIONEVENT e);
	void OnCollisionWithMario(LPCOLLISIONEVENT e);

	void OnReactionTo(LPCOLLISIONEVENT e, int action);
	void ReactionInSleepingState(LPCOLLISIONEVENT e, int action);
	void ReactionInRunningState(LPCOLLISIONEVENT e, int action);

	virtual void Render();

	void WakeUp(DWORD dt);

	virtual void Refresh();
};
