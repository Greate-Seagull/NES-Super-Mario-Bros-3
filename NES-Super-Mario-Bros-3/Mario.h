#pragma once
#include "Creature.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"

#define MARIO_VX 0.0f
#define MARIO_VY 0.0f
#define MARIO_SMALL_WALKING_MAX_VX 0.1f
#define MARIO_SMALL_RUNNING_MAX_VX 0.4f


#define MARIO_SMALL_WALKING_AX 0.0002f
#define MARIO_SMALL_RUNNING_AX 0.00024f

#define MARIO_BIG_WALKING_AX 0.00016f
#define MARIO_BIG_RUNNING_AX 0.0002f

#define MARIO_JUMPING_AY -0.0002f
#define MARIO_SHARP false

#define TICK_DECELERATE 500
#define TICK_FREEFALL 500

//life
#define	MARIO_LEVEL_SMALL	1.0f
#define	MARIO_LEVEL_BIG		2.0f
#define MARIO_LEVEL_RACOON	3.0f

//state
#define MARIO_STATE_DIE				-2

//velocity division
#define SPEED_1_FRAME	0.4f
#define SPEED_2_FRAMES	0.2f
#define SPEED_3_FRAMES	0.16f
#define SPEED_4_FRAMES	0.12f
#define SPEED_5_FRAMES	0.08f
#define SPEED_6_FRAMES	0.04f
#define SPEED_IDLE		0.0f

#define MARIO_ACCEL_WALK_X	0.0005f
#define MARIO_ACCEL_RUN_X	0.0007f

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_RUN_SPEED_Y	0.6f

#define MARIO_GRAVITY			0.002f

#define MARIO_JUMP_DEFLECT_SPEED  0.4f

#pragma region ANIMATION_ID

// 10000
#define ID_ANI_MARIO 10000
// 1000
#define ID_ANI_SMALL 0
#define ID_ANI_BIG 1000
#define ID_ANI_RACOON 2000
// 100
#define ID_ANI_IDLE 0
#define ID_ANI_RUN_6_FRAMES 160
#define ID_ANI_RUN_5_FRAMES 150
#define ID_ANI_RUN_4_FRAMES 140
#define ID_ANI_RUN_3_FRAMES 130
#define ID_ANI_RUN_2_FRAMES 120
#define ID_ANI_RUN_1_FRAMES 110
#define ID_ANI_FLY			100
#define ID_ANI_JUMP 10
#define ID_ANI_FALL 20
#define ID_ANI_SIT 30
// 1
#define ID_ANI_LEFT 0
#define ID_ANI_RIGHT 1

#pragma endregion

#define GROUND_Y 160.0f

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 12


#define MARIO_UNTOUCHABLE_TIME 2500

class CMario : public CCreature
{
	float maxVx;

	bool isSitting;
	bool isBoost;

	int aniID;

	DWORD decelerateTick;
	DWORD freefallTick;

	int untouchable; 
	ULONGLONG untouchable_start;
	BOOLEAN isOnPlatform;
	int coin; 

	/*void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);

	int GetAniIdBig();
	int GetAniIdSmall();*/

public:
	CMario(float x, float y);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	//void SetState(int state);

	/*int IsCollidable()
	{ 
		return (state != MARIO_STATE_DIE); 
	}*/

	//int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable==0); }

	//void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	//void SetLevel(int l);
	//void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void DetermineState();
	void ApplyState(float &ax, float &ay);
	void ApplySmallState(float &ax, float &ay);
	void ApplyBigState(float& ax, float& ay);
	void DetermineAccelerator(float& ax, float& ay, DWORD& t);

	void ChangeAnimation();
	void Accelerate(DWORD t, float ax, float ay);
};