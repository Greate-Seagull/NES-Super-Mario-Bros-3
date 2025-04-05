#pragma once

#include "Creature.h"

#define PIRANHA_VX 0.0f
#define PIRANHA_VY 0.0f
#define PIRANHA_AX 0.0f
#define PIRANHA_AY 0.0f
#define PIRANHA_SHARP true
#define PIRANHA_LIFE 1.0f

class CPiranhaPlant : public CCreature
{
public:
	CPiranhaPlant(float x, float y);
};