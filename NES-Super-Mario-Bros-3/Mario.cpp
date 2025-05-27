#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"
#include "PlayScene.h"

#include "Goomba.h"
#include "Coin.h"
#include "Brick.h"
#include "BrickParticle.h"
#include "Portal.h"
#include "Platform.h"
#include "SuperMushroom.h"
#include "SuperLeaf.h"
#include "Background.h"
#include "Pipe.h"

#include "Collision.h"

bool isOnPlatform = false;

float switchSceneTime = 0;
#define MAX_SCENE_TIME 800

float exitPipeTime = 0;
bool finishedExitingPipe = false;
#define MAX_EXIT_PIPE_TIME 800

int sceneDestination;

int flyingPoint = 100;

CMario::CMario(float x, float y):
	CCreature(x, y)
{
	is_invulnerable = false;
	is_jumping = false;
	is_flying = false;

	decrease_momentum_time = 0;
	momentum = 0;
	/*isOnPlatform = false;
	coin = 0;*/

	SetState(STATE_LIVE);
	SetLife(MARIO_LEVEL_SMALL);
}

void CMario::Prepare(DWORD dt)
{
	switch (state)
	{
	case STATE_LIVE:
		StartSpecialActions();
		StartNormalActions(dt);
		break;
	case MARIO_STATE_GAIN_POWER:
		GainingPower(dt);
		break;
	case MARIO_STATE_LOSE_POWER:
		LosingPower(dt);
		break;
	case MARIO_PIPE_ENTRY_DOWN:
		PipeEntryDown(dt);
		break;
	case MARIO_PIPE_ENTRY_UP:
		PipeEntryUp(dt);
		break;
	case MARIO_PIPE_EXIT_DOWN:
		PipeExitDown(dt);
		break;
	case MARIO_PIPE_EXIT_UP:
		PipeExitUp(dt);
		break;
	case STATE_DIE:		
		changing_state_time += dt;
		if (changing_state_time >= MARIO_DYING_TIME) CMovableObject::Prepare(dt);
		break;
	}
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{		
	//DebugOutTitle(L"Momentum: %d, vx: %f", momentum, fabs(vx));
	switch (state)
	{
	case STATE_LIVE:
		Living(dt, coObjects);
		break;
	case STATE_DIE:
		Dying(dt);
		break;
	}

	if (this->isOnGround) flyingPoint = 100;

	DebugOut(L"%d\n", flyingPoint);
}

void CMario::Living(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Flying();
	if (isOnPlatform) y = y + dt * PLATFORM_FALLING_SPEED;
	Move(dt);

	DoSpecialActions(dt, coObjects);

	UpdateMomentum(dt);
	Invulnerable(dt);
}

void CMario::OnNoCollisionWithBlocking(DWORD dt)
{
	isOnGround = false;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{	
	if (dynamic_cast<CPlatform*>(e->obj))
		OnCollisionWithPlatform(e);
	else if (dynamic_cast<CBlock*>(e->obj))
		OnCollisionWithBlock(e);
	else if (dynamic_cast<CHarmfulObject*>(e->obj))
	{
		if (!is_invulnerable) OnCollisionWithHarmfulObject(e);
	}
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CHelpfulObject*>(e->obj))
		OnCollisionWithHelpfulObject(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CRandomCard*>(e->obj))
		OnCollisionWithReward(e);
	/*else if (dynamic_cast<CDeadStateTrigger*>(e->obj))
		OnCollisionWithDeadTrigger(e);*/
	/*else if (dynamic_cast<CRandomCard*>(e->obj))
		OnCollisionWithRandomCard(e);*/
}

void CMario::ReactionToTouch(CGameObject* by_another)
{
	KeyStateManager* keyState = CGame::GetInstance()->GetKeyboard();	

	if (keyState->IsHold(VK_A))
		Carry((CHarmfulObject*)by_another);
	else
		Kick();
}

void CMario::ReactionToAttack1(CGameObject* by_another)
{
	if (this->y + this->bbox_height / 2.0f <= by_another->GetY() - by_another->GetBBoxHeight() / 2.0f)
	{
		CHarmfulObject::Attack(by_another);
		BackJump();
	}
	else
		UnderAttack((CHarmfulObject*) by_another);
}

void CMario::ReactionToAttack2(CGameObject* by_another)
{
	UnderAttack(by_another);
}

void CMario::ReactionToAttack3(CGameObject* by_another)
{
	UnderAttack(by_another);
}

void CMario::ReactionToBigger(CGameObject* by_another)
{
	SetState(MARIO_STATE_GAIN_POWER);
}

void CMario::ReactionToRacoonize(CGameObject* by_another)
{
	SetState(MARIO_STATE_GAIN_POWER);
}

void CMario::OnCollisionWithHarmfulObject(LPCOLLISIONEVENT e)
{
	KeyStateManager* keyState = CGame::GetInstance()->GetKeyboard();
	CHarmfulObject* enemy = dynamic_cast<CHarmfulObject*>(e->obj);

	if (e->ny < 0)
	{
		CHarmfulObject::Attack(enemy);
		BackJump();
	}
	else if (keyState->IsHold(VK_A) && Grab(enemy)); //case collision on the left or right or below
	else Touch(enemy);
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	Touch(e->obj);
	LPPLAYSCENE currentScene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	currentScene->CollectCoin();
	currentScene->InsertScore(50);
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

void CMario::OnCollisionWithPlatform(LPCOLLISIONEVENT e)
{
	if (e->ny)
	{
		vy = 0.0f; 
		isOnGround = true;
		on_ground_y = y;
	}	

	if (e->nx)
	{
		vx = 0.0f;
	}

	CPlatform* p = (CPlatform*)(e->obj);
	p->SetState(PLATFORM_STATE_FALLING);
	if (p->IsFallingType() == 1) isOnPlatform = true;
	else isOnPlatform = false;
}

void CMario::OnCollisionWithHelpfulObject(LPCOLLISIONEVENT e)
{
	Touch(e->obj);
}

void CMario::OnCollisionWithBlock(LPCOLLISIONEVENT e)
{	
	if (dynamic_cast<CPipe*>(e->obj))
	{
		Touch(e->obj);
	}

	if (e->ny)
	{
		vy = 0.0f;

		if (e->ny > 0)
		{
			is_jumping = false;
			Touch(e->obj);
		}
		else
		{
			isOnGround = true;
			on_ground_y = y;
		}
	}

	if (e->nx)
	{
		vx = 0.0f;
	}
}

void CMario::OnCollisionWithReward(LPCOLLISIONEVENT e)
{
	CRandomCard* rC = (CRandomCard*)(e->obj);
	rC->Switch(false);
}

void CMario::Render()
{
	if (is_invulnerable && changing_state_time % 120 < 60)
		return;

	//Determine animation
	ChangeAnimation();
	
	//Determine draw position
	float draw_x = x;
	float draw_y = y;

	switch (state)
	{
		case STATE_LIVE:
			if(life == MARIO_LEVEL_RACOON) ChangeDrawX(draw_x);
			break;
		case MARIO_STATE_GAIN_POWER:
			if (life == MARIO_LEVEL_SMALL) ChangeDrawY(draw_y);
			break;
		case MARIO_STATE_LOSE_POWER:
			if (life == MARIO_LEVEL_BIG) ChangeDrawY(draw_y);
			break;			
	}		

	//Render
	CAnimations::GetInstance()->Get(aniID)->Render(draw_x, draw_y, true);
}

void CMario::ChangeAnimation()
{
	int level = ID_ANI_SMALL;
	if (life == MARIO_LEVEL_RACOON)
	{
		level = ID_ANI_RACOON;
	}
	else if (life == MARIO_LEVEL_BIG)
	{
		level = ID_ANI_BIG;
	}
	
	int action;
	DWORD timePerFrame = TIME_FRAME;
	switch (state)
	{
		case STATE_LIVE:
			ChangeAnimationInLivingState(action, timePerFrame);
			break;
		case MARIO_STATE_GAIN_POWER:
			action = ID_ANI_LEVEL_UP;
			break;
		case MARIO_STATE_LOSE_POWER:
			action = ID_ANI_LEVEL_DOWN;
			break;
		case MARIO_PIPE_ENTRY_DOWN:
			action = ID_ANI_PIPE_ENTER;
			break;
		case MARIO_PIPE_ENTRY_UP:
			action = ID_ANI_PIPE_ENTER;
			break;
		case MARIO_PIPE_EXIT_DOWN:
			action = ID_ANI_PIPE_ENTER;
			break;
		case MARIO_PIPE_EXIT_UP:
			action = ID_ANI_PIPE_ENTER;
			break;
		case STATE_DIE:
			action = ID_ANI_DIE;
			break;
	}
	
	int direction = 0;
	if (state != MARIO_PIPE_ENTRY_DOWN &&
		state != MARIO_PIPE_ENTRY_UP &&
		state != MARIO_PIPE_EXIT_DOWN &&
		state != MARIO_PIPE_EXIT_UP)
	{
		if (nz)
		{
			direction = (nz < 0) ? ID_ANI_FRONT : ID_ANI_BEHIND;
		}
		else
		{
			direction = (nx <= 0) ? ID_ANI_LEFT : ID_ANI_RIGHT;
		}
	}

	aniID = ID_ANI_MARIO + level + action + direction;

	if(timePerFrame < TIME_FRAME)
		CAnimations::GetInstance()->Get(aniID)->ChangeTimePerFrame(timePerFrame);
	
	CAnimations::GetInstance()->Get(aniID)->SwitchSprite();
}

void CMario::ChangeAnimationInLivingState(int &actionID, DWORD &timePerFrame)
{
	//for normal actions
	if (vy > 0 && life > MARIO_LEVEL_SMALL)
		actionID = ID_ANI_FALL;
	else if (vy)
		actionID = ID_ANI_JUMP;
	else if (vx == 0)
		actionID = ID_ANI_IDLE;
	else if (vx > 0 && nx == DIRECTION_LEFT && !weapon)
		actionID = ID_ANI_BRACE;
	else if (vx < 0 && nx == DIRECTION_RIGHT && !weapon)
		actionID = ID_ANI_BRACE;
	else if (fabs(vx) >= MARIO_SMALL_RUNNING_MAX_VX && !weapon)
		actionID = ID_ANI_SUPER_RUN;
	else
	{
		actionID = ID_ANI_RUN;
		timePerFrame *= (MARIO_SMALL_RUNNING_MAX_VX - abs(vx)) / MARIO_SMALL_RUNNING_MAX_VX;
	}

	//for special actions
	switch (special_action)
	{
	case ID_ANI_IDLE:
	case ID_ANI_CARRY:
		actionID += special_action;
		break;
	default:
		actionID = special_action;
		break;
	}
}

void CMario::ChangeDrawX(float& x)
{
	LPANIMATION_FRAME currentFrame = CAnimations::GetInstance()->Get(aniID)->GetCurrentFrame();

	LPSPRITE sprite = currentFrame->GetSprite();
	float sprite_width = (float)sprite->GetWidth();

	x = this->x + nx * (bbox_width - sprite_width) / 2.0f;
}

void CMario::ChangeDrawY(float& y)
{
	LPANIMATION_FRAME currentFrame = CAnimations::GetInstance()->Get(aniID)->GetCurrentFrame();

	LPSPRITE sprite = currentFrame->GetSprite();
	float sprite_height = (float)sprite->GetHeight();

	y = this->y + (bbox_height - sprite_height) / 2.0f;
}

bool CMario::SetSpecialAction(int actionID)
{
	//validation
	if (special_action == actionID)
		return false;
	
	switch (actionID)
	{
	case ID_ANI_SIT:
		if (life < MARIO_LEVEL_BIG) return false;
		break;
	case ID_ANI_ATTACK:
		if (life < MARIO_LEVEL_RACOON) return false;
		break;
	}

	//cancel previous action
	switch (special_action)
	{
	case ID_ANI_SIT:
		Stand();
		break;
	case ID_ANI_ATTACK:
		UntriggerTail();
		break;
	case ID_ANI_CARRY:
		CCreature::Drop();
		break;
	}

	//apply new action
	special_action = actionID;
	return true;
}

void CMario::StartSpecialActions()
{
	KeyStateManager* keyState = CGame::GetInstance()->GetKeyboard();

	/*if (keyState->IsPressed(VK_UP))
		SetLife(life + 1.0f);
	else if (keyState->IsPressed(VK_DOWN))
		SetLife(life - 1.0f);*/

	/*
	if (keyState->IsHold(VK_UP))
		Fly();
	if (keyState->IsHold(VK_UP) && keyState->IsHold(VK_DOWN))
		is_flying = false;
	*/
	
	if (keyState->IsHold(VK_A))
		Run();
	else
		Walk();

	if (keyState->IsReleased(VK_A))
		Tosh();
	else if (special_action == ID_ANI_KICK || special_action == ID_ANI_CARRY);
	else if (keyState->IsHold(VK_DOWN))
		Sit();
	else if (keyState->IsPressed(VK_A))
		Attack();
}

void CMario::DoSpecialActions(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	switch (special_action)
	{
	case ID_ANI_KICK:
		Kicking(dt);
		break;
	case ID_ANI_ATTACK:
		Attacking(dt, coObjects);
		break;
	case ID_ANI_CARRY:
		Carrying();
		break;
	case ID_ANI_SIT:
		Sitting();
		break;
	}
}

void CMario::StartNormalActions(DWORD& t)
{
	KeyStateManager* keyState = CGame::GetInstance()->GetKeyboard();
	
	float calculated_ax = 0.0f;
	float calculated_ay = GAME_GRAVITY;
	int new_nx = 0;

	//go left or right
	if (keyState->IsHold(VK_LEFT))
	{
		if (vx <= 0)
		{
			calculated_ax += -ax;
		}
		else
		{		
			float temp_ax = min(MARIO_BRAKE_AX, fabs(vx / t));
			calculated_ax += -temp_ax;
		}

		new_nx += DIRECTION_LEFT;
	}
	if (keyState->IsHold(VK_RIGHT))
	{
		if (vx >= 0)
		{
			calculated_ax += ax;
		}
		else
		{
			float temp_ax = min(MARIO_BRAKE_AX, fabs(vx / t));
			calculated_ax += temp_ax;
		}

		new_nx += DIRECTION_RIGHT;
	}
	
	//jumping
	if (isOnGround && keyState->IsPressed(VK_S) && !is_flying)
	{
		isOnPlatform = false;
		ny = DIRECTION_UP;
		vy = ny * MARIO_START_JUMP_VY;
		calculated_ay = 0.0f;

		isOnGround = false;
		is_jumping = true;
	}
	else if (is_jumping && fabs(y - on_ground_y) < MARIO_MAX_JUMP_HEIGHT && keyState->IsHold(VK_S))
	{
		if (fabs(vy) <= MARIO_SMALL_JUMPING_MAX_VY)
			calculated_ay = 0.0f;
		//else calculated_ay = GAME_GRAVITY;
	}
	else
	{
		//calculated_ay = GAME_GRAVITY;
		ny = DIRECTION_DOWN;

		is_jumping = false;
	}

	//decelerate
	if (new_nx == 0)
	{
		calculated_ax = min(MARIO_DECELERATE_AX, fabs(vx) / t);

		if (vx > 0)
			calculated_ax = -calculated_ax;
	}
	else this->nx = new_nx;

	//
	Accelerate(calculated_ax, calculated_ay, t);
}

void CMario::Accelerate(float ax, float ay, DWORD t)
{
	CMovableObject::Accelerate(ax, ay, t);

	if (vx > maxVx)
	{
		vx = maxVx;
	}
	else if (vx < -maxVx)
	{
		vx = -maxVx;
	}
}

void CMario::UnderAttack(CGameObject* by_another)
{
	if (is_invulnerable)
		return;

	SetState(MARIO_STATE_LOSE_POWER);
}

bool CMario::Sit()
{
	if (SetSpecialAction(ID_ANI_SIT) == false)
		return false;

	y += (bbox_height - MARIO_BIG_SITTING_BBOX_HEIGHT) / 2.0f;
	bbox_height = MARIO_BIG_SITTING_BBOX_HEIGHT;

	return true;
}

void CMario::Sitting()
{
	KeyStateManager* keyState = CGame::GetInstance()->GetKeyboard();

	if (keyState->IsReleased(VK_DOWN) || keyState->IsHold(VK_LEFT) || keyState->IsHold(VK_RIGHT))
		SetSpecialAction(ID_ANI_IDLE);
}

bool CMario::Stand()
{
	float stand_bbox = (life < MARIO_LEVEL_BIG) ? MARIO_SMALL_BBOX_HEIGHT : MARIO_BIG_BBOX_HEIGHT;
	y += (bbox_height - stand_bbox) / 2.0f;
	bbox_height = stand_bbox;

	return true;
}

bool CMario::Run()
{
	ax = MARIO_SMALL_RUNNING_AX;
	maxVx = MARIO_SMALL_RUNNING_MAX_VX;

	return true;
}

void CMario::UpdateMomentum(DWORD dt)
{
	if (fabs(vx) <= MARIO_SMALL_WALKING_MAX_VX)
	{
		decrease_momentum_time += dt;
		if (decrease_momentum_time > MARIO_MOMENTUM_TIME)
		{
			momentum = max(0, momentum - 1);
			decrease_momentum_time %= dt;
		}
	}
	else
	{
		momentum = max(momentum, 1 + (int)((fabs(vx) - MARIO_SMALL_WALKING_MAX_VX) / MARIO_VX_BAND));
		momentum = min(momentum, MARIO_MAX_MOMENTUM);
		decrease_momentum_time = 0;
	}
}

bool CMario::Walk()
{
	ax = MARIO_SMALL_RUNNING_AX;

	if (abs(vx) <= MARIO_SMALL_WALKING_MAX_VX)
	{		
		maxVx = MARIO_SMALL_WALKING_MAX_VX;
	}
	else
	{
		ax = -MARIO_DECELERATE_AX;
	}

	return true;
}

bool CMario::Attack()
{
	if (SetSpecialAction(ID_ANI_ATTACK) == false)
		return false;

	attacking_time = 0;
	attack_phase = -1;
	tail = new CRacoonTail(x, y);

	return true;
}

void CMario::Attacking(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//Switching phase
	attacking_time += dt;
	ChangeAttackPhase(attacking_time / MARIO_ATTACK_PHASE_TIME);
	
	//Tail's position should be calculated after processing attacking phase to synchronize with the animation
	if (special_action == ID_ANI_ATTACK) // if attack hasn't ended
	{
		float tail_x = x - nx * (bbox_width + tail->GetBBoxWidth()) / 2.0f; //attack the opposite
		float tail_y = y + MARIO_RACOON_TAIL_Y_OFFSET;
		tail->SetPosition(tail_x, tail_y);
		tail->Update(dt, coObjects);
	}
}

void CMario::ChangeAttackPhase(int phase)
{
	if (this->attack_phase == phase)
	{
		return;
	}

	this->attack_phase = phase;

	if (phase == 0)
	{
		nz = 0;
	}
	else if (phase == 1)
	{
		nz = DIRECTION_FRONT;
	}
	else if (phase == 2)
	{
		nz = 0;
		nx = -nx;
	}
	else if (phase == 3)
	{
		nz = DIRECTION_BEHIND;
	}
	else if (phase == 4)
	{
		nz = 0;
		nx = -nx;
	}
	else
	{
		SetSpecialAction(ID_ANI_IDLE);
	}
}

void CMario::UntriggerTail()
{
	delete tail;
	tail = nullptr;
	nz = 0;
}

void CMario::BackJump()
{
	vy = MARIO_JUMP_DEFLECT_VY;
	ny = DIRECTION_UP;
}

void CMario::Jump()
{
}

bool CMario::Grab(CHarmfulObject* weapon)
{
	if (this->weapon)
		return false;

	SetSpecialAction(ID_ANI_CARRY);

	CCreature::Carry(weapon);

	return true;
}

void CMario::Carrying()
{
	if (!weapon)
		return;

	if (weapon->IsControlled())
	{
		float weapon_x, weapon_y;

		weapon_x = x + nx * MARIO_CARRY_OFFSET_X;
		weapon_y = y + (bbox_height - weapon->GetBBoxHeight() - 2.0f) / 4.0f;

		weapon->SetPosition(weapon_x, weapon_y);
	}
	else
	{
		SetSpecialAction(ID_ANI_IDLE);
	}
}

bool CMario::Tosh()
{
	if (!weapon)
		return false;

	Touch(weapon);
	return Kick();
}

bool CMario::Kick()
{
	SetSpecialAction(ID_ANI_KICK);

	attacking_time = 0;	

	return true;
}

void CMario::Kicking(DWORD dt)
{
	attacking_time += dt;
	if (attacking_time >= MARIO_KICK_TIME)
	{
		SetSpecialAction(ID_ANI_IDLE);
	}
}

void CMario::StartInvulnerable()
{
	is_invulnerable = true;
	changing_state_time = 0;
}

void CMario::Invulnerable(DWORD dt)
{
	if (!is_invulnerable)
		return;

	changing_state_time += dt;
	if (changing_state_time >= MARIO_INVULNERABLE_TIME)
	{
		is_invulnerable = false;
	}
}

void CMario::GainingPower(DWORD dt)
{
	changing_state_time += dt;
	if (changing_state_time >= action_duration)
	{
		SetLife(life + 1.0f);
		SetState(STATE_LIVE);
	}
}

void CMario::LosingPower(DWORD dt)
{
	changing_state_time += dt;
	if (changing_state_time > action_duration)
	{
		SetLife(life - 1.0f);
		SetState(STATE_LIVE);
		StartInvulnerable();
	}
}

void CMario::SetLife(float l)
{
	// Adjust position to avoid falling off platform
	if (life == l)
		return;

	life = l;

	if (l == MARIO_LEVEL_RACOON)
	{
		ToRacoonLevel();
	}
	else if (l == MARIO_LEVEL_BIG)
	{		
		ToBigLevel();
	}
	else if (l == MARIO_LEVEL_SMALL)
	{
		ToSmallLevel();
	}
	else
	{
		SetState(STATE_DIE);
	}
}

void CMario::ToSmallLevel()
{
	//Cancel unexisted action in previous state
	switch (special_action)
	{
	case ID_ANI_ATTACK:
	case ID_ANI_SIT:
		SetSpecialAction(ID_ANI_IDLE);
		break;
	}

	//Change position and bbox
	y += (bbox_height - MARIO_SMALL_BBOX_HEIGHT) / 2.0f;
	bbox_height = MARIO_SMALL_BBOX_HEIGHT;
	bbox_width = MARIO_SMALL_BBOX_WIDTH;
}

void CMario::ToBigLevel()
{
	//Cancel unexisted action in previous state
	switch (special_action)
	{
	case ID_ANI_ATTACK:
		SetSpecialAction(ID_ANI_IDLE);
		break;
	case ID_ANI_SIT:
		return;
	}

	//Change position and bbox
	y += (bbox_height - MARIO_BIG_BBOX_HEIGHT) / 2.0f - 1.0f;
	bbox_height = MARIO_BIG_BBOX_HEIGHT;
	bbox_width = MARIO_BIG_BBOX_WIDTH;
}

void CMario::ToRacoonLevel()
{
	y += (bbox_height - MARIO_BIG_BBOX_HEIGHT) / 2.0f - 1.0f;
	bbox_height = MARIO_BIG_BBOX_HEIGHT;
	bbox_width = MARIO_BIG_BBOX_WIDTH;
}

void CMario::ToGainingPowerState()
{
	this->state = MARIO_STATE_GAIN_POWER;

	changing_state_time = 0;

	if (life == MARIO_LEVEL_SMALL)
	{
		action_duration = MARIO_BIG_TRANSFORM_TIME;
	}
	else if (life == MARIO_LEVEL_BIG)
	{
		nz = DIRECTION_FRONT; //for changing level up animation
		action_duration = MARIO_RACOON_TRANSFORM_TIME;
	}
}

void CMario::ToLosingPowerState()
{
	this->state = MARIO_STATE_LOSE_POWER;

	changing_state_time = 0;

	if (life == MARIO_LEVEL_RACOON)
	{
		nz = DIRECTION_FRONT; //for changing level down animation
		action_duration = MARIO_RACOON_TRANSFORM_TIME;
	}
	else if (life == MARIO_LEVEL_BIG)
	{
		action_duration = MARIO_SMALL_TRANSFORM_TIME;
	}
	else //Only for small mario
	{
		//SetLevel(life - 1.0f);
		Die();
	}	
}

void CMario::ToLivingState()
{
	//Clear previous state
	switch (state)
	{
	case MARIO_STATE_GAIN_POWER:
	case MARIO_STATE_LOSE_POWER:
		nz = 0;
		break;
	case STATE_DIE:
		break;
	}

	this->state = STATE_LIVE;
}

void CMario::ToDyingState()
{
	this->state = STATE_DIE;

	nx = 0;
	ny = DIRECTION_UP;
	nz = 0;

	vx = 0.0f;
	vy = ny * ATTACK_BOOM_VY;

	ax = 0.0f;
	ay = GAME_GRAVITY;

	changing_state_time = 0;
}

void CMario::Dying(DWORD dt)
{
	if (changing_state_time >= MARIO_DYING_TIME)
		Move(dt);
}

void CMario::Fly(bool switch_fly)
{
	is_flying = switch_fly;
}

void CMario::Flying()
{
	if (is_flying == false)
		return;

	KeyStateManager* keyState = CGame::GetInstance()->GetKeyboard();

	vy = -0.1f;
	vx = nx * 0.1f;
	
	/*
	if (keyState->IsHold(VK_UP))
	{
		vy = -0.25f;
	}
	else if (keyState->IsHold(VK_DOWN))
	{
		vy = 0.25f;
	}
	else
	{
		vy = 0.0f;
	}
	*/
}

void CMario::PipeEntry(int warp_direction, int scene_destination)
{
	KeyStateManager* keyState = CGame::GetInstance()->GetKeyboard();

	if (keyState->IsHold(VK_UP) && warp_direction == 1)
	{
		SetState(MARIO_PIPE_ENTRY_UP);
		sceneDestination = scene_destination;
	}
	else if (keyState->IsHold(VK_DOWN) && warp_direction == -1)
	{
		SetState(MARIO_PIPE_ENTRY_DOWN);
		sceneDestination = scene_destination;
	}
}

void CMario::PipeEntryUp(DWORD dt)
{
	y -= MARIO_PIPE_ENTRY_SPEED * dt;

	switchSceneTime += dt;
	if (switchSceneTime >= MAX_SCENE_TIME)
	{
		switchSceneTime = 0;
		finishedExitingPipe = false;
		Fly(false);
		CGame::GetInstance()->InitiateSwitchScene(sceneDestination);
	}

	LPPLAYSCENE curr = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	curr->TogglePipeSwitch(true);
}

void CMario::PipeEntryDown(DWORD dt)
{
	y += MARIO_PIPE_ENTRY_SPEED * dt;

	switchSceneTime += dt;
	if (switchSceneTime >= MAX_SCENE_TIME)
	{
		switchSceneTime = 0;
		finishedExitingPipe = false;
		Fly(false);
		CGame::GetInstance()->InitiateSwitchScene(sceneDestination);
	}

	LPPLAYSCENE curr = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	curr->TogglePipeSwitch(true);
}

void CMario::PipeExitUp(DWORD dt)
{
	y -= MARIO_PIPE_ENTRY_SPEED * dt;

	if (!finishedExitingPipe)
	{
		exitPipeTime += dt;
		if (exitPipeTime >= MAX_EXIT_PIPE_TIME)
		{
			exitPipeTime = 0;
			finishedExitingPipe = true;
			SetState(STATE_LIVE);
		}
	}
}

void CMario::PipeExitDown(DWORD dt)
{
	y += MARIO_PIPE_ENTRY_SPEED * dt;

	if (!finishedExitingPipe)
	{
		exitPipeTime += dt;
		if (exitPipeTime >= MAX_EXIT_PIPE_TIME)
		{
			exitPipeTime = 0;
			finishedExitingPipe = true;
			SetState(STATE_LIVE);
		}
	}
}

void CMario::SetFootPlatform(bool onPlatform)
{
	isOnPlatform = onPlatform;
}

void CMario::SetState(int state)
{
	if (this->state == state)
	{
		return;
	}

	switch (state)
	{
		case MARIO_STATE_GAIN_POWER:
			ToGainingPowerState();
			break;
		case MARIO_STATE_LOSE_POWER:
			ToLosingPowerState();
			break;
		case STATE_LIVE:
			ToLivingState();
			break;
		case STATE_DIE:
			ToDyingState();			
			break;
		default: this->state = state;
	}
}

void CMario::IncreaseFlyingPoint()
{
	switch (flyingPoint)
	{
		case 100: flyingPoint = 200; break;
		case 200: flyingPoint = 400; break;
		case 400: flyingPoint = 800; break;
		case 800: flyingPoint = 1000; break;
		case 1000: flyingPoint = 2000; break;
		case 2000: flyingPoint = 4000; break;
		case 4000: flyingPoint = 8000; break;
		case 8000: flyingPoint = 10000; break;
	}
}

void CMario::InsertFlyingScore(float x, float y)
{
	LPPLAYSCENE curr = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	curr->InsertScore(x, y, flyingPoint);

	IncreaseFlyingPoint();
}