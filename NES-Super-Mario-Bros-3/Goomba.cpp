#include "Goomba.h"
#include "Mario.h"
#include "Platform.h"

CGoomba::CGoomba(float x, float y):
	CCreature(x, y, GOOMBA_SHARP, GOOMBA_LIFE)
{
	bbox_height = GOOMBA_BBOX_HEIGHT;
	bbox_width = GOOMBA_BBOX_WIDTH;
	vx = -GOOMBA_VX;
	die_start = -1;
}

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	//if (!e->obj->IsBlocking()) return; 
	//if (dynamic_cast<CGoomba*>(e->obj)) return; 
	if (dynamic_cast<CPlatform*>(e->obj))
	{
		OnCollisionWithPlatform(e);
	}
}

void CGoomba::OnCollisionWithPlatform(LPCOLLISIONEVENT e)
{
	if (e->ny)
	{
		vy = 0;
	}
	if (e->nx)
	{
		vx = -vx;
	}
}

void CGoomba::SetState(int state)
{
	switch (state)
	{
		case STATE_DIE:
			die_start = GetTickCount64();
			y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE) / 2;
			bbox_height = GOOMBA_BBOX_HEIGHT_DIE;
			vx = 0;
			break;
	}
}

void CGoomba::ProcessState()
{
	switch (state)
	{
		case STATE_DIE:
			if (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT)
			{
				this->Delete();
			}
			break;
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	ProcessLife();
	ProcessState();

	dt = 16;
	Accelerate(0.0f, GAME_GRAVITY, dt);
	CCollision::GetInstance()->Process(this, dt, coObjects);
	Move(dt);
}


void CGoomba::Render()
{
	int aniId = ID_ANI_GOOMBA_WALKING;
	if (state == STATE_DIE)
	{
		aniId = ID_ANI_GOOMBA_DIE;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x,y);
	RenderBoundingBox();
}