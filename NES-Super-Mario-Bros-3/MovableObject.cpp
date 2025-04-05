#include "MovableObject.h"

CMovableObject::CMovableObject(float x, float y):
	CGameObject(x, y)
{
	this->vx = 0.0f;
	this->vy = 0.0f;
	this->nx = 0;
}

void CMovableObject::Move(DWORD t, float ax, float ay)
{
	x += vx * t + ax / 2 * t * t;
	vx += ax * t;

	y += vy * t + ay / 2 * t * t;
	vy += ay * t;
}
