#pragma once
#include "Creature.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"

#define MARIO_VX 0.0f
#define MARIO_VY 0.0f
#define MARIO_WALKING_MAX_VX 0.1f
#define MARIO_RUNNING_MAX_VX 0.4f
#define MARIO_WALKING_AX 0.0002f
#define MARIO_RUNNING_AX 0.00024f
#define MARIO_JUMPING_AY -0.002f
#define MARIO_SHARP false

#define TICK_DECELERATE 500
#define TICK_FREEFALL 500

//life
#define	MARIO_LEVEL_SMALL	1.0f
#define	MARIO_LEVEL_BIG		2.0f
#define MARIO_LEVEL_RACOON	3.0f

//state
//appear on both state
#define MARIO_STATE_DIE				-2
#define MARIO_STATE_RELEASE			-1
#define MARIO_STATE_STANDING		0
//On land state
#define MARIO_STATE_SITTING			1
#define MARIO_STATE_WALKING			2
#define MARIO_STATE_RUNNING			3
//On sky state
#define MARIO_STATE_JUMPING			1

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.2f

#define MARIO_ACCEL_WALK_X	0.0005f
#define MARIO_ACCEL_RUN_X	0.0007f

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_RUN_SPEED_Y	0.6f

#define MARIO_GRAVITY			0.002f

#define MARIO_JUMP_DEFLECT_SPEED  0.4f

#pragma region ANIMATION_ID

#define ID_ANI_MARIO_IDLE_RIGHT 400
#define ID_ANI_MARIO_IDLE_LEFT 401

#define ID_ANI_MARIO_WALKING_RIGHT 500
#define ID_ANI_MARIO_WALKING_LEFT 501

#define ID_ANI_MARIO_RUNNING_RIGHT 600
#define ID_ANI_MARIO_RUNNING_LEFT 601

#define ID_ANI_MARIO_JUMP_WALK_RIGHT 700
#define ID_ANI_MARIO_JUMP_WALK_LEFT 701

#define ID_ANI_MARIO_JUMP_RUN_RIGHT 800
#define ID_ANI_MARIO_JUMP_RUN_LEFT 801

#define ID_ANI_MARIO_SIT_RIGHT 900
#define ID_ANI_MARIO_SIT_LEFT 901

#define ID_ANI_MARIO_BRACE_RIGHT 1000
#define ID_ANI_MARIO_BRACE_LEFT 1001

#define ID_ANI_MARIO_DIE 999

// SMALL MARIO
//Standing
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1100
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1101
//Jumping
#define ID_ANI_MARIO_SMALL_JUMP_LEFT 1200
#define ID_ANI_MARIO_SMALL_JUMP_RIGHT 1201
//Running stage
#define ID_ANI_MARIO_SMALL_RUN_1_FRAME_LEFT 1300
#define ID_ANI_MARIO_SMALL_RUN_1_FRAME_RIGHT 1301

#define ID_ANI_MARIO_SMALL_RUN_2_FRAMES_LEFT 1310
#define ID_ANI_MARIO_SMALL_RUN_2_FRAMES_RIGHT 1311

#define ID_ANI_MARIO_SMALL_RUN_3_FRAMES_LEFT 1320
#define ID_ANI_MARIO_SMALL_RUN_3_FRAMES_RIGHT 1321

#define ID_ANI_MARIO_SMALL_RUN_4_FRAMES_LEFT 1330
#define ID_ANI_MARIO_SMALL_RUN_4_FRAMES_RIGHT 1331

#define ID_ANI_MARIO_SMALL_RUN_5_FRAMES_LEFT 1340
#define ID_ANI_MARIO_SMALL_RUN_5_FRAMES_RIGHT 1341

#define ID_ANI_MARIO_SMALL_RUN_6_FRAMES_LEFT 1350
#define ID_ANI_MARIO_SMALL_RUN_6_FRAMES_RIGHT 1351
//#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1400
//#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1401
//
//#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 1500
//#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 1501


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
	BOOLEAN isSitting;
	float maxVx;

	int OnLandState;
	int OnSkyState;

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
	/*void SetState(int state);

	int IsCollidable()
	{ 
		return (state != MARIO_STATE_DIE); 
	}

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable==0); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void SetLevel(int l);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }*/

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void PrepareState();
	void DetermineState();
	void ApplyState(float &ax, float &ay);
	void DetermineAccelerator(float& ax, float& ay, DWORD& t);

	void ChangeAnimation(int& ani);
	void Accelerate(DWORD t, float ax, float ay);
};