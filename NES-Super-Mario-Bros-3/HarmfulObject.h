#pragma once

#include "MovableObject.h"

class CHarmfulObject : public CMovableObject
{
protected:
	bool isControl;
public:
	CHarmfulObject(float x, float y);
	virtual void MeleeAttack(CHarmfulObject* another) = 0;
	virtual bool IsControlled();
	virtual void SetControl();
	virtual void Drop();
};
