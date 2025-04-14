#pragma once

#include "HelpfulObject.h"

#define MUSHROOM_VX 0.07f
#define MUSHROOM_VY -0.025f

#define MUSHROOM_BBOX_HEIGHT 16.0f
#define MUSHROOM_BBOX_WIDTH 16.0f

#define EFFECT_BIGGER 20

#define MUSHROOM_STATE_SLEEP 10
#define MUSHROOM_STATE_EMERGE 11
#define MUSHROOM_STATE_RUN 12

#define ANI_ID_MUSHROOM 60100

class CSuperMushroom : public CHelpfulObject
{
	//Emerge
	float start_y;	

public:
	CSuperMushroom(float x, float y);

	int IsCollidable() { return 1; }

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void InPhase(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void SetState(int state);
	void ToStateSleeping();
	void ToStateEmerging();
	void ToStateRunning();

	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithPlatform(LPCOLLISIONEVENT e);

	void Reaction(CGameObject* by_another, int action);
	void Reaction_SleepingState(CGameObject* by_another, int action);
	void Reaction_RunningState(CGameObject* by_another, int action);

	virtual void Render();

	//Special
	void LaunchEffect(CGameObject* creature);
};
