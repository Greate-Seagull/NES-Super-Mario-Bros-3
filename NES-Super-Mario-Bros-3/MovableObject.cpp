#include "MovableObject.h"

CMovableObject::CMovableObject(float x, float y):
	CGameObject(x, y)
{	
}

void CMovableObject::Prepare(DWORD dt)
{
	Accelerate(ax, ay, dt);
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

void CMovableObject::GetSpeed(float& vx, float& vy)
{
	vx = this->vx;
	vy = this->vy;
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
}

void CMovableObject::Refresh()
{
	this->vx = 0.0f;
	this->vy = 0.0f;

	this->ax = 0.0f;
	this->ay = GAME_GRAVITY;

	this->nx = 0;
	this->ny = 0;
	this->nz = 0;

	this->isOnGround = false;
}