#pragma once
#include "Creature.h"
#include "RacoonTail.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"

//MOVING
#define MARIO_SMALL_WALKING_MAX_VX 0.0925f
#define MARIO_SMALL_RUNNING_MAX_VX 0.2f
#define MARIO_SMALL_JUMPING_MAX_VY 0.15f
#define MARIO_SMALL_RUNNING_AX 0.00015f//0.0002f

#define MARIO_DECELERATE_AX 0.00016f
#define MARIO_BRAKE_AX 0.00075f

#define MARIO_START_JUMP_VY 0.25f
#define MARIO_MAX_JUMP_HEIGHT 60.0f

#define MARIO_FLY_V 0.1f

#define MARIO_JUMP_DEFLECT_VY  -0.25f

#define MARIO_MAX_MOMENTUM 7
#define MARIO_VX_BAND 0.015f //(MARIO_SMALL_RUNNING_MAX_VX - MARIO_SMALL_WALKING_MAX_VX) / MARIO_MAX_MOMENTUM

#define MARIO_PIPE_ENTRY_SPEED 0.03f

//TIME
#define MARIO_BIG_TRANSFORM_TIME 600
#define MARIO_RACOON_TRANSFORM_TIME 350
#define MARIO_SMALL_TRANSFORM_TIME 700
#define MARIO_INVULNERABLE_TIME 1000
#define MARIO_ATTACK_PHASE_TIME 60
#define MARIO_KICK_TIME 200
#define MARIO_DYING_TIME 700
#define MARIO_MOMENTUM_TIME 250//300
#define MARIO_FLY_COOLDOWN 250
#define MARIO_FLY_TIME 4000

//LIFE
#define	MARIO_LEVEL_SMALL	1.0f
#define	MARIO_LEVEL_BIG		2.0f
#define MARIO_LEVEL_RACOON	3.0f

//STATE
#define MARIO_STATE_GAIN_POWER 10
#define MARIO_STATE_LOSE_POWER 11

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
#define ID_ANI_SUPER_JUMP 15
#define ID_ANI_FALL 20
#define ID_ANI_SUPER_FALL 25
#define ID_ANI_SIT 30
#define ID_ANI_RUN 40
#define ID_ANI_SUPER_RUN 45
#define ID_ANI_BRACE 50
#define ID_ANI_LEVEL_UP 60
#define ID_ANI_LEVEL_DOWN 70
#define ID_ANI_ATTACK 80
#define ID_ANI_KICK 90
#define ID_ANI_CARRY 100
#define ID_ANI_FLY 200
#define ID_ANI_INTO_THE_PIPE 210
#define ID_ANI_DIE 999
//DIRECTIONS
#define ID_ANI_LEFT 0
#define ID_ANI_RIGHT 1
#define ID_ANI_FRONT 2
#define ID_ANI_BEHIND 3

//BOUNDING BOX
#define MARIO_SMALL_BBOX_WIDTH  12.0f
#define MARIO_SMALL_BBOX_HEIGHT 15.0f

#define MARIO_BIG_BBOX_WIDTH  14.0f
#define MARIO_BIG_BBOX_HEIGHT 27.0f

#define MARIO_BIG_SITTING_BBOX_HEIGHT 18.0f

//TAIL POSITION
#define MARIO_RACOON_TAIL_Y_OFFSET 7.0f
#define MARIO_CARRY_OFFSET_X 12.0f

//
#define CARD_COUNT 3

class CMario : public CCreature
{
	bool is_invulnerable;
	bool is_jumping;
	bool isInGround;
	bool isDigging;

	float on_ground_y;
	float on_ceil_y;

	int changing_state_time;

	int action_duration;

	int attacking_time;
	int attack_phase;
	int attack_direction;
	CRacoonTail* tail;

	int momentum;
	int decrease_momentum_time;

	int coins;
	int scores;
	int cards[CARD_COUNT] = {0};
	int cardIndex;	

	bool is_boosting;

	bool is_flying;
	DWORD fly_cooldown;
	DWORD total_fly_time;

	int special_action;

	bool isFastTravel;
	int destination;
	float des_x, des_y;

	void OnCollisionWithHarmfulObject(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithPlatform(LPCOLLISIONEVENT e);
	void OnCollisionWithHelpfulObject(LPCOLLISIONEVENT e);
	void OnCollisionWithBlock(LPCOLLISIONEVENT e);
	void OnCollisionWithReward(LPCOLLISIONEVENT e);

public:
	CMario(float x, float y);

	void Prepare(DWORD dt);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Living(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void ChangeAnimation();
	void ChangeAnimationInLivingState(int& actionID, DWORD& timePerFrame);
	void ChangeDrawX(float& x);
	void ChangeDrawY(float& y);
	void Render();

	int IsCollidable() { return state == STATE_LIVE; }
	int IsLinkedTo(CGameObject* obj);

	void OnNoCollisionWithBlocking(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	//void ReactionToCarry(CGameObject* by_another);
	void OnReactionToTouching(LPCOLLISIONEVENT e);
	void OnReactionToAttack1(LPCOLLISIONEVENT e);
	void OnReactionToAttack2(LPCOLLISIONEVENT e);
	void OnReactionToAttack3(LPCOLLISIONEVENT e);
	void OnReactionToBigger(LPCOLLISIONEVENT e);
	void OnReactionToRacoonize(LPCOLLISIONEVENT e);

	float GetLife() { return life; }
	void SetLife(float l);
	void ToSmallLevel();
	void ToBigLevel();
	void ToRacoonLevel();

	void SetState(int state);
	void ToGainingPowerState();
	void ToLosingPowerState();
	void ToLivingState();
	void ToDyingState();

	void StartNormalActions(DWORD& t);

	void Accelerate(float ax, float ay, DWORD t);

	void UnderAttack(CGameObject* by_another);

	bool SetSpecialAction(int actionID);
	void StartSpecialActions();
	void DoSpecialActions(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	bool Sit();
	void Sitting();

	bool Stand();

	bool Run();
	void UpdateMomentum(DWORD dt);
	bool Walk();

	bool Attack();
	void Attacking(DWORD dt);
	void ChangeAttackPhase(int phase);
	void UntriggerTail();

	void BackJump();
	void Jump();

	bool Grab(LPCOLLISIONEVENT e);
	void Carrying();
	bool Tosh();

	bool Kick();
	void Kicking(DWORD dt);

	void StartInvulnerable();
	void Invulnerable(DWORD dt);

	void GainingPower(DWORD dt);
	void LosingPower(DWORD dt);

	void Dying(DWORD dt);

	bool IsFlying() { return is_flying; }
	void Fly();

	bool IntoThePipe(int direction);
	void Digging();
	bool IsInGround() { return isInGround; }

	/*void IncreaseFlyingPoint();
	void InsertFlyingScore(float x, float y);*/

	int GetMomentum() { return momentum; }
	int GetCoins() { return coins; }
	int GetScores() { return scores; }
	int* GetCards() { return cards; }
	int GetLatestCard() { return cards[cardIndex - 1]; }

	bool IsFastTravel() { return isFastTravel; }
	int GetDestination() { return destination; }
	void GetDestinationPosition(float &x, float &y) { x = des_x; y = des_y; }

	void Refresh();

	void CreateItem(CPlayScene* ps);
};