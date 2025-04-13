#pragma once

#include "PiranhaPlant.h"
#include "Fireball.h"

#define VENUS_AMMO_LIMIT 1

#define VENUS_BBOX_HEIGHT 32.0f
#define VENUS_BBOX_WIDTH 16.0f

#define VENUS_LOOK_MAX_DISTANCE 145.0f
#define VENUS_SHOOT_MIN_DISTANCE 115.0f
#define VENUS_SHOOT_POSITION_OFFSET VENUS_BBOX_HEIGHT / 4

#define ANI_ID_VENUS 6000
//Actions
#define ANI_ID_VENUS_LOOK 0
#define ANI_ID_VENUS_SHOOT 100
//Vertical direction
#define ANI_ID_VENUS_DOWN 0	
#define ANI_ID_VENUS_UP 10	
//Horizontal direction
#define ANI_ID_VENUS_LEFT 0	
#define ANI_ID_VENUS_RIGHT 1

class CVenusFireTrap: public CPiranhaPlant
{
	//Shoot sensor	
	int mag_size;
	CFireball* boomerang;
	bool assault_mode;
public:
	CVenusFireTrap(float x, float y);

	virtual void ChangeAnimation();

	virtual void InPhaseAttack();

	virtual void ToStateHide();

	//Shoot methods
	virtual void LookforMario();
	virtual void Aim();
	virtual void Shoot();
	virtual void Reload();
};