#include "PlayScene.h"

#include "SuperMushroom.h"
#include "Platform.h"
#include "Mario.h"

CSuperMushroom::CSuperMushroom(float x, float y):
	CHelpfulObject(x, y)
{
	bbox_height = MUSHROOM_BBOX_HEIGHT;
	bbox_width = MUSHROOM_BBOX_WIDTH;

	effect = EFFECT_BIGGER;
	
	aniID = ANI_ID_MUSHROOM;

	SetState(MUSHROOM_STATE_SLEEP);
}

void CSuperMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	dt = 16;
	InPhase(dt, coObjects);
}

void CSuperMushroom::InPhase(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	switch (state)
	{
		case MUSHROOM_STATE_SLEEP:
			break;
		case MUSHROOM_STATE_EMERGE:
			if (abs(y - start_y) < bbox_height) //emerging
			{
				Move(dt);
			}
			else //prepare to run
			{
				SetState(MUSHROOM_STATE_RUN);
			}
			break;
		case MUSHROOM_STATE_RUN:
			Accelerate(0.0f, GAME_GRAVITY, dt);
			CCollision::GetInstance()->Process(this, dt, coObjects);
			Move(dt);
			break;
	}
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

void CSuperMushroom::Reaction(CGameObject* by_another, int action)
{
	LookAwayFromMario();

	switch (state)
	{		
		case MUSHROOM_STATE_SLEEP:
			Reaction_SleepingState(by_another, action);
			break;
		case MUSHROOM_STATE_RUN:
			Reaction_RunningState(by_another, action);
			break;
	}
}

void CSuperMushroom::Reaction_SleepingState(CGameObject* by_another, int action)
{
	switch (action)
	{
		case ACTION_TOUCH:
			SetState(MUSHROOM_STATE_EMERGE);
			break;
	}
}

void CSuperMushroom::Reaction_RunningState(CGameObject* by_another, int action)
{
	if (CMario* mario = dynamic_cast<CMario*>(by_another))
	{
		CHelpfulObject::LaunchEffect(mario);
		SetState(STATE_DIE);
	}
}

void CSuperMushroom::Render()
{
	CAnimations::GetInstance()->Get(aniID)->Render(x, y);
	RenderBoundingBox();
}

void CSuperMushroom::LookAwayFromMario()
{
	//Get target
	LPPLAYSCENE playScene = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
	CMario* mario = (CMario*)playScene->GetPlayer();

	//Determine target position
	float mario_x, mario_y;
	mario->GetPosition(mario_x, mario_y);

	//Look away
	nx = (mario_x <= x) ? DIRECTION_LEFT : DIRECTION_RIGHT;
	nx = -nx;
}