#include "Game.h"
#include "PlayScene.h"
#include "Game.h"

#include "VenusFireTrap.h"
#include "Mario.h"
#include "debug.h"

CVenusFireTrap::CVenusFireTrap(float x, float y) :
	CPiranhaPlant(x, y)
{
	bbox_height = VENUS_BBOX_HEIGHT;
	bbox_width = VENUS_BBOX_WIDTH;

	mag_size = VENUS_AMMO_LIMIT;
	assault_mode = false;
}

void CVenusFireTrap::ChangeAnimation()
{
	int object = ANI_ID_VENUS;

	int action = ANI_ID_VENUS_LOOK;
	if (state == PIRANHA_STATE_ATTACK)
	{
		action = ANI_ID_VENUS_SHOOT;
	}

	int horizontal = (nx < 0) ? ANI_ID_VENUS_LEFT : ANI_ID_VENUS_RIGHT;

	int vertical = (ny < 0) ? ANI_ID_VENUS_UP : ANI_ID_VENUS_DOWN;

	aniID = object + action + vertical + horizontal;
}

void CVenusFireTrap::InPhaseAttack()
{
	DWORD tick = GetTickCount64();
	if (tick - start_action_time >= PIRANHA_ATTACK_TIME)
	{
		SetState((this->state + 1) % PIRANHA_NUMBER_STATE);
	}
	else if (tick - start_action_time >= PIRANHA_ATTACK_TIME / 2)
	{
		Shoot();
	}
}

void CVenusFireTrap::ToStateHide()
{
	CPiranhaPlant::ToStateHide();
	Reload();
}

//----------------------------------------------------SHOOT----------------------------------------------------------
void CVenusFireTrap::Shoot()
{
	if (mag_size)
	{
		if (boomerang == nullptr)
		{
			boomerang = new CFireball(x, y);
			CPlayScene* playScene = (CPlayScene*)(CGame::GetInstance()->GetCurrentScene());
			playScene->Add(boomerang);
		}

		if (abs(target_dx) <= VENUS_SHOOT_MIN_DISTANCE)
		{
			mag_size -= 1;
			assault_mode = true;
			Aim();
			//return new CFireball(x, y, FIREBALL_RANGE_SHORT, shoot_nx, shoot_ny);
		}
		else if (assault_mode && abs(target_dx) <= VENUS_LOOK_MAX_DISTANCE)
		{
			mag_size -= 1;
			Aim();
			//return new CFireball(x, y, FIREBALL_RANGE_SHORT, shoot_nx, shoot_ny);
		}
		else
		{
			assault_mode = false;
		}
	}
	//return nullptr;
}

void CVenusFireTrap::LookforMario()
{
	CPiranhaPlant::LookforMario();
	//look towards object
	nx = (target_dx <= 0) ? DIRECTION_LEFT : DIRECTION_RIGHT;
	ny = (target_dy <= 0) ? DIRECTION_TOP : DIRECTION_DOWN;
}

void CVenusFireTrap::Aim()
{
	float shoot_nx = nx;
	float shoot_ny = ny;

	float shoot_x = x;
	float shoot_y = y - VENUS_SHOOT_POSITION_OFFSET;

	float shoot_range_min = abs(target_dy) + VENUS_SHOOT_POSITION_OFFSET; //short range => dx = dy => dx = dy = target_dx + PIRANHA_SHOOT_POSITION_OFFSET
	float shoot_range = (abs(target_dx) <= shoot_range_min) ? FIREBALL_RANGE_SHORT : FIREBALL_RANGE_LONG;

	boomerang->SetPosition(shoot_x, shoot_y);
	boomerang->ApplyRange(shoot_range);
	boomerang->ApplyDirection(shoot_nx, shoot_ny);
}

void CVenusFireTrap::Reload()
{
	if (mag_size < VENUS_AMMO_LIMIT)
	{
		mag_size++;
	}
}