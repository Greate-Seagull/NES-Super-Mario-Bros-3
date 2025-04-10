#pragma once

#include "HarmfulObject.h"

class CCreature : public CHarmfulObject
{
protected:
	float life;
public:
	CCreature(float x, float y, float life);
	virtual void ProcessLife();
	virtual void UnderAttack(CHarmfulObject* by_another);
	virtual void MeleeAttack(CHarmfulObject* another);
	virtual string ToString();
};