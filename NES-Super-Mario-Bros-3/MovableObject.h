#pragma once

#include "GameObject.h"

#define DIRECTION_LEFT -1
#define DIRECTION_RIGHT 1

#define DIRECTION_UP -1
#define DIRECTION_DOWN 1

#define DIRECTION_FRONT -1
#define DIRECTION_BEHIND 1

#define STOP_V 0.0f

class CMovableObject : public CGameObject
{
protected:
	float vx, vy; //velocity	
	int nx, ny, nz; //Direction

	bool isOnGround;

	bool is_moved;

public:
	CMovableObject(float x, float y);

	virtual void Prepare(DWORD dt);

	virtual int IsUpdated() { return is_moved; }
	
	virtual void Move(DWORD t);
	virtual void Accelerate(float ax, float ay, DWORD t);

	virtual void SetSpeed(float vx, float vt) { this->vx = vx; this->vy = vy; }
	virtual void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }

	virtual void ChangeDirection();
	virtual void Stop();

	int GetNx() { return nx; }
	int GetNy() { return ny; }
	int GetNz() { return nz; }
};
