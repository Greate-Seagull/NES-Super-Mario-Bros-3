#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Brick.h"
#include "BrickParticle.h"
#include "Portal.h"
#include "Platform.h"
#include "SuperMushroom.h"
#include "SuperLeaf.h"

#include "Collision.h"

CMario::CMario(float x, float y):
	CCreature(x, y)
{
	is_falling = false;
	is_invulnerable = false;

	decrease_momentum_time = 0;
	momentum = 0;
	/*isOnPlatform = false;
	coin = 0;*/

	SetState(STATE_LIVE);
	SetLevel(MARIO_LEVEL_SMALL);
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{	
	if(dt > 20)
		DebugOutTitle(L"time: %d", dt);
	//DebugOutTitle(L"Momentum: %d, vx: %f", momentum, fabs(vx));
	//dt = 16;
	InPhase(dt, coObjects);
}

void CMario::InPhase(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	switch (state)
	{
		case STATE_LIVE:
			Living(dt, coObjects);
			break;
		case MARIO_STATE_GAIN_POWER:
			GainingPower(dt);
			break;
		case MARIO_STATE_LOSE_POWER:
			LosingPower(dt);
			break;
		case STATE_DIE:		
			Dying(dt);
			break;
	}
}

void CMario::Living(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float calculated_ax, calculated_ay;
	ComputeAccelerator(calculated_ax, calculated_ay, dt);
	Accelerate(calculated_ax, calculated_ay, dt);

	CCollision::GetInstance()->Process(this, dt, coObjects);
	if (this->state != STATE_LIVE) //Check state changed after collision
		return;

	Move(dt);

	//Walk();
	TriggerActions();
	//One of lagging reason
	UpdateMomentum(dt);
	Invulnerable(dt);
	Carrying();
	Attacking(dt, coObjects);
	Kicking(dt);
}

void CMario::OnNoCollision(DWORD dt)
{
	//isOnPlatform = false;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{	
	if (dynamic_cast<CHarmfulObject*>(e->obj))
	{
		if (!is_invulnerable) OnCollisionWithHarmfulObject(e);
	}
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CBlock*>(e->obj))
		OnCollisionWithBlock(e);
	/*else if (dynamic_cast<CBrick*>(e->obj))
		OnCollisionWithBrick(e);
	else if (dynamic_cast<CDeadStateTrigger*>(e->obj))
		OnCollisionWithDeadTrigger(e);*/
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CHelpfulObject*>(e->obj))
		OnCollisionWithHelpfulObject(e);
	else if (dynamic_cast<CPlatform*>(e->obj))
		OnCollisionWithPlatform(e);
}

void CMario::Reaction(CGameObject* by_another, int action)
{
	switch (action)
	{		
		case ACTION_ATTACK:
		case ACTION_DESTROY:
			UnderAttack(by_another);
			break;
		case ACTION_NOTHING:
			Kick();
			break;
		case EFFECT_BIGGER:
			SetState(MARIO_STATE_GAIN_POWER);
			//SetLevel(MARIO_LEVEL_BIG);
			break;
		case EFFECT_RACOONIZE:
			SetState(MARIO_STATE_GAIN_POWER);
			//SetLevel(MARIO_LEVEL_RACOON);
			break;
	}
}

void CMario::OnCollisionWithHarmfulObject(LPCOLLISIONEVENT e)
{
	CHarmfulObject* enemy = dynamic_cast<CHarmfulObject*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		MeleeAttack(enemy);
		BackJump();
	}
	else if (want_to_carry)
	{
		CCreature::Carry(enemy);
	}
	else // hit by Goomba
	{
		Touch(enemy);
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	//coin++;
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
		vy = 0;
		ny = 0;
		is_falling = false;
	}	
}

void CMario::OnCollisionWithHelpfulObject(LPCOLLISIONEVENT e)
{
	Touch(e->obj);
}

