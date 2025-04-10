#pragma once

#include "MovableObject.h"

class CHarmfulObject : public CMovableObject
{
protected:
public:
	CHarmfulObject(float x, float y);
	virtual void MeleeAttack(CHarmfulObject* another) = 0;
};
