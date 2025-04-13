#include "MapIcon.h"

void CMapIcon::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_GENERAL_MAP_ICON - icon_Type)->Render(x, y);
}

void CMapIcon::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = 0;
	t = 0;
	r = 0;
	b = 0;
}