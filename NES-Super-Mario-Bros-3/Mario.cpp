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
	is_sitting = false;
	is_boosting = false;
	is_falling = false;
	is_invulnerable = false;

	/*untouchable = 0;
	untouchable_start = -1;
	isOnPlatform = false;
	coin = 0;*/

	SetLevel(MARIO_LEVEL_SMALL);
	SetState(STATE_LIVE);
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{	
	dt = min(17, dt);
	dt = max(15, dt);	
	Inphase(dt, coObjects);
	
	//DebugOutTitle(L"life: %f", life);

	//// reset untouchable timer if untouchable time has passed
	//if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	//{
	//	untouchable_start = 0;
	//	untouchable = 0;
	//}
}

void CMario::Inphase(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	switch (state)
	{
	case STATE_LIVE:
		ProcessInput();
		UpdateMovementState();
		ComputeAccelerator(dt);
		Accelerate(dt);
		CCollision::GetInstance()->Process(this, dt, coObjects);
		Move(dt);
		Carrying();
		break;
	case MARIO_STATE_GAIN_POWER:
		GainingPower();
		break;
	case MARIO_STATE_LOSE_POWER:
		if (GetTickCount64() - action_time > MARIO_LEVELDOWN_TRANSFORM_TIME)
		{
			SetState(STATE_LIVE);
			StartInvulnerable();
		}
		break;
	}
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
			SetLevel(MARIO_LEVEL_BIG);
		case EFFECT_RACOONIZE:
			SetState(MARIO_STATE_GAIN_POWER);
			SetLevel(MARIO_LEVEL_RACOON);
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
	if (!is_invulnerable || (GetTickCount64() - action_time) % 120 >= 60)
	{
		ChangeAnimation();
	
		float draw_x = x;
		float draw_y = y;
		ChangeDrawPosition(draw_x, draw_y);

		CAnimations::GetInstance()->Get(aniID)->Render(draw_x, draw_y);
	}
	
	RenderBoundingBox();

	//DebugOutTitle(L"Coins: %d", coin);
}

void CMario::ProcessInput()
{
	KeyStateManager* keyState = CGame::GetInstance()->GetKeyboard();

	/*if (keyState->IsPressed(VK_1))
	{
		SetLevel(MARIO_LEVEL_SMALL);
	}
	else if (keyState->IsPressed(VK_2))
	{
		SetLevel(MARIO_LEVEL_BIG);
	}
	else if (keyState->IsPressed(VK_3))
	{
		SetLevel(MARIO_LEVEL_RACOON);
	}*/

	is_sitting = keyState->IsHold(VK_DOWN) && !keyState->IsHold(VK_LEFT) && !keyState->IsHold(VK_RIGHT);
	is_boosting = keyState->IsHold(VK_A);
}

void CMario::UpdateMovementState()
{
	if (is_boosting)
	{
		ax = MARIO_SMALL_RUNNING_AX;
		maxVx = MARIO_SMALL_RUNNING_MAX_VX;
	}
	else
	{
		ax = MARIO_SMALL_WALKING_AX;

		if (abs(vx) <= MARIO_SMALL_WALKING_MAX_VX)
			maxVx = MARIO_SMALL_WALKING_MAX_VX;

		Drop();
	}

	if (is_sitting)
	{

	}

	if (is_falling)
	{

	}

	if (is_invulnerable && GetTickCount64() - action_time > MARIO_INVULNERABLE_TIME)
	{
		is_invulnerable = false;
	}
}

