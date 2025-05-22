#pragma once

#include "Goomba.h"
#include "Wings.h"

#define PARAGOOMBA_VY -0.1f
#define PARAGOOMBA_HIGH_JUMP_VY -0.3f
#define PARAGOOMBA_LIFE 2.0f

#define PARAGOOMBA_BBOX_HEIGHT 15.0f
#define PARAGOOMBA_BBOX_WIDTH 16.0f
#define PARAGOOMBA_BBOX_DIE_HEIGHT 9

#define PARAGOOMBA_STATE_GOOMBA 10

#define PARAGOOMBA_CHASE_RUN_DURATION 30 * 16
#define PARAGOOMBA_CHASE_MAX_MOMENTUM 4

#define WINGS_DISTANCE_BETWEEN 14.0f
#define WINGS_Y_OFFSET 6.0f

//animations
//object
#define ANI_ID_PARAGOOMBA 21000

class CParagoomba : public CGoomba
{
	//Chasing
	float start_y;
	int momentum;
	DWORD start_momentum;

	//Swinging
	CWing* wings;	
public:
	CParagoomba(float x, float y);

	void ChangeAnimation();
	void Render();

	void OnReactionToAttack1(LPCOLLISIONEVENT e);

	void SetState(int state);
	void ToStateLiving();
	void ToStateDying();

	void Prepare(DWORD dt);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Living(DWORD dt);

	//chasing
	void LookforMario();
	void ChaseMario(DWORD dt);
	void SetMomentum(int m);

	//swinging
	void Flutter();
	void LoseWings();
};