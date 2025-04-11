#include "Game.h"
#include "PlayScene.h"

#include "PiranhaPlant.h"
#include "Mario.h"
#include "debug.h"

CPiranhaPlant::CPiranhaPlant(float x, float y):
	CCreature(x, y, PIRANHA_LIFE)
{
	bbox_height = PIRANHA_BBOX_HEIGHT;
	bbox_width = PIRANHA_BBOX_WIDTH;

	vx = PIRANHA_VX;
	vy = PIRANHA_VY;

	SetState(PIRANHA_STATE_EMERGE);
	start_y = y;
}

void CPiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	ProcessLife();

	dt = 16;
	ChangeState();
	//CCollision::GetInstance()->Process(this, dt, coObjects);
	LookforMario();
	Move(dt);
}

void CPiranhaPlant::ChangeState()
{	
	switch (state)
	{
		case PIRANHA_STATE_EMERGE:
			if (abs(y - start_y) >= bbox_height)
			{
				SetState((this->state + 1) % PIRANHA_NUMBER_STATE);
			}
			break;
		case PIRANHA_STATE_SHOOT:
			if (GetTickCount64() - start_behavior_time >= PIRANHA_SHOOT_TIME)
			{
				SetState((this->state + 1) % PIRANHA_NUMBER_STATE);
			}
			break;
		case PIRANHA_STATE_DIG:
			if (abs(y - start_y) >= bbox_height)
			{
				SetState((this->state + 1) % PIRANHA_NUMBER_STATE);
			}
			break;
		case PIRANHA_STATE_RELOAD:
			if (abs(nx) <= PIRANHA_DISTANCE_HIDE)
			{
				//just hide
			}
			else if (GetTickCount64() - start_behavior_time >= PIRANHA_RELOAD_TIME)
			{
				SetState((this->state + 1) % PIRANHA_NUMBER_STATE);
			}
			break;
	}
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
			start_y = y;
			vy = -PIRANHA_VY;
			break;
		case PIRANHA_STATE_SHOOT:
			start_behavior_time = GetTickCount64();
			vy = 0.0f;
			break;
		case PIRANHA_STATE_DIG:
			start_y = y;
			vy = PIRANHA_VY;
			break;
		case PIRANHA_STATE_RELOAD:
			start_behavior_time = GetTickCount64();
			vy = 0.0f;
			break;
		case STATE_DIE:
			//CGameObject::SetState(state);
			this->Delete();
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

	int vertical = (ny < 0) ? ANI_ID_PIRANHA_UP : ANI_ID_PIRANHA_DOWN;

	int horizontal = (nx < 0) ? ANI_ID_PIRANHA_LEFT: ANI_ID_PIRANHA_RIGHT;

	aniID = object + action + vertical + horizontal;
}

void CPiranhaPlant::UnderAttack(CHarmfulObject* by_another)
{	
	this->MeleeAttack(by_another);
}

void CPiranhaPlant::LookforMario()
{
	LPPLAYSCENE playScene = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
	CMario* mario = (CMario*)playScene->GetPlayer();

	float mario_x, mario_y;
	mario->GetPosition(mario_x, mario_y);
	nx = mario_x - x;
	ny = mario_y - y;
}

void CPiranhaPlant::OnCollisionWith(LPCOLLISIONEVENT e)
{
	
}