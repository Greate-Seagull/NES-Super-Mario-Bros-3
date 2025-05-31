#include "BoomerangBrother.h"

#include "Mario.h"
#include "Block.h"
#include "Platform.h"

CBoomerangBrother::CBoomerangBrother(float x, float y):
	CEnemy(x, y)
{
	Refresh();
}

void CBoomerangBrother::Prepare(DWORD dt)
{
	switch (state)
	{
	case STATE_LIVE:		
		if (life)
		{
			oscillate_phase += M_PI / 1200.0f * dt;
			if (oscillate_phase >= 2.0f * M_PI) oscillate_phase -= 2.0f * M_PI;

			vx = M_PI / 75.0f * sin(oscillate_phase);
		}
		//vy = 0.0f;
		break;
	}

	CMovableObject::Prepare(dt);
}

void CBoomerangBrother::SetSpecialAction(int action)
{
	this->special_action = action;
}

void CBoomerangBrother::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (life) LookForMario();

	Move(dt);
	DoAction();
}

void CBoomerangBrother::DoAction()
{	
	switch (special_action)
	{
	case ANI_ID_BOOMERANG_BROTHER_TOSH:
		Toshing();
	case ANI_ID_BOOMERANG_BROTHER_SWING:
		Swinging();
	}
}

void CBoomerangBrother::Toshing()
{
	if (vx > 0.0f && oscillate_phase >= 0.5f * M_PI)
		SetSpecialAction(ANI_ID_BOOMERANG_BROTHER_SWING);
	else if (vx < 0.0f && oscillate_phase >= 1.5f * M_PI)
		SetSpecialAction(ANI_ID_BOOMERANG_BROTHER_SWING);
}

void CBoomerangBrother::Swinging()
{
	if (vx > 0.0f && oscillate_phase >= 0.75f * M_PI)
		SetSpecialAction(ANI_ID_BOOMERANG_BROTHER_TOSH);
	else if (vx < 0.0f && oscillate_phase >= 1.75f * M_PI)
		SetSpecialAction(ANI_ID_BOOMERANG_BROTHER_TOSH);
}

void CBoomerangBrother::ChangeAnimation()
{
	int object = GetObjectAniID();
	int action = special_action;
	int horizontal = nx <= 0 ? ID_ANI_DIRECTION_LEFT : ID_ANI_DIRECTION_RIGHT;
	int vertical = life == 0.0f ? ID_ANI_DIRECTION_UP: ID_ANI_DIRECTION_DOWN;

	aniID = object + action + vertical + horizontal;
}

void CBoomerangBrother::Render()
{
	ChangeAnimation();
	CAnimations::GetInstance()->Get(aniID)->Render(x, y);
}

void CBoomerangBrother::OnNoCollisionWithBlocking(DWORD dt)
{
	isOnGround = false;
}

void CBoomerangBrother::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CPlatform*>(e->obj))
		OnCollisionWithPlatform(e);
	else if (dynamic_cast<CBlock*>(e->obj))
		OnCollisionWithBlock(e);
	else if (dynamic_cast<CMario*>(e->obj))
		OnCollisionWithMario(e);
	else if (dynamic_cast<CEnemy*>(e->obj))
		OnCollisionWithEnemy(e);
}

void CBoomerangBrother::OnCollisionWithPlatform(LPCOLLISIONEVENT e)
{
	if (e->ny)
	{
		vy = 0.0f;
		isOnGround = true;
	}
	if (e->nx)
	{		
		nx = -nx;
		vx = nx * fabs(vx);
	}
}

void CBoomerangBrother::OnCollisionWithBlock(LPCOLLISIONEVENT e)
{
	if (e->ny)
	{
		vy = 0.0f;
		isOnGround = true;
	}
	if (e->nx)
	{
		nx = -nx;
		vx = nx * fabs(vx);
	}
}

void CBoomerangBrother::OnCollisionWithMario(LPCOLLISIONEVENT e)
{
	Attack(e);
}

void CBoomerangBrother::OnCollisionWithEnemy(LPCOLLISIONEVENT e)
{
	Touch(e);
	nx = -nx;
	vx = nx * fabs(vx);
}

void CBoomerangBrother::OnReactionToAttack1(LPCOLLISIONEVENT e)
{
	CHarmfulObject::OnReactionToAttack3(e);
	UnderAttack((CHarmfulObject*)e->obj);
}

void CBoomerangBrother::OnReactionToAttack2(LPCOLLISIONEVENT e)
{
	CHarmfulObject::OnReactionToAttack3(e);
	UnderAttack((CHarmfulObject*)e->obj);
}

void CBoomerangBrother::OnReactionToAttack3(LPCOLLISIONEVENT e)
{
	CHarmfulObject::OnReactionToAttack3(e);
	UnderAttack((CHarmfulObject*)e->obj);
}

void CBoomerangBrother::Refresh()
{
	LookForMario();
	life = BOOMERANG_BROTHER_LIFE;
	SetBoundingBox(BOOMERANG_BROTHER_BBOX_WIDTH, BOOMERANG_BROTHER_BBOX_HEIGHT);
	SetState(STATE_LIVE);
	oscillate_phase = 0.0f;
	special_action = ANI_ID_BOOMERANG_BROTHER_SWING;
}
