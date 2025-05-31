#pragma once

#include "Creature.h"

class CEnemy: public CCreature
{	
public:
	CEnemy(float x, float y);	
	virtual void LookForMario();
};
