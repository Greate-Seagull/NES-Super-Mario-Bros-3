#include "BoomerangBrother.h"

#include "PlayScene.h"

#include "Platform.h"

CBoomerangBrother::CBoomerangBrother(float x, float y):
	CEnemy(x, y)
{	
	magSize = BOOMERANG_BROTHER_MAGSIZE;

	Refresh();
}

void CBoomerangBrother::Prepare(DWORD dt)
{
	switch (state)
	{
	case STATE_LIVE:		
		if (life)
		{
			oscillate_phase += BOOMERANG_BROTHER_FREQUENCY * dt;
			if (oscillate_phase >= 2.0f * M_PI) oscillate_phase -= 2.0f * M_PI;

			vx = BOOMERANG_BROTHER_AMPLITUDE * sin(oscillate_phase);

			if (isOnGround && rand() % 1000 < 1) //Random jump
				vy = BOOMERANG_BROTHER_RANDOM_VY;

			LookForMario();			
		}
		break;
	}

	CMovableObject::Prepare(dt);
}

void CBoomerangBrother::SetSpecialAction(int action)
{
	this->special_action = action;
}

void CBoomerangBrother::Tosh()
{
	if (isFiring == false)
		return;

	boomerang[magSize - 1]->SetNx(this->nx);
	boomerang[magSize - 1]->Fire();

	magSize--;
	isFiring = magSize > 0;
}

void CBoomerangBrother::Swing()
{
	boomerang[magSize - 1]->Refresh();
}

void CBoomerangBrother::Reload()
{
	magSize++;
	isFiring = magSize == BOOMERANG_BROTHER_MAGSIZE;
}

void CBoomerangBrother::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{	
	Move(dt);

	if(life)
		DoAction();
}

void CBoomerangBrother::DoAction()
{	
	switch (special_action)
	{
	case ANI_ID_BOOMERANG_BROTHER_TOSH:
		Toshing();
		break;
	case ANI_ID_BOOMERANG_BROTHER_SWING:
		Swinging();
		break;
	case ANI_ID_BOOMERANG_BROTHER_RELOAD:
		Reloading();
		break;
	}
}

void CBoomerangBrother::Toshing()
{
	float k = oscillate_phase / M_PI;
	float phase = k - (int)k;
	if (phase >= 0.5f && phase <= 0.75f)
	{
		if (isFiring)
		{
			Swing();
			SetSpecialAction(ANI_ID_BOOMERANG_BROTHER_SWING);
		}
		else
			SetSpecialAction(ANI_ID_BOOMERANG_BROTHER_RELOAD);
	}	
}

void CBoomerangBrother::Swinging()
{
	boomerang[magSize - 1]->SetNx(this->nx);
	boomerang[magSize - 1]->SetPosition(x + nx * BOOMERANG_HOLD_OFFSET_X, y + BOOMERANG_HOLD_OFFSET_Y);

	float k = oscillate_phase / M_PI;
	float phase = k - (int)k;
	if (phase < 0.5f || phase > 0.75f)
	{
		Tosh();
		SetSpecialAction(ANI_ID_BOOMERANG_BROTHER_TOSH);
	}
}

void CBoomerangBrother::Reloading()
{
	float k = oscillate_phase / M_PI;
	float phase = k - (int)k;
	if (phase < 0.5f || phase > 0.75f)
	{
		Reload();
		SetSpecialAction(ANI_ID_BOOMERANG_BROTHER_TOSH);
	}
}

void CBoomerangBrother::ChangeAnimation()
{
	int object = GetObjectAniID();
	int action = special_action != ANI_ID_BOOMERANG_BROTHER_SWING ? ANI_ID_BOOMERANG_BROTHER_TOSH : special_action;
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
	else if (dynamic_cast<CBoomerang*>(e->obj))
		OnCollisionWithBoomerang(e);
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

void CBoomerangBrother::OnCollisionWithBoomerang(LPCOLLISIONEVENT e)
{
	e->obj->SetPosition(DEFAULT_X, DEFAULT_Y);
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
	e->Reverse();
	Touch(e);
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
	CEnemy::Refresh();

	LookForMario();
	life = BOOMERANG_BROTHER_LIFE;
	SetBoundingBox(BOOMERANG_BROTHER_BBOX_WIDTH, BOOMERANG_BROTHER_BBOX_HEIGHT);
	SetState(STATE_LIVE);

	oscillate_phase = BOOMERANG_BROTHER_INITIAL_PHASE;	
	special_action = ANI_ID_BOOMERANG_BROTHER_TOSH;
	isFiring = magSize == BOOMERANG_BROTHER_MAGSIZE;
}

void CBoomerangBrother::TakeBoomerangs()
{
	boomerang.resize(magSize);
	for (int i = 0; i < boomerang.size(); i++)
	{
		boomerang[i] = new CBoomerang(DEFAULT_X, DEFAULT_Y);
	}
}

void CBoomerangBrother::CreateItem(CPlayScene* ps)
{
	TakeBoomerangs();
	for (int i = 0; i < boomerang.size(); i++)
	{
		boomerang[i]->CreateItem(ps);
		ps->Insert(boomerang[i], -1);
	}
}