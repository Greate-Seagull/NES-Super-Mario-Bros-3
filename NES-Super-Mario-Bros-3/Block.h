#pragma once

#include "GameObject.h"

class CBlock : public CGameObject
{
public:
	CBlock(float x, float y) : CGameObject(x, y) {}
	virtual void Render() = 0;
	void Update(DWORD dt) {}
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) = 0;
};