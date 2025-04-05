#pragma once

#include "MovableObject.h"

class CHarmfulObject : public CMovableObject
{
protected:
	bool isSharp;
public:
	CHarmfulObject(float x, float y,
		float vx, float vy,
		float ax, float ay,
		bool isSharp);
	virtual void Attack();
};
