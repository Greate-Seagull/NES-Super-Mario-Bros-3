#pragma once

#include "GameObject.h"

class CMovableObject : public CGameObject
{
protected:
	float vx, vy; //velocity	
	int nx; //Direction

public:
	CMovableObject(float x, float y);
	
	virtual void Move(DWORD t, float ax, float ay);
	virtual void SetSpeed(float vx, float vt) { this->vx = vx; this->vy = vy; }
	virtual void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }
};