void CMario::ComputeAccelerator(DWORD& t)
{
	KeyStateManager* keyState = CGame::GetInstance()->GetKeyboard();

	float new_ax = 0.0f;
	float new_ay = GAME_GRAVITY;
	int new_nx = 0;

	if (keyState->IsHold(VK_LEFT))
	{
		if (vx > 0)
		{
			float temp_ax = min(MARIO_BRAKE_AX, fabs(vx / t));
			new_ax += -temp_ax;
		}
		else
		{		
			new_ax += -ax;
		}

		new_nx--;
	}
	if (keyState->IsHold(VK_RIGHT))
	{
		if (vx < 0)
		{
			float temp_ax = min(MARIO_BRAKE_AX, fabs(vx / t));
			new_ax += temp_ax;
		}
		else
		{
			new_ax += ax;
		}

		new_nx++;
	}
	if (new_ax == 0.0f)
	{
		new_ax = min(MARIO_DECELERATE_AX, fabs(vx) / t);

		if (vx > 0)
		{
			new_ax = -new_ax;
		}
	}

	//Jump
	if (keyState->IsPressed(VK_S) && !is_falling)
	{
		new_ay += MARIO_START_JUMPING_AY;
		startJumpingPosition = y;
	}
	else if (keyState->IsReleased(VK_S) || abs(y - startJumpingPosition) >= MARIO_MAX_JUMP_HEIGHT || vy > 0.0f)
	{
		is_falling = true;		
	}
	else if (keyState->IsHold(VK_S) && !is_falling)
	{
		if(abs(vy) <= MARIO_SMALL_JUMPING_MAX_VY)
			new_ay += -GAME_GRAVITY;
	}
	//------------------------------------------

	if (new_nx)
	{
		this->nx = new_nx;
	}

	ax = new_ax;
	ay = new_ay;

	//DebugOutTitle(L"ax: %f", applied_ax);
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

	int direction = (nx <= 0) ? ID_ANI_LEFT : ID_ANI_RIGHT;
	
	aniID = ID_ANI_MARIO + level + action + direction;	

	if(timePerFrame < TIME_FRAME)
		CAnimations::GetInstance()->Get(aniID)->ChangeTimePerFrame(timePerFrame);
}

void CMario::ChangeAnimationInLivingState(int &actionID, DWORD &timePerFrame)
{
	if (is_sitting && life > MARIO_LEVEL_SMALL)
	{
		actionID = ID_ANI_SIT;
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

void CMario::ChangeDrawPosition(float& x, float& y)
{
	LPANIMATION_FRAME currentFrame = CAnimations::GetInstance()->Get(aniID)->GetCurrentFrame();

	if (currentFrame)
	{
		LPSPRITE sprite = currentFrame->GetSprite();
		float sprite_height = (float)sprite->GetHeight();
		y = this->y + (bbox_height - sprite_height) / 2;
	}
}

void CMario::Accelerate(DWORD t)
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
	SetLevel(life - 1);	
}

void CMario::BackJump()
{
	if (life)
	{
		vy = MARIO_JUMP_DEFLECT_VX;
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
	action_time = GetTickCount64();
}

void CMario::GainingPower()
{
	int duration;
	if (life == MARIO_LEVEL_RACOON)
	{
		duration = MARIO_RACOON_TRANSFORM_TIME;
	}
	else if (life == MARIO_LEVEL_BIG)
	{
		duration = MARIO_BIG_TRANSFORM_TIME;
	}
	else
	{
		return;
	}


	if (GetTickCount64() - action_time > duration)
	{
		SetState(STATE_LIVE);
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (life == l)
		return;

	life = l;

	if (l == MARIO_LEVEL_RACOON)
	{
		y += (bbox_height - MARIO_RACOON_BBOX_HEIGHT) / 2;
		bbox_height = MARIO_RACOON_BBOX_HEIGHT;
		bbox_width = MARIO_RACOON_BBOX_WIDTH;
	}
	else if (l == MARIO_LEVEL_BIG)
	{
		y += (bbox_height - MARIO_BIG_BBOX_HEIGHT) / 2;
		bbox_height = MARIO_BIG_BBOX_HEIGHT;
		bbox_width = MARIO_BIG_BBOX_WIDTH;
	}
	else
	{
		y += (bbox_height - MARIO_SMALL_BBOX_HEIGHT) / 2;
		bbox_height = MARIO_SMALL_BBOX_HEIGHT;
		bbox_width = MARIO_SMALL_BBOX_WIDTH;
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
		case MARIO_STATE_LOSE_POWER:
			action_time = GetTickCount64();
			break;
		case STATE_LIVE:
			break;
		case STATE_DIE:
			break;
	}
}

