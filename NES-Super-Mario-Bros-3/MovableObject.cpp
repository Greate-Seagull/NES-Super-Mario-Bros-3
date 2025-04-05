#include "MovableObject.h"

CMovableObject::CMovableObject(float x, float y, float vx, float vy, float ax, float ay):
	CGameObject(x, y)
{
	this->vx = vx;
	this->vy = vy;
	this->ax = ax;
	this->ay = ay;
	this->nx = 1;
}

void CMovableObject::Move(DWORD t)
{
	x += vx * t + ax / 2 * t * t;
	vx += ax * t;

	y += vy * t + ay / 2 * t * t;
	vy += ay * t;
}
