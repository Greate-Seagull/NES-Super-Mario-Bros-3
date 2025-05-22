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
	float ax, ay; //accelerator
	int nx, ny, nz; //Direction

	bool isOnGround;
public:
	CMovableObject(float x, float y);

	//virtual int IsBlocking() { return vx == 0 && vy == 0; }

	virtual void Prepare(DWORD dt);

	virtual void Move(DWORD t);
	virtual void Accelerate(float ax, float ay, DWORD t);

	virtual void SetSpeed(float vx, float vy) { this->vx = vx; this->vy = vy; }
	virtual void GetSpeed(float& vx, float& vy);

	virtual void SetNx(float nx) { this->nx = nx; }
	virtual void SetNy(float ny) { this->ny = ny; }
	virtual int GetNx() { return nx; }
	virtual int GetNy() { return ny; }
	virtual int GetNz() { return nz; }

	virtual void ChangeDirection();
	virtual void Stop();
};
