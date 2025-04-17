#pragma once

#include "HelpfulObject.h"

#define LEAF_VX 0.0f
#define LEAF_BLOWNUP_VY -0.325f
#define LEAF_FALLING_VY 0.0625f

#define LEAF_X_AMPLITUDE 16.0f
#define LEAF_Y_AMPLITUDE 2.0f
#define LEAF_X_FREQUENCY 0.00628f
#define LEAF_X_INITIAL_PHASE 0.0f

#define LEAF_GRAVITY 0.75f;

#define LEAF_BBOX_HEIGHT 15.0f
#define LEAF_BBOX_WIDTH 16.0f

#define EFFECT_RACOONIZE 30

#define LEAF_STATE_SLEEP 10
#define LEAF_STATE_BLOWN 11
#define LEAF_STATE_FALL 12

#define ANI_ID_LEAF 60200
#define ANI_ID_LEAF_LEFT	0
#define ANI_ID_LEAF_RIGHT	1

class CSuperLeaf : public CHelpfulObject
{
	float x_oscillate_phase;
	float y_oscillate_phase;
	float origin_x;
public:
	CSuperLeaf(float x, float y);

	int IsCollidable() { return 1; }

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void InPhase(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void InPhaseBlownState(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void InPhaseFallingState(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void SetState(int state);
	void ToStateSleeping();
	void ToStateBlownUp();
	void ToStateFalling();

	void Reaction(CGameObject* by_another, int action);
	void ReactionInSleepingState(CGameObject* by_another, int action);
	void ReactionInFallingState(CGameObject* by_another, int action);

	void ChangeAnimation();
	void Render();

	//Special
	void Oscillate(DWORD dt);
};