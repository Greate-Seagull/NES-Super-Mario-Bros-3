#include "FloatingPlatform.h"
#include "Mario.h"

void CFloatingPlatform::GetSpeed(float& vx, float& vy)
{
	vx = this->vx;
	vy = this->vy;
}

void CFloatingPlatform::Prepare(DWORD dt)
{
	if (isFloating == false)
		vy += dt * FALLING_GRAVITY;
}

void CFloatingPlatform::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//if (isFloating)
	//	x += dt * vx;
	//else
	//	y += dt * vy;

	if (isFloating)
		x += dt * vx;
	else
	{		
		y += dt * vy;
	}	
}

void CFloatingPlatform::OnReactionTo(LPCOLLISIONEVENT e, int action)
{
	if(e->ny < 0 && dynamic_cast<CMario*>(e->src_obj))
		isFloating = false;
}
