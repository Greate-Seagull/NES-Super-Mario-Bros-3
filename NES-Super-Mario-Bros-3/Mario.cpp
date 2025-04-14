#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"
#include "Platform.h"
#include "SuperMushroom.h"

#include "Collision.h"

CMario::CMario(float x, float y):
	CCreature(x, y)
{
	isSitting = false;
	isBoost = false;
	isFalling = false;
	maxVx = MARIO_SMALL_WALKING_MAX_VX;

	untouchable = 0;
	untouchable_start = -1;
	isOnPlatform = false;
	coin = 0;

	SetLevel(MARIO_LEVEL_SMALL);
	state = STATE_LIVE;

	ax = 0.0f;
	ay = 0.0f;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{	
	dt = min(17, dt);
	dt = max(15, dt);	

	ProcessInput();
	ApplyState();
	ComputeAccelerator(dt);
	Accelerate(dt);
	CCollision::GetInstance()->Process(this, dt, coObjects);
	Move(dt);
	Carrying();
	//DebugOutTitle(L"life: %f", life);

	//// reset untouchable timer if untouchable time has passed
	//if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	//{
	//	untouchable_start = 0;
	//	untouchable = 0;
	//}
}

void CMario::OnNoCollision(DWORD dt)
{
	isOnPlatform = false;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{	
	if (dynamic_cast<CHarmfulObject*>(e->obj))
		OnCollisionWithHarmfulObject(e);
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
			UnderAttack((CHarmfulObject*)by_another);
			break;
		case EFFECT_BIGGER:
			SetLevel(MARIO_LEVEL_BIG);
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
	else if (isBoost)
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
	coin++;
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
		isFalling = false;
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
	CAnimations* animations = CAnimations::GetInstance();

	ChangeAnimation();

	animations->Get(aniID)->Render(x, y);

	RenderBoundingBox();
	
	//DebugOutTitle(L"Coins: %d", coin);
}

//void CMario::SetState(int state)
//{
//	// DIE is the end state, cannot be changed! 
//	if (this->state == MARIO_STATE_DIE) return; 
//
//	switch (state)
//	{
//	case MARIO_STATE_RUNNING_RIGHT:
//		if (isSitting) break;
//		maxVx = MARIO_RUNNING_SPEED;
//		nx = 1;
//		break;
//	case MARIO_STATE_RUNNING_LEFT:
//		if (isSitting) break;
//		maxVx = -MARIO_RUNNING_SPEED;
//		nx = -1;
//		break;
//	case MARIO_STATE_WALKING_RIGHT:
//		if (isSitting) break;
//		maxVx = MARIO_WALKING_SPEED;
//		nx = 1;
//		break;
//	case MARIO_STATE_WALKING_LEFT:
//		if (isSitting) break;
//		maxVx = -MARIO_WALKING_SPEED;
//		nx = -1;
//		break;
//	case MARIO_STATE_JUMP:
//		if (isSitting) break;
//		if (isOnPlatform)
//		{
//			if (abs(this->vx) == MARIO_RUNNING_SPEED)
//				vy = -MARIO_JUMP_RUN_SPEED_Y;
//			else
//				vy = -MARIO_JUMP_SPEED_Y;
//		}
//		break;
//
//	case MARIO_STATE_RELEASE_JUMP:
//		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
//		break;
//
//	case MARIO_STATE_SIT:
//		if (isOnPlatform && life != MARIO_LEVEL_SMALL)
//		{
//			state = MARIO_STATE_IDLE;
//			isSitting = true;
//			vx = 0; vy = 0.0f;
//			y +=MARIO_SIT_HEIGHT_ADJUST;
//		}
//		break;
//
//	case MARIO_STATE_SIT_RELEASE:
//		if (isSitting)
//		{
//			isSitting = false;
//			state = MARIO_STATE_IDLE;
//			y -= MARIO_SIT_HEIGHT_ADJUST;
//		}
//		break;
//
//	case MARIO_STATE_IDLE:
//		vx = 0.0f;
//		break;
//
//	case MARIO_STATE_DIE:
//		vy = -MARIO_JUMP_DEFLECT_SPEED;
//		vx = 0;
//		break;
//	}
//
//	CGameObject::SetState(state);
//}

void CMario::ProcessInput()
{
	KeyStateManager* keyState = CGame::GetInstance()->GetKeyboard();

	if (keyState->IsPressed(VK_1))
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
	}

	isSitting = keyState->IsHold(VK_DOWN) && !keyState->IsHold(VK_LEFT) && !keyState->IsHold(VK_RIGHT);
	isBoost = keyState->IsHold(VK_A);
}

void CMario::ApplyState()
{
	if (isBoost)
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

	if (isSitting)
	{

	}

	if (isFalling)
	{

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
	if (keyState->IsPressed(VK_S) && !isFalling)
	{
		new_ay += MARIO_START_JUMPING_AY;
		startJumpingPosition = y;
	}
	else if (keyState->IsReleased(VK_S) || abs(y - startJumpingPosition) >= MARIO_MAX_JUMP_HEIGHT || vy > 0.0f)
	{
		isFalling = true;		
	}
	else if (keyState->IsHold(VK_S) && !isFalling)
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

	int direction = ID_ANI_LEFT;
	if (nx == DIRECTION_RIGHT)
	{
		direction = ID_ANI_RIGHT;
	}
	
	int action = ID_ANI_IDLE;
	if (isSitting)
	{
		action = ID_ANI_SIT;
	}
	else if (vy)
	{
		action = ID_ANI_JUMP;
	}
	else if (vx == 0)
	{
		action = ID_ANI_IDLE;
	}
	else if (vx > 0 && nx == DIRECTION_LEFT)
	{
		action = ID_ANI_BRACE;
	}
	else if (vx < 0 && nx == DIRECTION_RIGHT)
	{
		action = ID_ANI_BRACE;
	}
	else
	{
		action = ID_ANI_RUN;
		
		aniID = ID_ANI_MARIO + level + action + direction;		
		CAnimations* animations = CAnimations::GetInstance();
		DWORD time = (MARIO_SMALL_RUNNING_MAX_VX - abs(vx)) / MARIO_SMALL_RUNNING_MAX_VX * TIME_FRAME;

		animations->Get(aniID)->ChangeTimePerFrame(time);		
	}

	aniID = ID_ANI_MARIO + level + action + direction;	
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

void CMario::Reaction(CHarmfulObject* by_another, int action)
{
	if (action == ACTION_ATTACK)
		UnderAttack(by_another);
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

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (life == l)
		return;

	if (l == MARIO_LEVEL_RACOON)
	{
		life = MARIO_LEVEL_RACOON;
		y += (bbox_height - MARIO_RACOON_BBOX_HEIGHT) / 2;
		bbox_height = MARIO_RACOON_BBOX_HEIGHT;
		bbox_width = MARIO_RACOON_BBOX_WIDTH;
	}
	else if (l == MARIO_LEVEL_BIG)
	{
		life = MARIO_LEVEL_BIG;
		y += (bbox_height - MARIO_BIG_BBOX_HEIGHT) / 2;
		bbox_height = MARIO_BIG_BBOX_HEIGHT;
		bbox_width = MARIO_BIG_BBOX_WIDTH;
	}
	else
	{
		life = MARIO_LEVEL_SMALL;
		y += (bbox_height - MARIO_SMALL_BBOX_HEIGHT) / 2;
		bbox_height = MARIO_SMALL_BBOX_HEIGHT;
		bbox_width = MARIO_SMALL_BBOX_WIDTH;
	}
}

