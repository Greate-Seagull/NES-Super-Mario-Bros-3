#include "Game.h"
#include "PlayScene.h"

#include "PiranhaPlant.h"
#include "Mario.h"
#include "debug.h"

CPiranhaPlant::CPiranhaPlant(float x, float y):
	CCreature(x, y)
{
	bbox_height = PIRANHA_BBOX_HEIGHT;
	bbox_width = PIRANHA_BBOX_WIDTH;

	vx = PIRANHA_VX;
	vy = PIRANHA_VY;

	SetState(PIRANHA_STATE_EMERGE);
	start_y = y;
	life = PIRANHA_LIFE;
	mag_size = PIRANHA_MAG_SIZE;
	assault_mode = false;
}

void CPiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	ProcessLife();

	dt = 16;
	ChangeState();	
	LookforMario();
	Move(dt);

	if(boomerang)
		boomerang->Update(dt, coObjects);
	//DebugOutTitle(L"mag: %d", mag_size);
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
		{
			DWORD tick = GetTickCount64();
			if (tick - start_action_time >= PIRANHA_SHOOT_TIME)
			{
				SetState((this->state + 1) % PIRANHA_NUMBER_STATE);
			}
			else if (tick - start_action_time >= PIRANHA_SHOOT_TIME / 2)
			{
				/*CPlayScene* playScene = (CPlayScene*) CGame::GetInstance()->GetCurrentScene();
				playScene->Add(Shoot());*/
				Shoot();
			}
			break;
		}
		case PIRANHA_STATE_DIG:
			if (abs(y - start_y) >= bbox_height)
			{
				SetState((this->state + 1) % PIRANHA_NUMBER_STATE);
			}
			break;
		case PIRANHA_STATE_RELOAD:
			Reload();
			if (abs(nx) <= PIRANHA_DISTANCE_HIDE)
			{
				//just hide
			}
			else if (GetTickCount64() - start_action_time >= PIRANHA_RELOAD_TIME)
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
			start_action_time = GetTickCount64();
			vy = 0.0f;
			break;
		case PIRANHA_STATE_DIG:
			start_y = y;
			vy = PIRANHA_VY;
			break;
		case PIRANHA_STATE_RELOAD:
			start_action_time = GetTickCount64();
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
	if(boomerang)
		boomerang->Render();

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

void CPiranhaPlant::Reaction(CHarmfulObject* by_another, int action)
{
	AgainstControl();
}

void CPiranhaPlant::Shoot()
{
	if (mag_size)
	{
		if (boomerang == nullptr)
			boomerang = new CFireball(x, y);

		if (abs(nx) <= PIRANHA_SHOOT_MIN_DISTANCE)
		{
			mag_size -= 1;
			assault_mode = true;

			float shoot_nx = (nx <= 0) ? DIRECTION_LEFT : DIRECTION_RIGHT;
			float shoot_ny = (ny <= 0) ? DIRECTION_TOP : DIRECTION_DOWN;

			float shoot_x = x;
			float shoot_y = y - bbox_height / 4;

			boomerang->SetPosition(shoot_x, shoot_y);
			boomerang->ApplyRange(FIREBALL_RANGE_SHORT);
			boomerang->ApplyDirection(shoot_nx, shoot_ny);

			//return new CFireball(x, y, FIREBALL_RANGE_SHORT, shoot_nx, shoot_ny);
		}
		else if (assault_mode && abs(nx) <= PIRANHA_LOOK_MAX_DISTANCE)
		{
			mag_size -= 1;

			float shoot_nx = (nx <= 0) ? DIRECTION_LEFT : DIRECTION_RIGHT;
			float shoot_ny = (ny <= 0) ? DIRECTION_TOP : DIRECTION_DOWN;

			float shoot_x = x;
			float shoot_y = y - bbox_height / 4;

			boomerang->SetPosition(shoot_x, shoot_y);
			boomerang->ApplyRange(FIREBALL_RANGE_SHORT);
			boomerang->ApplyDirection(shoot_nx, shoot_ny);
			//return new CFireball(x, y, FIREBALL_RANGE_SHORT, shoot_nx, shoot_ny);
		}
		else
		{
			assault_mode = false;
		}
	}
	//return nullptr;
}

void CPiranhaPlant::Reload()
{
	if (mag_size < PIRANHA_MAG_SIZE)
	{
		mag_size++;
	}
}

void CPiranhaPlant::OnCollisionWith(LPCOLLISIONEVENT e)
{
	
}