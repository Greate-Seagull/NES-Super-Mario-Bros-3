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

		aniID = ID_ANI_GENERAL_MAP_ICON - icon_Type; //Thêm dòng này để dùng CGameObject::Render()
	}
	void Render(); //GameObject có Render rồi! "virtual void Render();"
	void Update(DWORD dt) {} //GameObject co ham update roi! "virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};"
	void GetBoundingBox(float& l, float& t, float& r, float& b); 
	int IsBlocking() { return 0; }
};