#pragma once

#include "GameObject.h"

class CBlock : public CGameObject
{
public:
	CBlock(float x, float y) : CGameObject(x, y) {}
	//GameObject co ham update roi! "virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};"
	//GameObject co ham render roi! "virtual void Render();"
};