#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_GENERAL_MAP_ICON -15000

class CMapIcon : public CGameObject {
protected:
	int icon_Type;
public:
	CMapIcon(float x, float y, int icon_type) : CGameObject(x, y) 
	{
		this->icon_Type = icon_type;

		bbox_width = 0.0f;
		bbox_height = 0.0f;
		
		aniID = ID_ANI_GENERAL_MAP_ICON - icon_Type;
	}
};