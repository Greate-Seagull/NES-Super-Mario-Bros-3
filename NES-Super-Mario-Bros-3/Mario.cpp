#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"

#include "Collision.h"

CMario::CMario(float x, float y):
	CCreature(x, y, MARIO_SHARP, MARIO_LEVEL_SMALL)
{
	isSitting = false;
	maxVx = 0.0f;

	decelerateTick = TICK_DECELERATE;
	freefallTick = TICK_FREEFALL;

	untouchable = 0;
	untouchable_start = -1;
	isOnPlatform = false;
	coin = 0;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	PrepareState();
	DetermineState();	
	float ax, ay;	
	ApplyState(ax, ay);
	DetermineAccelerator(ax, ay, dt);
	Move(dt, ax, ay);

	DebugOutTitle(L"vx: %f", vx);

	//if (abs(vx) > abs(maxVx)) vx = maxVx;

	//// reset untouchable timer if untouchable time has passed
	//if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	//{
	//	untouchable_start = 0;
	//	untouchable = 0;
	//}

	//CCollision::GetInstance()->Process(this, dt, coObjects);
}

//void CMario::OnNoCollision(DWORD dt)
//{
//	x += vx * dt;
//	y += vy * dt;
//	isOnPlatform = false;
//}
//
//void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
//{
//	if (e->ny != 0 && e->obj->IsBlocking())
//	{
//		vy = 0;
//		if (e->ny < 0) isOnPlatform = true;
//	}
//	else 
//	if (e->nx != 0 && e->obj->IsBlocking())
//	{
//		vx = 0;
//	}
//
//	if (dynamic_cast<CGoomba*>(e->obj))
//		OnCollisionWithGoomba(e);
//	else if (dynamic_cast<CCoin*>(e->obj))
//		OnCollisionWithCoin(e);
//	else if (dynamic_cast<CPortal*>(e->obj))
//		OnCollisionWithPortal(e);
//}

//void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
//{
//	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
//
//	// jump on top >> kill Goomba and deflect a bit 
//	if (e->ny < 0)
//	{
//		if (goomba->GetState() != GOOMBA_STATE_DIE)
//		{
//			goomba->SetState(GOOMBA_STATE_DIE);
//			vy = -MARIO_JUMP_DEFLECT_SPEED;
//		}
//	}
//	else // hit by Goomba
//	{
//		if (untouchable == 0)
//		{
//			if (goomba->GetState() != GOOMBA_STATE_DIE)
//			{
//				if (life > MARIO_LEVEL_SMALL)
//				{
//					life = MARIO_LEVEL_SMALL;
//					StartUntouchable();
//				}
//				else
//				{
//					DebugOut(L">>> Mario DIE >>> \n");
//					SetState(MARIO_STATE_DIE);
//				}
//			}
//		}
//	}
//}

//void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
//{
//	e->obj->Delete();
//	coin++;
//}
//
//void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
//{
//	CPortal* p = (CPortal*)e->obj;
//	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
//}

