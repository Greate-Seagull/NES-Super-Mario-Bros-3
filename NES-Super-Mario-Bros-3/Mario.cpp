#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
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

	/*isOnPlatform = false;
	coin = 0;*/

	SetState(STATE_LIVE);
	SetLevel(MARIO_LEVEL_SMALL);
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{	
	if(dt > 30)
		DebugOutTitle(L"time: %d", dt);
	dt = 16;
	InPhase(dt, coObjects);
}

void CMario::InPhase(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	switch (state)
	{
	case STATE_LIVE:
		InPhaseLivingState(dt, coObjects);
		break;
	case MARIO_STATE_GAIN_POWER:
		GainingPower();
		break;
	case MARIO_STATE_LOSE_POWER:
		LosingPower();
		break;
	}
}

void CMario::InPhaseLivingState(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	ProcessInput();

	float calculated_ax, calculated_ay;
	ComputeAccelerator(calculated_ax, calculated_ay, dt);
	Accelerate(calculated_ax, calculated_ay, dt);

	CCollision::GetInstance()->Process(this, dt, coObjects);
	Move(dt);

	Invulnerable();
	Carrying();
	Attacking(dt, coObjects);
}

void CMario::OnNoCollision(DWORD dt)
{
	//isOnPlatform = false;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{	
	if (dynamic_cast<CHarmfulObject*>(e->obj))
	{
		if(!is_invulnerable) OnCollisionWithHarmfulObject(e);
	}
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
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
			UnderAttack(by_another);
			break;
		case EFFECT_BIGGER:
			SetState(MARIO_STATE_GAIN_POWER);
			//SetLevel(MARIO_LEVEL_BIG);
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
	else if (is_boosting)
	{
		Carry(enemy);
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
	if (e->ny > 0 || e->nx)
	{
		Touch(e->obj);
	}
}

void CMario::Render()
{
	if (is_invulnerable && (GetTickCount64() - changing_state_time) % 120 < 60)
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
	RenderBoundingBox();

	if (tail)
		tail->Render();

	//DebugOutTitle(L"Coins: %d", coin);
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
	else if (vx > 0 && nx == DIRECTION_LEFT)
	{
		actionID = ID_ANI_BRACE;
	}
	else if (vx < 0 && nx == DIRECTION_RIGHT)
	{
		actionID = ID_ANI_BRACE;
	}
	else
	{
		actionID = ID_ANI_RUN;
		timePerFrame *= (MARIO_SMALL_RUNNING_MAX_VX - abs(vx)) / MARIO_SMALL_RUNNING_MAX_VX;
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

void CMario::ProcessInput()
{
	KeyStateManager* keyState = CGame::GetInstance()->GetKeyboard();

	if (keyState->IsHold(VK_DOWN) && !keyState->IsHold(VK_LEFT) && !keyState->IsHold(VK_RIGHT))
		Sit();
	else
		Stand();

	if (keyState->IsPressed(VK_A))
		Attack();
	if (keyState->IsHold(VK_A))
		Run();
	else
		Walk();
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
	SetState(MARIO_STATE_LOSE_POWER);
}

void CMario::Sit()
{
	if (is_sitting)
		return;
	if (life == MARIO_LEVEL_SMALL) //Cannot sit
	{
		is_sitting = false;
		return;
	}

	is_sitting = true;

	y += (bbox_height - MARIO_BIG_SITTING_BBOX_HEIGHT) / 2;
	bbox_height = MARIO_BIG_SITTING_BBOX_HEIGHT;
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

void CMario::Walk()
{
	is_boosting = false;

	if (abs(vx) <= MARIO_SMALL_WALKING_MAX_VX)
	{
		ax = MARIO_SMALL_WALKING_AX;
		maxVx = MARIO_SMALL_WALKING_MAX_VX;
	}
	else
	{
		ax = -MARIO_BRAKE_AX;
	}

	Drop();
}

void CMario::Attack()
{
	if (life < MARIO_LEVEL_RACOON || is_attacking)
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
		delete tail;
		tail = nullptr;

		is_attacking = false;
	}
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

void CMario::Carrying()
{
	if (weapon && weapon->IsControlled())
	{
		float weapon_x, weapon_y;

		weapon_x = x + nx * (this->bbox_width + weapon->getBBoxWidth()) / 2;
		weapon_y = y + (this->bbox_height - weapon->getBBoxHeight()) / 2;

		weapon->SetPosition(weapon_x, weapon_y);
	}
	else
	{
		Drop();
	}
}

void CMario::Drop()
{
	Touch(weapon);
	CCreature::Drop();
}

void CMario::StartInvulnerable()
{
	is_invulnerable = true;
	changing_state_time = GetTickCount64();
}

void CMario::Invulnerable()
{
	if (is_invulnerable && GetTickCount64() - changing_state_time > MARIO_INVULNERABLE_TIME)
	{
		is_invulnerable = false;
	}
}

void CMario::GainingPower()
{
	if (GetTickCount64() - changing_state_time > transfrom_duration)
	{
		nz = 0;
		SetLevel(life + 1.0f);
		SetState(STATE_LIVE);
	}
}

void CMario::LosingPower()
{
	if (GetTickCount64() - changing_state_time > transfrom_duration)
	{
		nz = 0;
		SetLevel(life - 1.0f);
		SetState(STATE_LIVE);
		StartInvulnerable();
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (life == l)
		return;

	life = l;

	if (l > MARIO_LEVEL_SMALL)
	{
		y += (bbox_height - MARIO_BIG_BBOX_HEIGHT) / 2;
		bbox_height = MARIO_BIG_BBOX_HEIGHT;
		bbox_width = MARIO_BIG_BBOX_WIDTH;
	}
	else if (l == MARIO_LEVEL_SMALL)
	{
		y += (bbox_height - MARIO_SMALL_BBOX_HEIGHT) / 2;
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
	changing_state_time = GetTickCount64();

	if (life == MARIO_LEVEL_SMALL)
	{
		transfrom_duration = MARIO_BIG_TRANSFORM_TIME;
	}
	else if (life == MARIO_LEVEL_BIG)
	{
		nz = DIRECTION_FRONT;
		transfrom_duration = MARIO_RACOON_TRANSFORM_TIME;
	}
}

void CMario::ToLosingPowerState()
{
	changing_state_time = GetTickCount64();

	if (life == MARIO_LEVEL_RACOON)
	{
		nz = DIRECTION_FRONT;
		transfrom_duration = MARIO_RACOON_TRANSFORM_TIME;
	}
	else if (life == MARIO_LEVEL_BIG)
	{
		transfrom_duration = MARIO_SMALL_TRANSFORM_TIME;
	}
	else
	{
		SetLevel(life - 1.0f);
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
			vx = 0.0f;
			vy = 0.0f;		
			break;
		case STATE_DIE:
			break;
	}
}

