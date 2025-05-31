#include "Boomerang.h"
#include "Mario.h"

CBoomerang::CBoomerang(float x, float y):
	CHarmfulObject(x, y)
{
	SetBoundingBox(BOOMERANG_BBOX_WIDTH, BOOMERANG_BBOX_HEIGHT);

	aniID = ANI_ID_BOOMERANG;

	Refresh();
}

void CBoomerang::Prepare(DWORD dt)
{
	switch (action)
	{
	case ANI_ID_BOOMERANG_HOLD:
		vx = 0.0f;
		vy = 0.0f;
		break;
	case ANI_ID_BOOMERANG_FLY:
		vx_oscillate_phase += BOOMERANG_FREQUENCY * dt;
		vx_oscillate_phase = min(vx_oscillate_phase, 1.5 * M_PI);
		vx = nx * BOOMERANG_VX_AMPLITUDE * sin(vx_oscillate_phase);

		vy_oscillate_phase += BOOMERANG_FREQUENCY * dt;
		vy_oscillate_phase = min(vy_oscillate_phase, 2.0 * M_PI);
		vy = BOOMERANG_VY_AMPLITUDE * sin(vy_oscillate_phase);
		break;
	}
}

void CBoomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Move(dt);	
}

void CBoomerang::Render()
{
	int object = ANI_ID_BOOMERANG;
	int action = this->action;
	int direction = nx <= 0 ? ID_ANI_DIRECTION_LEFT : ID_ANI_DIRECTION_RIGHT;
	aniID = object + action + direction;

	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniID)->Render(x, y);
}

int CBoomerang::IsCollidable()
{
	return vx || vy;
}

void CBoomerang::OnReactionTo(LPCOLLISIONEVENT e, int action)
{
	if (dynamic_cast<CMario*>(e->src_obj))
	{
		e->Reverse();
		Attack(e);
	}
}

void CBoomerang::Refresh()
{
	vx_oscillate_phase = BOOMERANG_INITIAL_PHASE_VX;
	vy_oscillate_phase = BOOMERANG_INITIAL_PHASE_VY;
	action = ANI_ID_BOOMERANG_HOLD;
}

void CBoomerang::Fire()
{
	this->action = ANI_ID_BOOMERANG_FLY;
}
