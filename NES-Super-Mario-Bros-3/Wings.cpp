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

	if (wing_number == 1)
	{
		int action = ANI_ID_WINGS_FREE;
		int direction = nx <= 0 ? ID_ANI_DIRECTION_LEFT : ID_ANI_DIRECTION_RIGHT;
		left_wing_aniID = object + action + direction;
	}
	else
	{
		int action = ANI_ID_WINGS_CLOSE;
		if (state == WINGS_STATE_OPEN)
		{
			action = ANI_ID_WINGS_OPEN;
		}

		left_wing_aniID = object + action + ID_ANI_DIRECTION_LEFT;
		right_wing_aniID = object + action + ID_ANI_DIRECTION_RIGHT;
	}
}

void CWing::Render()
{
	ChangeAnimation();

	if (wing_number == 1)
	{
		float left_x = x + nx * distance_between_wings / 2;

		CAnimations::GetInstance()->Get(left_wing_aniID)->Render(left_x, y);
	}
	else
	{
		float left_x = x - distance_between_wings / 2;
		float right_x = x + distance_between_wings / 2;

		CAnimations::GetInstance()->Get(left_wing_aniID)->Render(left_x, y);
		CAnimations::GetInstance()->Get(right_wing_aniID)->Render(right_x, y);
	}
}
