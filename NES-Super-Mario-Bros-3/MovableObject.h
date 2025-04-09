#pragma once

#include "GameObject.h"

#define DIRECTION_LEFT -1
#define DIRECTION_RIGHT 1
#define DIRECTION_REMAIN 0

class CMovableObject : public CGameObject
{
protected:
	float vx, vy; //velocity	
	int nx; //Direction

public:
	CMovableObject(float x, float y);
	
	virtual void Move(DWORD t);
	virtual void Accelerate(float ax, float ay, DWORD t);
	virtual void SetSpeed(float vx, float vt) { this->vx = vx; this->vy = vy; }
	virtual void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }
};
