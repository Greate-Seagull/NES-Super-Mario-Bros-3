#include "MovableObject.h"

CMovableObject::CMovableObject(float x, float y):
	CGameObject(x, y)
{
	this->vx = 0.0f;
	this->vy = 0.0f;
	this->nx = 0;
}

void CMovableObject::Move(DWORD t)
{
	x += vx * t;
	y += vy * t;
}

void CMovableObject::Accelerate(DWORD t, float ax, float ay)
{
	vx += ax * t;
	vy += ay * t;
}
