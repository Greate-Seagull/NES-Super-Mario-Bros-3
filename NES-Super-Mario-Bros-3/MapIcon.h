#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_GENERAL_MAP_ICON -15000

#define BUSH_BBOX_WIDTH 16
#define BUSH_BBOX_HEIGHT 16

class CMapIcon : public CGameObject {
protected:
	int icon_Type;
public:
	CMapIcon(float x, float y, int icon_type) : CGameObject(x, y) 
	{
		this->icon_Type = icon_type;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};