#pragma once

#include "HelpfulObject.h"

#define WINGS_STATE_OPEN 10
#define WINGS_STATE_CLOSE 11

//Animations
//Object
#define ANI_ID_WINGS 60000
//Actions
#define ANI_ID_WINGS_OPEN 0
#define ANI_ID_WINGS_CLOSE 10
//Direction
#define ANI_ID_WINGS_LEFT 0
#define ANI_ID_WINGS_RIGHT 1

class CWing : public CHelpfulObject
{
	int wing_number;
	float distance_between_wings;

	int left_wing_aniID;
	int right_wing_aniID;
public:
	CWing(float x, float y, int wing_number = 1, float distance_between_wings = 0.0f);
	
	void ChangeAnimation();
	void Render();
};
