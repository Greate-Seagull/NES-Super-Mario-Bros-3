#pragma once
#include "Creature.h"
#include "RacoonTail.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"

//MOVING
#define MARIO_SMALL_WALKING_MAX_VX 0.1f
#define MARIO_SMALL_RUNNING_MAX_VX 0.25f
#define MARIO_SMALL_JUMPING_MAX_VY 0.15f
#define MARIO_SMALL_RUNNING_AX 0.00016f//0.0002f

#define MARIO_DECELERATE_AX 0.00016f
#define MARIO_BRAKE_AX 0.0004f

#define MARIO_START_JUMP_VY -0.25f
#define MARIO_MAX_JUMP_HEIGHT 60.0f

#define MARIO_JUMP_DEFLECT_VY  -0.25f

#define MARIO_VX_BAND 0.02f
#define MARIO_MAX_MOMENTUM 7

//TIME
#define MARIO_BIG_TRANSFORM_TIME 600
#define MARIO_RACOON_TRANSFORM_TIME 350
#define MARIO_SMALL_TRANSFORM_TIME 700
#define MARIO_INVULNERABLE_TIME 1000
#define MARIO_ATTACK_PHASE_TIME 60
#define MARIO_KICK_TIME 200
#define MARIO_DYING_TIME 700
#define MARIO_MOMENTUM_TIME 300

//LIFE
#define	MARIO_LEVEL_SMALL	1.0f
#define	MARIO_LEVEL_BIG		2.0f
#define MARIO_LEVEL_RACOON	3.0f

//STATE
#define MARIO_STATE_GAIN_POWER 10
#define MARIO_STATE_LOSE_POWER 11

//PIPE ENTERING/EXITING
#define MARIO_PIPE_ENTRY_SPEED 0.03f
#define MARIO_PIPE_ENTRY_DOWN 20
#define MARIO_PIPE_ENTRY_UP 21
#define MARIO_PIPE_EXIT_DOWN 30
#define MARIO_PIPE_EXIT_UP 31

#define MARIO_ACCEL_WALK_X	0.0005f
#define MARIO_ACCEL_RUN_X	0.0007f

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_RUN_SPEED_Y	0.6f

#define MARIO_JUMP_DEFLECT_VX  -0.25f

#pragma region ANIMATION_ID

// 10000
=======
//ANIMATION
//OBJECT
#define ID_ANI_MARIO 0
//LEVELS
#define ID_ANI_SMALL 0
#define ID_ANI_BIG 1000
#define ID_ANI_RACOON 2000
//ACTIONS
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
//DIRECTIONS
#define ID_ANI_LEFT 0
#define ID_ANI_RIGHT 1
#define ID_ANI_FRONT 2
#define ID_ANI_BEHIND 3
// PIPE ACTION
#define ID_ANI_PIPE_ENTER 150

//BOUNDING BOX
#define MARIO_SMALL_BBOX_WIDTH  12.0f
#define MARIO_SMALL_BBOX_HEIGHT 15.0f

#define MARIO_BIG_BBOX_WIDTH  14.0f
#define MARIO_BIG_BBOX_HEIGHT 27.0f

#define MARIO_BIG_SITTING_BBOX_HEIGHT 18.0f

//TAIL POSITION
#define MARIO_RACOON_TAIL_Y_OFFSET 7.0f
#define MARIO_CARRY_OFFSET_X 10.0f

class CMario : public CCreature
{
	float ax, ay;
	float maxVx;

	bool is_falling;
	bool is_invulnerable;

	float on_ground_position;

	int changing_state_time;

	int action_duration;

	int attacking_time;
	int attack_phase;
	CRacoonTail* tail;

	int momentum;
	int decrease_momentum_time;

	bool is_flying;

	bool is_boosting;

	int special_action;

	//int coin; 

	void OnCollisionWithHarmfulObject(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithPlatform(LPCOLLISIONEVENT e);
	void OnCollisionWithHelpfulObject(LPCOLLISIONEVENT e);
	void OnCollisionWithBlock(LPCOLLISIONEVENT e);

public:
	CMario(float x, float y);

	void Prepare(DWORD dt);
	void RefreshInLiveState(DWORD dt);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Living(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void ChangeAnimation();
	void ChangeAnimationInLivingState(int& actionID, DWORD& timePerFrame);
	void ChangeDrawX(float& x);
	void ChangeDrawY(float& y);
	void Render();

	int IsCollidable() { return (state == STATE_LIVE); }
	int IsBlocking() { return (state != STATE_DIE /*&& untouchable==0*/); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	//void ReactionToCarry(CGameObject* by_another);
	void ReactionToTouch(CGameObject* by_another);
	void ReactionToAttack1(CGameObject* by_another);
	void ReactionToAttack2(CGameObject* by_another);
	void ReactionToAttack3(CGameObject* by_another);
	void ReactionToBigger(CGameObject* by_another);
	void ReactionToRacoonize(CGameObject* by_another);

	void SetLife(float l);
	float GetLife() { return life; }

	void SetState(int state);
	void ToGainingPowerState();
	void ToLosingPowerState();
	void ToDyingState();

	void StartNormalActions(DWORD& t);

	void Accelerate(float ax, float ay, DWORD t);

	void UnderAttack(CGameObject* by_another);

	bool SetSpecialAction(int actionID);
	void StartSpecialActions();
	void DoSpecialActions(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void Sit();
	void Sitting();

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

	void Carry(CHarmfulObject* weapon);
	void Carrying();
	void Tosh();

	void Kick();
	void Kicking(DWORD dt);

	void PipeEntry(int warp_direction, int scene_destination);
	void PipeEntryUp(DWORD dt);
	void PipeEntryDown(DWORD dt);
	void PipeExitUp(DWORD dt);
	void PipeExitDown(DWORD dt);

	void StartInvulnerable();
	void Invulnerable(DWORD dt);

	void GainingPower(DWORD dt);
	void LosingPower(DWORD dt);
	void LoseRacoonPower();

	void Dying(DWORD dt);

	bool IsFlying() { return is_flying; }
	void Fly();
	void Flying();
};