void CMario::OnCollisionWithBlock(LPCOLLISIONEVENT e)
{	
	Touch(e->obj);

	if (e->ny)
	{
		vy = 0.0f;
		is_falling = (e->ny > 0);
	}

	if (e->nx)
	{
		vx = 0.0f;		
	}
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
	//RenderBoundingBox();

	if (tail)
		tail->Render();
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
		case STATE_DIE:
			action = ID_ANI_DIE;
			break;
	}
	
	int direction = 0;
	if (nz)
	{
		direction = (nz < 0) ? ID_ANI_FRONT : ID_ANI_BEHIND;
	}
	else
	{
		direction = (nx <= 0) ? ID_ANI_LEFT : ID_ANI_RIGHT;
	}
	

	aniID = ID_ANI_MARIO + level + action + direction;	

	if(timePerFrame < TIME_FRAME)
		CAnimations::GetInstance()->Get(aniID)->ChangeTimePerFrame(timePerFrame);
	
	CAnimations::GetInstance()->Get(aniID)->SwitchSprite();
}

void CMario::ChangeAnimationInLivingState(int &actionID, DWORD &timePerFrame)
{
	if (is_sitting)
	{
		actionID = ID_ANI_SIT;
	}
	else if (is_attacking)
	{
		actionID = ID_ANI_ATTACK;
	}
	else if (is_kicking)
	{
		actionID = ID_ANI_KICK;
	}
	else if (vy > 0 && life > MARIO_LEVEL_SMALL)
	{
		actionID = ID_ANI_FALL;
	}
	else if (vy)
	{
		actionID = ID_ANI_JUMP;
	}
	else if (vx == 0)
	{
		actionID = ID_ANI_IDLE;
	}
	else if (vx > 0 && nx == DIRECTION_LEFT && !weapon)
	{
		actionID = ID_ANI_BRACE;
	}
	else if (vx < 0 && nx == DIRECTION_RIGHT && !weapon)
	{
		actionID = ID_ANI_BRACE;
	}
	else if (fabs(vx) >= MARIO_SMALL_RUNNING_MAX_VX && !weapon)
	{
		actionID = ID_ANI_SUPER_RUN;
	}
	else
	{
		actionID = ID_ANI_RUN;
		timePerFrame *= (MARIO_SMALL_RUNNING_MAX_VX - abs(vx)) / MARIO_SMALL_RUNNING_MAX_VX;
	}

	if (weapon)
	{
		actionID += ID_ANI_CARRY;
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

void CMario::TriggerActions()
{
	KeyStateManager* keyState = CGame::GetInstance()->GetKeyboard();

	if (keyState->IsHold(VK_DOWN) &&
		!keyState->IsHold(VK_LEFT) && !keyState->IsHold(VK_RIGHT))
		Sit();
	else
		Stand();

	if (keyState->IsPressed(VK_A))
		Attack();
	if (keyState->IsHold(VK_A))
	{
		Run();
		Carry();
	}
	else
	{
		Walk();
		Tosh();
	}
}

void CMario::ComputeAccelerator(float &calculated_ax, float &calculated_ay, DWORD& t)
{
	KeyStateManager* keyState = CGame::GetInstance()->GetKeyboard();

	calculated_ax = 0.0f;
	calculated_ay = GAME_GRAVITY;
	int new_nx = 0;

	if (keyState->IsHold(VK_LEFT))
	{
		if (vx > 0)
		{
			float temp_ax = min(MARIO_BRAKE_AX, fabs(vx / t));
			calculated_ax += -temp_ax;
		}
		else
		{		
			calculated_ax += -ax;
		}

		new_nx += DIRECTION_LEFT;
	}
	if (keyState->IsHold(VK_RIGHT))
	{
		if (vx < 0)
		{
			float temp_ax = min(MARIO_BRAKE_AX, fabs(vx / t));
			calculated_ax += temp_ax;
		}
		else
		{
			calculated_ax += ax;
		}

		new_nx += DIRECTION_RIGHT;
	}
	if (calculated_ax == 0.0f)
	{
		calculated_ax = min(MARIO_DECELERATE_AX, fabs(vx) / t);

		if (vx > 0)
		{
			calculated_ax = -calculated_ax;
		}
	}

	//Jump
	if (keyState->IsPressed(VK_S) && !is_falling)
	{
		calculated_ay += MARIO_START_JUMPING_AY;
		startJumpingPosition = y;
		ny = DIRECTION_UP;
	}
	else if (keyState->IsReleased(VK_S) || abs(y - startJumpingPosition) >= MARIO_MAX_JUMP_HEIGHT || vy > 0.0f)
	{
		is_falling = true;
		ny = DIRECTION_DOWN;
	}
	else if (keyState->IsHold(VK_S) && !is_falling)
	{
		if(abs(vy) <= MARIO_SMALL_JUMPING_MAX_VY)
			calculated_ay += -GAME_GRAVITY;

		ny = DIRECTION_UP;
	}
	//------------------------------------------

	if (new_nx)
	{
		this->nx = new_nx;
	}
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

void CMario::Sit()
{

	if (is_sitting)
		return;
	if (life == MARIO_LEVEL_SMALL || weapon) //Cannot sit
	{
		Stand();
		return;
	}	

	UntriggerTail();
	is_sitting = true;

	y += (bbox_height - MARIO_BIG_SITTING_BBOX_HEIGHT) / 2;
	bbox_height = MARIO_BIG_SITTING_BBOX_HEIGHT;
	nz = 0;
}

void CMario::Stand()
{
	is_sitting = false;

	float new_bbox_height;
	
	if (life > MARIO_LEVEL_SMALL)
	{
		new_bbox_height = MARIO_BIG_BBOX_HEIGHT;
	}	
	else //small mario no need to change
	{
		return;
	}

	y += (bbox_height - new_bbox_height) / 2;
	bbox_height = new_bbox_height;
}

void CMario::Run()
{
	if (is_boosting)
		return;

	is_boosting = true;

	ax = MARIO_SMALL_RUNNING_AX;
	maxVx = MARIO_SMALL_RUNNING_MAX_VX;
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

void CMario::Walk()
{
	is_boosting = false;

	ax = MARIO_SMALL_RUNNING_AX;

	if (abs(vx) <= MARIO_SMALL_WALKING_MAX_VX)
	{		
		maxVx = MARIO_SMALL_WALKING_MAX_VX;
	}
	else
	{
		ax = -MARIO_DECELERATE_AX;
	}	
}

void CMario::Attack()
{
	if (life < MARIO_LEVEL_RACOON || is_attacking || is_sitting)
		return;

	is_attacking = true;
	attacking_time = 0;
	attack_phase = -1;
	tail = new CRacoonTail(x, y);
}

void CMario::Attacking(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (is_attacking == false)
		return;

	//Switching phase
	attacking_time += dt;
	int phase = attacking_time / MARIO_ATTACK_PHASE_TIME;
	ToAttackPhase(phase);
	
	//Tail's position should be done after processing phase to synchronize with animation
	if (tail)
	{
		float tail_x = x - nx * (bbox_width + tail->getBBoxWidth()) / 2.0f; //attack the opposite
		float tail_y = y + MARIO_RACOON_TAIL_Y_OFFSET;
		tail->SetPosition(tail_x, tail_y);
		tail->Update(dt, coObjects);
	}
}

void CMario::ToAttackPhase(int phase)
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
		UntriggerTail();
	}
}

void CMario::UntriggerTail()
{
	delete tail;
	tail = nullptr;

	is_attacking = false;
}

void CMario::BackJump()
{
	if (life)
	{
		vy = MARIO_JUMP_DEFLECT_VX;
		ny = DIRECTION_UP;
	}
}

void CMario::Jump()
{
}

void CMario::Carry()
{
	if (want_to_carry || is_attacking)
		return;

	want_to_carry = true;
}

void CMario::Carrying()
{
	if (!weapon)
		return;

	if (weapon->IsControlled())
	{
		float weapon_x, weapon_y;

		weapon_x = x + nx * MARIO_CARRY_OFFSET_X;
		weapon_y = y + (bbox_height - weapon->getBBoxHeight() - 2.0f) / 4.0f;

		weapon->SetPosition(weapon_x, weapon_y);
	}
	else
	{
		CCreature::Drop();
	}
}

void CMario::Tosh()
{
	want_to_carry = false;

	if (!weapon)
		return;

	CCreature::Drop();
}

void CMario::Kick()
{
	if (is_kicking == true)
		return;

	is_kicking = true;
	attacking_time = 0;
}

void CMario::Kicking(DWORD dt)
{
	if (is_kicking == false)
		return;

	attacking_time += dt;
	if (attacking_time >= MARIO_KICK_TIME)
	{
		is_kicking = false;
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
	if (changing_state_time > action_duration)
	{
		SetLevel(life + 1.0f);
		SetState(STATE_LIVE);
	}
}

void CMario::LosingPower(DWORD dt)
{
	changing_state_time += dt;
	if (changing_state_time > action_duration)
	{
		SetLevel(life - 1.0f);
		SetState(STATE_LIVE);
		StartInvulnerable();
	}
}

void CMario::SetLevel(float l)
{
	// Adjust position to avoid falling off platform
	if (life == l)
		return;

	life = l;

	if (l > MARIO_LEVEL_SMALL)
	{
		y += (bbox_height - MARIO_BIG_BBOX_HEIGHT) / 2.0f;
		bbox_height = MARIO_BIG_BBOX_HEIGHT;
		bbox_width = MARIO_BIG_BBOX_WIDTH;
	}
	else if (l == MARIO_LEVEL_SMALL)
	{
		y += (bbox_height - MARIO_SMALL_BBOX_HEIGHT) / 2.0f;
		bbox_height = MARIO_SMALL_BBOX_HEIGHT;
		bbox_width = MARIO_SMALL_BBOX_WIDTH;
	}
	else
	{
		SetState(STATE_DIE);
	}
}

void CMario::ToGainingPowerState()
{
	changing_state_time = 0;

	if (life == MARIO_LEVEL_SMALL)
	{
		action_duration = MARIO_BIG_TRANSFORM_TIME;
	}
	else if (life == MARIO_LEVEL_BIG)
	{
		nz = DIRECTION_FRONT;
		action_duration = MARIO_RACOON_TRANSFORM_TIME;
	}
}

void CMario::ToLosingPowerState()
{
	changing_state_time = 0;

	if (life == MARIO_LEVEL_RACOON)
	{
		nz = DIRECTION_FRONT;
		action_duration = MARIO_RACOON_TRANSFORM_TIME;
		LoseRacoonPower();
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

void CMario::ToDyingState()
{
	nx = 0;
	ny = DIRECTION_UP;
	nz = 0;

	vx = 0.0f;
	vy = ny * ATTACK_BOOM_VY;

	changing_state_time = 0;
}

void CMario::LoseRacoonPower()
{
	UntriggerTail();
}

void CMario::Dying(DWORD dt)
{
	changing_state_time += dt;
	if (changing_state_time >= MARIO_DYING_TIME)
	{
		Accelerate(0.0f, GAME_GRAVITY, dt);
		Move(dt);
	}
}

void CMario::SetState(int state)
{
	if (this->state == state)
	{
		return;
	}

	this->state = state;

	switch (state)
	{
		case MARIO_STATE_GAIN_POWER:
			ToGainingPowerState();
			break;
		case MARIO_STATE_LOSE_POWER:
			ToLosingPowerState();
			break;
		case STATE_LIVE:
			nz = 0;
			break;
		case STATE_DIE:
			ToDyingState();			
			break;
	}
}

