#include "Wings.h"

CWing::CWing(float x, float y, int wing_number, float distance_between_wings):
	CHelpfulObject(x, y)
{
	this->wing_number = wing_number;
	this->distance_between_wings = distance_between_wings;
}

void CWing::ChangeAnimation()
{
	int object = ANI_ID_WINGS;
	int action = ANI_ID_WINGS_CLOSE;
	if (state == WINGS_STATE_OPEN)
	{
		action = ANI_ID_WINGS_OPEN;
	}

	left_wing_aniID = object + action + ANI_ID_WINGS_LEFT;
	right_wing_aniID = object + action + ANI_ID_WINGS_RIGHT;
}

void CWing::Render()
{
	ChangeAnimation();
	float left_x = x - distance_between_wings / 2;
	float right_x = x + distance_between_wings / 2;

	CAnimations::GetInstance()->Get(left_wing_aniID)->Render(left_x, y);
	CAnimations::GetInstance()->Get(right_wing_aniID)->Render(right_x, y);
}
