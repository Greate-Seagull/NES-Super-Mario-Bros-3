#include "PiranhaPlant.h"
#include "debug.h"

CPiranhaPlant::CPiranhaPlant(float x, float y):
	CCreature(x, y, PIRANHA_SHARP, PIRANHA_LIFE)
{
	origin_x = x;
	origin_y = y;

	vx = PIRANHA_VX;
	vy = PIRANHA_VY;
	SetState(PIRANHA_STATE_EMERGE);
}

void CPiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	DebugOutTitle(L"State: %d", state);
	dt = 16;
	DetermineState();
	Move(dt);
}

void CPiranhaPlant::DetermineState()
{	
	switch (state)
	{
		case PIRANHA_STATE_EMERGE:
			if (abs(y - origin_y) >= PIRANHA_BBOX_HEIGHT)
			{
				SetState((this->state + 1) % PIRANHA_NUMBER_STATE);
				start_behavior_time = GetTickCount64();
			}
			break;
		case PIRANHA_STATE_SHOOT:
			if (GetTickCount64() - start_behavior_time >= PIRANHA_SHOOT_TIME)
			{
				SetState((this->state + 1) % PIRANHA_NUMBER_STATE);
			}
			break;
		case PIRANHA_STATE_DIG:
			if (abs(y - origin_y) == 0)
			{
				SetState((this->state + 1) % PIRANHA_NUMBER_STATE);
				start_behavior_time = GetTickCount64();
			}
			break;
		case PIRANHA_STATE_RELOAD:
			if (GetTickCount64() - start_behavior_time >= PIRANHA_RELOAD_TIME)
			{
				SetState((this->state + 1) % PIRANHA_NUMBER_STATE);
			}
			break;
	}

	/*if (abs(y - origin_y) < PIRANHA_BBOX_HEIGHT && state == PIRANHA_STATE_RELOAD)
	{
		SetState(PIRANHA_STATE_EMERGE);
	}
	else if (GetTickCount64() - start_behavior_time < PIRANHA_SHOOT_TIME && state == PIRANHA_SHOOT_TIME)
	{
		SetState(PIRANHA_STATE_SHOOT);
	}
	else if (abs(y - origin_y) != 0 && state != PIRANHA_STATE_EMERGE)
	{
		SetState(PIRANHA_STATE_DIG);
	}
	else if (GetTickCount64() - start_behavior_time < PIRANHA_RELOAD_TIME && state != PIRANHA_RELOAD_TIME)
	{
		SetState(PIRANHA_STATE_RELOAD);
	}
	else
	{
		start_behavior_time = GetTickCount64();
	}*/
}

void CPiranhaPlant::SetState(int state)
{
	if (this->state == state)
	{
		return;
	}

	this->state = state;

	switch (state)
	{
		case PIRANHA_STATE_EMERGE:
			vy = -PIRANHA_VY;
			break;
		case PIRANHA_STATE_SHOOT:
			vy = 0.0f;
			break;
		case PIRANHA_STATE_DIG:
			vy = PIRANHA_VY;
			break;
		case PIRANHA_STATE_RELOAD:
			vy = 0.0f;
			break;
	}
}

void CPiranhaPlant::Render()
{
	/*if (state == PIRANHA_STATE_RELOAD) 
	{
		return;
	}*/

	CAnimations* animations = CAnimations::GetInstance();

	ChangeAnimation();

	animations->Get(aniID)->Render(x, y);

	RenderBoundingBox();
}

void CPiranhaPlant::ChangeAnimation()
{
	int object = ANI_ID_PIRANHA;

	int action = ANI_ID_PIRANHA_LOOK;
	if (state == PIRANHA_STATE_SHOOT)
	{
		action = ANI_ID_PIRANHA_SHOOT;
	}

	int vertical = ANI_ID_PIRANHA_DOWN;

	int horizontal = nx;

	aniID = object + action + vertical + horizontal;
}

void CPiranhaPlant::UnderAttack()
{	
}