//
// Get animation ID for small Mario
//
//int CMario::GetAniIdSmall()
//{
//	int aniId = -1;
//	if (!isOnPlatform)
//	{
//		if (vy < 0)
//		{
//			if (nx > 0) aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
//			else aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
//		}
//		//else
//		//{
//		//	if (nx >= 0)
//		//		aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
//		//	else
//		//		aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
//		//}
//	}
//	else
//		if (isSitting)
//		{
//			if (nx > 0)
//				aniId = ID_ANI_MARIO_SIT_RIGHT;
//			else
//				aniId = ID_ANI_MARIO_SIT_LEFT;
//		}
//		else
//			if (vx == 0)
//			{
//				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
//				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
//			}
//			//else if (vx > 0)
//			//{
//			//	if (ax < 0)
//			//		aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
//			//	else if (ax == MARIO_ACCEL_RUN_X)
//			//		aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
//			//	else if (ax == MARIO_ACCEL_WALK_X)
//			//		aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
//			//}
//			//else // vx < 0
//			//{
//			//	if (ax > 0)
//			//		aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
//			//	else if (ax == -MARIO_ACCEL_RUN_X)
//			//		aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
//			//	else if (ax == -MARIO_ACCEL_WALK_X)
//			//		aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
//			//}
//
//	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
//
//	return aniId;
//}
//
//
////
//// Get animdation ID for big Mario
////
//int CMario::GetAniIdBig()
//{
//	int aniId = -1;
//	if (!isOnPlatform)
//	{
//		if (vy < 0)
//		{
//			if (nx >= 0)
//				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
//			else
//				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
//		}
//		/*if (abs(ax) == MARIO_ACCEL_RUN_X)
//		{
//			if (nx >= 0)
//				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
//			else
//				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
//		}
//		else
//		{
//			if (nx >= 0)
//				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
//			else
//				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
//		}*/
//	}
//	else
//		if (isSitting)
//		{
//			if (nx > 0)
//				aniId = ID_ANI_MARIO_SIT_RIGHT;
//			else
//				aniId = ID_ANI_MARIO_SIT_LEFT;
//		}
//		else
//			if (vx == 0)
//			{
//				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
//				else aniId = ID_ANI_MARIO_IDLE_LEFT;
//			}
//			//else if (vx > 0)
//			//{
//			//	if (ax < 0)
//			//		aniId = ID_ANI_MARIO_BRACE_RIGHT;
//			//	else if (ax == MARIO_ACCEL_RUN_X)
//			//		aniId = ID_ANI_MARIO_RUNNING_RIGHT;
//			//	else if (ax == MARIO_ACCEL_WALK_X)
//			//		aniId = ID_ANI_MARIO_WALKING_RIGHT;
//			//}
//			//else // vx < 0
//			//{
//			//	if (ax > 0)
//			//		aniId = ID_ANI_MARIO_BRACE_LEFT;
//			//	else if (ax == -MARIO_ACCEL_RUN_X)
//			//		aniId = ID_ANI_MARIO_RUNNING_LEFT;
//			//	else if (ax == -MARIO_ACCEL_WALK_X)
//			//		aniId = ID_ANI_MARIO_WALKING_LEFT;
//			//}
//
//	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;
//
//	return aniId;
//}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = ID_ANI_MARIO_SMALL_RUN_6_FRAMES_RIGHT;

	//ChangeAnimation(aniId);

	animations->Get(aniId)->Render(x, y);

	//RenderBoundingBox();
	
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

	//only standing can start sitting
	if (keyState->IsHold(VK_DOWN))
	{
		if (OnLandState == MARIO_STATE_STANDING && OnSkyState == MARIO_STATE_STANDING)
		{
			OnLandState = MARIO_STATE_SITTING;
		}
	}

	//walk anyway even though sit / stand / walk / run
	if (keyState->IsHold(VK_LEFT))
	{
		OnLandState = MARIO_STATE_WALKING;
	}
	if (keyState->IsHold(VK_RIGHT))
	{
		OnLandState = MARIO_STATE_WALKING;
	}

	//only walking can start running
	if (keyState->IsHold(VK_A))
	{
		if (OnLandState == MARIO_STATE_WALKING)
		{
			OnLandState = MARIO_STATE_RUNNING;
		}
	}

	if (keyState->IsHold(VK_S))
	{
		OnSkyState = MARIO_STATE_JUMPING;
	}
}

void CMario::ApplyState(float &ax, float &ay)
{
	switch (OnLandState)
	{
		case MARIO_STATE_WALKING:
			ax = MARIO_WALKING_AX;
			break;
		case MARIO_STATE_RUNNING:
			ax = MARIO_RUNNING_AX;
			break;
		default:
			ax = 0.0f;
	}

	switch (OnSkyState)
	{
		case MARIO_STATE_JUMPING:
			ay = MARIO_JUMPING_AY;
			break;
		default:
			ay = 0.0f;
	}
}

void CMario::DetermineAccelerator(float& applied_ax, float& applied_ay, DWORD& t)
{
	KeyStateManager* keyState = CGame::GetInstance()->GetKeyboard();

	float ax = 0.0f;
	float ay = 0.0f;
	int nx = 0;

	if (keyState->IsHold(VK_LEFT))
	{
		ax += -applied_ax;
		nx--;
	}
	if (keyState->IsHold(VK_RIGHT))
	{
		ax += applied_ax;
		nx++;
	}
	//Decelerate if not pressing any moving key
	if (ax == 0.0f)
	{
		if (decelerateTick)
		{
			ax = -vx / decelerateTick; // calculate decelerate ax		
			t = min(decelerateTick, t); // get exact time to stop
			decelerateTick -= t; // calculate remaining time to stop
		}
	}
	else
	{
		decelerateTick = TICK_DECELERATE;
	}

	if (keyState->IsHold(VK_S))
	{
		ay = applied_ay;
		freefallTick = TICK_FREEFALL;
	}
	else
	{
		if (freefallTick)
		{
			ay = -vy / freefallTick; //decelerate ay
			t = min(freefallTick, t); 
			freefallTick -= t;
		}
	}
	

	if (nx)
	{
		this->nx = nx;
	}

	applied_ax = ax;
	applied_ay = ay;
}

void CMario::ChangeAnimation(int& ani)
{
	switch (OnLandState)
	{
		default:
		{
			if (nx == DIRECTION_LEFT)
			{
				ani = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else
			{
				ani = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
			}
		}
	}

	switch (OnSkyState)
	{
		case MARIO_STATE_JUMPING:
		{
			if (nx == DIRECTION_LEFT)
			{
				ani = ID_ANI_MARIO_SMALL_JUMP_LEFT;
			}
			else
			{
				ani = ID_ANI_MARIO_SMALL_JUMP_RIGHT;
			}
		}
	}
}

void CMario::PrepareState()
{
	if (vx == 0 && vy == 0)
	{
		OnLandState = MARIO_STATE_STANDING;
		OnSkyState = MARIO_STATE_STANDING;
	}
}

//void CMario::SetLevel(int l)
//{
//	// Adjust position to avoid falling off platform
//	if (this->life == MARIO_LEVEL_SMALL)
//	{
//		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
//	}
//	life = l;
//}

