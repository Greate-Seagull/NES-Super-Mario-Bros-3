#pragma once

#include "HarmfulObject.h"

class CCreature : public CHarmfulObject
{
protected:
	float life;
public:
	CCreature(float x, float y, bool isSharp, float life);
	virtual void ProcessLife();
	virtual void UnderAttack();
	virtual string ToString();
};