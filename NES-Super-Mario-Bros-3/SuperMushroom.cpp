#include "PlayScene.h"

#include "SuperMushroom.h"
#include "Platform.h"
#include "Mario.h"

CSuperMushroom::CSuperMushroom(float x, float y):
	CHelpfulObject(x, y)
{
	SetBoundingBox(MUSHROOM_BBOX_WIDTH, MUSHROOM_BBOX_HEIGHT);	
	
	aniID = ANI_ID_MUSHROOM;	
}

void CSuperMushroom::Prepare(DWORD dt)
{
	switch (state)
	{
	case MUSHROOM_STATE_WAKEUP:
		WakeUp(dt);
		break;
	case MUSHROOM_STATE_EMERGE:
		if (abs(y - start_y) > bbox_height) SetState(MUSHROOM_STATE_RUN);
		break;
	case MUSHROOM_STATE_RUN:
		CMovableObject::Prepare(dt);
		break;
	}
}

void CSuperMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Move(dt);
}

void CSuperMushroom::SetState(int state)
{
	if (this->state == state)
	{
		return;
	}

	this->state = state;

	switch (state)
	{
		case MUSHROOM_STATE_SLEEP:
			ToStateSleeping();
			break;
		case MUSHROOM_STATE_WAKEUP:
			ToStateWakingUp();
			break;
		case MUSHROOM_STATE_EMERGE:
			ToStateEmerging();
			break;
		case MUSHROOM_STATE_RUN:
			ToStateRunning();
			break;
		case STATE_DIE:
			Delete();
			break;
	}
}

void CSuperMushroom::ToStateSleeping()
{
	vx = 0.0f;
	vy = 0.0f;
}

void CSuperMushroom::ToStateWakingUp()
{
	wakeup_time = 0;
}

void CSuperMushroom::ToStateEmerging()
{
	vy = MUSHROOM_VY;
	start_y = y;
}

void CSuperMushroom::ToStateRunning()
{
	vx = nx * MUSHROOM_VX;
	vy = 0.0f;
}

void CSuperMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CPlatform*>(e->obj))
	{
		OnCollisionWithPlatform(e);
	}
	else if (dynamic_cast<CBlock*>(e->obj))
	{
		OnCollisionWithBlock(e);
	}
	else if (dynamic_cast<CMario*>(e->obj))
	{
		OnCollisionWithMario(e);
	}
}

void CSuperMushroom::OnCollisionWithPlatform(LPCOLLISIONEVENT e)
{
	if (e->ny)
	{
		vy = 0;
		isOnGround = true;
	}
	if (e->nx)
	{
		vx = -vx;
	}
}

void CSuperMushroom::OnCollisionWithBlock(LPCOLLISIONEVENT e)
{
	if (e->ny)
	{
		vy = 0;
		isOnGround = true;
	}
	if (e->nx)
	{
		vx = -vx;
	}
}

void CSuperMushroom::OnCollisionWithMario(LPCOLLISIONEVENT e)
{
	if (CMario* mario = dynamic_cast<CMario*>(e->obj))
	{
		mario->InsertScoreObject(x, y - 16, 1000);
		CHelpfulObject::LaunchEffect(e);
		SetState(STATE_DIE);
	}
}

void CSuperMushroom::OnReactionTo(LPCOLLISIONEVENT e, int action)
{
	switch (state)
	{		
		case MUSHROOM_STATE_SLEEP:
			ReactionInSleepingState(e, action);
			break;
		case MUSHROOM_STATE_EMERGE:
		case MUSHROOM_STATE_RUN:
			ReactionInRunningState(e, action);
			break;
	}
}

void CSuperMushroom::ReactionInSleepingState(LPCOLLISIONEVENT e, int action)
{	
	nx = (x <= e->src_obj->GetX()) ? DIRECTION_LEFT : DIRECTION_RIGHT;
	SetState(MUSHROOM_STATE_WAKEUP);
}

void CSuperMushroom::ReactionInRunningState(LPCOLLISIONEVENT e, int action)
{
	if (CMario* mario = dynamic_cast<CMario*>(e->src_obj))
	{
		e->Reverse();
		CHelpfulObject::LaunchEffect(e);
		SetState(STATE_DIE);
	}
}

void CSuperMushroom::Render()
{
	if(state > MUSHROOM_STATE_WAKEUP)
		CAnimations::GetInstance()->Get(aniID)->Render(x, y);
	//RenderBoundingBox();
}

void CSuperMushroom::WakeUp(DWORD dt)
{
	wakeup_time += dt;
	if (wakeup_time >= MUSHROOM_WAKEUP_TIME)
	{
		SetState(MUSHROOM_STATE_EMERGE);
	}
}

void CSuperMushroom::Refresh()
{
	CHelpfulObject::Refresh();

	effect = EFFECT_BIGGER;
	SetState(MUSHROOM_STATE_SLEEP);
}
