#pragma once
#include "Creature.h"
#include "RacoonTail.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"

#define MARIO_SMALL_WALKING_MAX_VX 0.1f
#define MARIO_SMALL_RUNNING_MAX_VX 0.25f
#define MARIO_SMALL_JUMPING_MAX_VY 0.15f

#define MARIO_VX_BAND 0.02f
#define MARIO_MAX_MOMENTUM 7

#define MARIO_SMALL_RUNNING_AX 0.0002f

#define MARIO_BIG_WALKING_AX 0.00016f
#define MARIO_BIG_RUNNING_AX 0.0002f

#define MARIO_DECELERATE_AX 0.00016f
#define MARIO_BRAKE_AX 0.0004f

#define MARIO_START_JUMPING_AY -0.015f - GAME_GRAVITY
#define MARIO_SHARP false

#define MARIO_MAX_JUMP_HEIGHT 60.0f

#define MARIO_BIG_TRANSFORM_TIME 600
#define MARIO_RACOON_TRANSFORM_TIME 350
#define MARIO_SMALL_TRANSFORM_TIME 700
#define MARIO_INVULNERABLE_TIME 1000
#define MARIO_ATTACK_PHASE_TIME 60
#define MARIO_KICK_TIME 200
#define MARIO_DYING_TIME 700
#define MARIO_MOMENTUM_TIME 300

//life
#define	MARIO_LEVEL_SMALL	1.0f
#define	MARIO_LEVEL_BIG		2.0f
#define MARIO_LEVEL_RACOON	3.0f

//state
#define MARIO_STATE_GAIN_POWER 10
#define MARIO_STATE_LOSE_POWER 11

#define MARIO_ACCEL_WALK_X	0.0005f
#define MARIO_ACCEL_RUN_X	0.0007f

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_RUN_SPEED_Y	0.6f

#define MARIO_JUMP_DEFLECT_VX  -0.25f

#pragma region ANIMATION_ID

// 10000
#define ID_ANI_MARIO 0
// 1000
#define ID_ANI_SMALL 0
#define ID_ANI_BIG 1000
#define ID_ANI_RACOON 2000
// 100
#define ID_ANI_IDLE 0
#define ID_ANI_JUMP 10
#define ID_ANI_FALL 20
#define ID_ANI_SIT 30
#define ID_ANI_RUN 40
#define ID_ANI_SUPER_RUN 45
#define ID_ANI_BRACE 50
#define ID_ANI_LEVEL_UP 60
#define ID_ANI_LEVEL_DOWN 70
#define ID_ANI_ATTACK 80
#define ID_ANI_KICK 90
#define ID_ANI_DIE 200
#define ID_ANI_CARRY 100
// 1
#define ID_ANI_LEFT 0
#define ID_ANI_RIGHT 1
#define ID_ANI_FRONT 2
#define ID_ANI_BEHIND 3

#pragma endregion

#define MARIO_SMALL_BBOX_WIDTH  12.0f
#define MARIO_SMALL_BBOX_HEIGHT 15.0f

#define MARIO_BIG_BBOX_WIDTH  14.0f
#define MARIO_BIG_BBOX_HEIGHT 27.0f

#define MARIO_BIG_SITTING_BBOX_HEIGHT 18.0f

#define MARIO_RACOON_TAIL_Y_OFFSET 7.0f
#define MARIO_CARRY_OFFSET_X 10.0f

class CMario : public CCreature
{
	float ax, ay;
	float maxVx;

	bool is_sitting;
	bool is_boosting;
	bool is_falling;
	bool is_invulnerable;

	bool want_to_carry;

	float startJumpingPosition;

	int changing_state_time;

	int action_duration;

	bool is_attacking;
	int attacking_time;
	int attack_phase;
	CRacoonTail* tail;

	bool is_kicking;

	int momentum;
	int decrease_momentum_time;

	//int coin; 

	void OnCollisionWithHarmfulObject(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithPlatform(LPCOLLISIONEVENT e);
	void OnCollisionWithHelpfulObject(LPCOLLISIONEVENT e);
	void OnCollisionWithBlock(LPCOLLISIONEVENT e);

public:
	CMario(float x, float y);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void InPhase(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Living(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void ChangeAnimation();
	void ChangeAnimationInLivingState(int& actionID, DWORD& timePerFrame);
	void ChangeDrawX(float& x);
	void ChangeDrawY(float& y);
	void Render();

	int IsCollidable() { return (state != STATE_DIE); }
	int IsBlocking() { return (state != STATE_DIE /*&& untouchable==0*/); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void Reaction(CGameObject* by_another, int action);

	void SetLevel(float l);

	void SetState(int state);
	void ToGainingPowerState();
	void ToLosingPowerState();
	void ToDyingState();

	void TriggerActions();
	void ComputeAccelerator(float& ax, float& ay, DWORD& t);

	void Accelerate(float ax, float ay, DWORD t);

	void UnderAttack(CGameObject* by_another);

	void Sit();
	void Stand();

	void Run();
	void UpdateMomentum(DWORD dt);
	void Walk();

	void Attack();
	void Attacking(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void ToAttackPhase(int phase);
	void UntriggerTail();

	void BackJump();
	void Jump();

	void Carry();
	void Carrying();
	void Tosh();

	void Kick();
	void Kicking(DWORD dt);

	void StartInvulnerable();
	void Invulnerable(DWORD dt);

	void GainingPower(DWORD dt);
	void LosingPower(DWORD dt);
	void LoseRacoonPower();

	void Dying(DWORD dt);
};