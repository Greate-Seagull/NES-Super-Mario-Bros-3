#include "MovableObject.h"

CMovableObject::CMovableObject(float x, float y):
	CGameObject(x, y)
{
	this->vx = 0.0f;
	this->vy = 0.0f;
	this->nx = 0;
	this->ny = 0;
	this->isOnGround = false;
}

void CMovableObject::Move(DWORD t)
{
	x += vx * t;
	y += vy * t;
}

void CMovableObject::Accelerate(float ax, float ay, DWORD t)
{
	vx += ax * t;
	vy += ay * t;
}

void CMovableObject::ChangeDirection()
{
	vx = -vx;
	nx = -nx;

	vy = -vy;
	ny = -ny;
}

void CMovableObject::Stop()
{
	vx = STOP_V;
	vy = STOP_V;

	nx = DIRECTION_REMAIN;
	ny = DIRECTION_REMAIN;
}
