#pragma once

#include "MovableObject.h"
#include "Creature.h"

class CHelpfulObject : public CMovableObject
{
protected:
	int effect;
public:
	CHelpfulObject(float x, float y);
	virtual void LaunchEffect(CCreature* creature);
};