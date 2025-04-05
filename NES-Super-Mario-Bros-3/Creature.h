#pragma once

#include "HarmfulObject.h"

class CCreature : public CHarmfulObject
{
protected:
	float life;
public:
	CCreature(float x, float y,
		float vx, float vy,
		float ax, float ay,
		bool isSharp, float life);
};