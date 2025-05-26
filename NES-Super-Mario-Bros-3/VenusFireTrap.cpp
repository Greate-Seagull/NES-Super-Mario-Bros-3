#include "Game.h"
#include "PlayScene.h"
#include "Game.h"

#include "VenusFireTrap.h"
#include "Mario.h"
#include "debug.h"

CVenusFireTrap::CVenusFireTrap(float x, float y) :
	CPiranhaPlant(x, y)
{
	mag_size = VENUS_AMMO_LIMIT;
	assault_mode = false;
	boomerang = nullptr;
}

void CVenusFireTrap::ChangeAnimation()
{
	int object = ANI_ID_VENUS;

	int action;
	switch (state)
	{
	case PIRANHA_STATE_ATTACK:
		action = ANI_ID_VENUS_SHOOT;
		break;
	case STATE_DIE:
		action = ANI_ID_PIRANHA_DIE;
		break;
	default:
		action = ANI_ID_VENUS_LOOK;
		break;
	}
	
	int horizontal = 0;
	int vertical = 0;
	if (state != STATE_DIE)
	{
		horizontal = (nx < 0) ? ID_ANI_DIRECTION_LEFT : ID_ANI_DIRECTION_RIGHT;

		vertical = (ny < 0) ? ID_ANI_DIRECTION_UP : ID_ANI_DIRECTION_DOWN;
	}

	aniID = object + action + vertical + horizontal;
}

void CVenusFireTrap::Attacking(DWORD dt)
{
	start_action_time += dt;
	if (start_action_time >= PIRANHA_ATTACK_TIME)
	{
		SetState((this->state + 1) % PIRANHA_NUMBER_STATE);
	}
	else if (start_action_time >= PIRANHA_ATTACK_TIME / 2)
	{
		Shoot();
	}
}

void CVenusFireTrap::ToStateHide()
{
	CPiranhaPlant::ToStateHide();
	Reload();
}

void CVenusFireTrap::Shoot()
{
	if (mag_size)
	{
		if (boomerang == nullptr)
		{
			boomerang = new CFireball(x, y);
			CPlayScene* playScene = (CPlayScene*)(CGame::GetInstance()->GetCurrentScene());
			playScene->Insert(boomerang, -1);
		}

		if (abs(target_dx) <= VENUS_SHOOT_MIN_DISTANCE)
		{
			mag_size -= 1;
			assault_mode = true;
			Aim();
		}
		else if (assault_mode && abs(target_dx) <= VENUS_LOOK_MAX_DISTANCE)
		{
			mag_size -= 1;
			Aim();
		}
		else
		{
			assault_mode = false;
			Unload();
		}
	}
}

void CVenusFireTrap::LookForMario()
{
	CPiranhaPlant::LookForMario();
	//look towards object
	nx = (target_dx <= 0) ? DIRECTION_LEFT : DIRECTION_RIGHT;
	ny = (target_dy <= 0) ? DIRECTION_UP : DIRECTION_DOWN;
}

void CVenusFireTrap::Aim()
{
	int shoot_nx = nx;
	int shoot_ny = ny;

	float shoot_x = x;
	float shoot_y = y - VENUS_SHOOT_POSITION_OFFSET;

	float shoot_range_min = fabs(target_dy) + VENUS_SHOOT_POSITION_OFFSET; //short range => dx = dy => dx = dy = target_dx + PIRANHA_SHOOT_POSITION_OFFSET
	int shoot_range_type = (fabs(target_dx) <= shoot_range_min) ? FIREBALL_RANGE_SHORT : FIREBALL_RANGE_LONG;

	boomerang->SetPosition(shoot_x, shoot_y);
	boomerang->ApplyRange(shoot_range_type);
	boomerang->ApplyDirection(shoot_nx, shoot_ny);
}

void CVenusFireTrap::Reload()
{
	if (mag_size < VENUS_AMMO_LIMIT)
	{
		mag_size++;
	}
}

void CVenusFireTrap::Unload()
{
	boomerang->ApplyRange(FIREBALL_RANGE_DISARM);
}
