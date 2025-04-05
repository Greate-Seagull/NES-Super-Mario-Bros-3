#pragma once

#include "GameObject.h"

class CMovableObject : public CGameObject
{
protected:
	float vx, vy; //velocity
	float ax, ay; //acceleration
	int nx; //Direction

public:
	CMovableObject(float x, float y,
		float vx, float vy,
		float ax, float ay);
	
	virtual void Move(DWORD t);
	virtual void SetSpeed(float vx, float vt) { this->vx = vx; this->vy = vy; }
	virtual void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }
};
