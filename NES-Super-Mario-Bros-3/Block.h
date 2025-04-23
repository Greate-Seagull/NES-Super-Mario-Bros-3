#pragma once

#include "MovableObject.h"

class CBlock : public CMovableObject
{
public:
	CBlock(float x, float y) : CMovableObject(x, y) {}
	virtual int IsCollidable() { return 1; }
	//GameObject co ham update roi! "virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};"
	//GameObject co ham render roi! "virtual void Render();"
};