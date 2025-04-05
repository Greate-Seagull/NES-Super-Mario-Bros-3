#pragma once

#include "MovableObject.h"

class CHarmfulObject : public CMovableObject
{
protected:
	bool isSharp;
public:
	CHarmfulObject(float x, float y, bool isSharp);
	virtual void Attack();
};
