#pragma once

#include "PiranhaPlant.h"
#include "Fireball.h"

#define VENUS_AMMO_LIMIT 1

#define VENUS_LOOK_MAX_DISTANCE 145.0f
#define VENUS_SHOOT_MIN_DISTANCE 115.0f
#define VENUS_SHOOT_POSITION_OFFSET PIRANHA_BBOX_HEIGHT / 4.0f

#define ANI_ID_RED_VENUS 25000
#define ANI_ID_GREEN_VENUS 29000
//Actions
#define ANI_ID_VENUS_LOOK 0
#define ANI_ID_VENUS_SHOOT 100

class CVenusFireTrap: public CPiranhaPlant
{
	//Shoot sensor	
	int mag_size;
	CFireball* boomerang;
	bool assault_mode;
public:
	CVenusFireTrap(float x, float y, int type = PIRANHA_TYPE_GREEN);

	virtual int GetObjectAniID();
	virtual void ChangeAnimation();

	virtual void Attacking(DWORD dt);

	virtual void ToStateHide();

	virtual void Refresh();

	//Shoot methods
	virtual void LookForMario();
	virtual void Aim();
	virtual void Shoot();
	virtual void Reload();
	virtual void Unload();

	virtual CFireball* GetFireball() { return boomerang; }

	void CreateItem(CPlayScene* ps);
};