#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"
#include "Platform.h"

#include "Collision.h"

CMario::CMario(float x, float y):
	CCreature(x, y, MARIO_SHARP, MARIO_LEVEL_SMALL)
{
	isSitting = false;
	isBoost = false;
	isFalling = false;
	maxVx = MARIO_SMALL_WALKING_MAX_VX;

	decelerateTick = TICK_DECELERATE;
	freefallTick = TICK_FREEFALL;

	untouchable = 0;
	untouchable_start = -1;
	isOnPlatform = false;
	coin = 0;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	dt = min(18, dt);

	ProcessLifeState();	
	DetermineState();

	float ax, ay;	
	ApplyState(ax, ay);
	DetermineAccelerator(ax, ay, dt);
	this->Accelerate(ax, ay, dt);
	CCollision::GetInstance()->Process(this, dt, coObjects);
	Move(dt);

	DebugOutTitle(L"vx: %f", vx);

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
	if (dynamic_cast<CCreature*>(e->obj))
		OnCollisionWithCreature(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CPlatform*>(e->obj))
		OnCollisionWithPlatform(e);
}

void CMario::OnCollisionWithCreature(LPCOLLISIONEVENT e)
{
	CCreature* enemy = dynamic_cast<CCreature*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		vy = -MARIO_JUMP_DEFLECT_SPEED;
	}
	else // hit by Goomba
	{
		if (enemy->GetState() != STATE_DIE)
		{
			life -= 1.0f;
		}
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

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (life==MARIO_LEVEL_BIG)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else 
		{
			left = x - MARIO_BIG_BBOX_WIDTH/2;
			top = y - MARIO_BIG_BBOX_HEIGHT/2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH/2;
		top = y - MARIO_SMALL_BBOX_HEIGHT/2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::DetermineState()
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

void CMario::ApplyState(float& ax, float& ay)
{
	if (life == MARIO_LEVEL_RACOON)
	{
		ApplyRacoonState(ax, ay);
	}
	else if (life == MARIO_LEVEL_BIG)
	{
		ApplyBigState(ax, ay);
	}
	else
	{
		ApplySmallState(ax, ay);
	}
}

void CMario::ApplySmallState(float &ax, float &ay)
{
	if (isBoost)
	{
		ax = MARIO_SMALL_RUNNING_AX;
		maxVx = MARIO_SMALL_RUNNING_MAX_VX;
	}
	else
	{
		ax = MARIO_SMALL_WALKING_AX;
		maxVx = MARIO_SMALL_WALKING_MAX_VX;
	}
}

void CMario::ApplyBigState(float& ax, float& ay)
{
	if (isBoost)
	{
		ax = MARIO_BIG_RUNNING_AX;
		maxVx = MARIO_SMALL_RUNNING_MAX_VX;
	}
	else
	{
		ax = MARIO_BIG_WALKING_AX;
		maxVx = MARIO_SMALL_WALKING_MAX_VX;
	}
}

void CMario::ApplyRacoonState(float& ax, float& ay)
{
	if (isBoost)
	{
		ax = MARIO_BIG_RUNNING_AX;
		maxVx = MARIO_SMALL_RUNNING_MAX_VX;
	}
	else
	{
		ax = MARIO_BIG_WALKING_AX;
		maxVx = MARIO_SMALL_WALKING_MAX_VX;
	}
}

void CMario::DetermineAccelerator(float& applied_ax, float& applied_ay, DWORD& t)
{
	KeyStateManager* keyState = CGame::GetInstance()->GetKeyboard();

	float ax = 0.0f;
	float ay = MARIO_GRAVITY;
	int nx = 0;

	if (keyState->IsHold(VK_LEFT))
	{
		if (vx > 0)
		{
			float temp_ax = min(MARIO_BRAKE_AX, fabs(vx / t));
			ax += -temp_ax;
		}
		else
		{		
			ax += -applied_ax;
		}
		nx--;
	}
	if (keyState->IsHold(VK_RIGHT))
	{
		if (vx < 0)
		{
			float temp_ax = min(MARIO_BRAKE_AX, fabs(vx / t));
			ax += temp_ax;
		}
		else
		{
			ax += applied_ax;
		}

		nx++;
	}
	if (ax == 0.0f)
	{
		ax = min(MARIO_DECELERATE_AX, fabs(vx) / t);

		if (vx > 0)
		{
			ax = -ax;
		}
	}

	//Jump
	if (keyState->IsPressed(VK_S) && !isFalling)
	{
		ay += MARIO_START_JUMPING_AY;
		startJumpingPosition = y;
	}
	if (keyState->IsHold(VK_S) && !isFalling)
	{
		if(abs(vy) <= MARIO_SMALL_JUMPING_MAX_VY && abs(y - startJumpingPosition) < MARIO_MAX_JUMP_HEIGHT)
			ay += -MARIO_GRAVITY;
	}
	else if (keyState->IsReleased(VK_S))
	{
		isFalling = true;
	}
	//------------------------------------------

	if (nx)
	{
		this->nx = nx;
	}

	applied_ax = ax;
	applied_ay = ay;
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

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (life == l)
		return;

	if (l == MARIO_LEVEL_RACOON)
	{
		life = MARIO_LEVEL_RACOON;
		y += MARIO_BIG_TRANSFORMATION_OFFSET;
	}
	else if (l == MARIO_LEVEL_BIG)
	{
		life = MARIO_LEVEL_BIG;
		y += MARIO_BIG_TRANSFORMATION_OFFSET;
	}
	else
	{
		life = MARIO_LEVEL_SMALL;
	}
}

