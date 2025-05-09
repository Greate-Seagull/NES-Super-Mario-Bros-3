#include "KoopaTroopa.h"
#include "Platform.h"

#include "debug.h"
#include "PlayScene.h"

CKoopaTroopa::CKoopaTroopa(float x, float y):
	CCreature(x, y)
{	
	vy = KOOPA_VY;	
	SetState(STATE_LIVE);
	life = KOOPA_LIFE;
}

void CKoopaTroopa::Prepare(DWORD dt)
{
	switch (state)
	{
	case STATE_LIVE:
		CMovableObject::Prepare(dt);
		break;
	case KOOPA_STATE_HIDE:
		Hide(dt);
		if (!IsControlled()) CMovableObject::Prepare(dt);
		break;
	case KOOPA_STATE_ROLL:
		CMovableObject::Prepare(dt);
		break;
	case KOOPA_STATE_POP:
		Pop(dt);
		if (!IsControlled()) CMovableObject::Prepare(dt);
		break;
	}

	/*DebugOutTitle(L"%f, %f", x, y);*/
}

void CKoopaTroopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{		
	switch (state)
	{
	case STATE_LIVE:
		Living(dt);
		break;
	case KOOPA_STATE_HIDE:
		Hiding(dt);
		break;
	case KOOPA_STATE_ROLL:
		Rolling(dt);
		break;
	case KOOPA_STATE_POP:
		Poping(dt);
		break;
	}
}

void CKoopaTroopa::Living(DWORD dt)
{	
	Move(dt);
}

void CKoopaTroopa::Hiding(DWORD dt)
{	
	Move(dt);
}

void CKoopaTroopa::Rolling(DWORD dt)
{
	Move(dt);
}

void CKoopaTroopa::Poping(DWORD dt)
{	
	Move(dt);
}

void CKoopaTroopa::OnNoCollisionWithBlocking(DWORD dt)
{
	isOnGround = false;

	switch (state)
	{
	case STATE_LIVE:
		Patrol();
		y = on_ground_y;
		vy = 0.0f;
		break;
	}
}

void CKoopaTroopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CPlatform*>(e->obj))
		OnCollisionWithPlatform(e);
	else if (dynamic_cast<CBlock*>(e->obj))
		OnCollisionWithBlock(e);
	else if (dynamic_cast<CHarmfulObject*>(e->obj))
		OnCollisionWithHarmfulObject(e);
}

void CKoopaTroopa::OnCollisionWithPlatform(LPCOLLISIONEVENT e)
{
	if (e->ny)
	{
		vy = 0.0f;

		if (e->ny < 0) 
		{
			isOnGround = true;
			on_ground_y = y;
		}

		switch (state)
		{
		case KOOPA_STATE_POP:
		case KOOPA_STATE_HIDE:
			Bounce();
			break;
		}
	}

	if (e->nx)
	{
		Patrol();
	}
}

void CKoopaTroopa::OnCollisionWithBlock(LPCOLLISIONEVENT e)
{	
	if (e->ny)
	{
		vy = 0.0f;

		if (e->ny < 0)
		{
			isOnGround = true;
			on_ground_y = y;
		}
	}

	if (e->nx)
	{
		Patrol();
	}

	switch (state)
	{
	case KOOPA_STATE_POP:
	case KOOPA_STATE_HIDE:
		ny = e->ny;
		Bounce();
		break;
	case KOOPA_STATE_ROLL:
		Destroy(e->obj);
		break;
	}
}

void CKoopaTroopa::OnCollisionWithHarmfulObject(LPCOLLISIONEVENT e)
{
	CHarmfulObject* obj = dynamic_cast<CHarmfulObject*>(e->obj);

	switch (state)
	{
		case STATE_LIVE:
			Attack(obj);
			break;
		case KOOPA_STATE_HIDE:
		case KOOPA_STATE_POP:
			if (dynamic_cast<CMario*>(obj) == nullptr)
			{
				Destroy(obj);
				Die();
			}
			break;
		case KOOPA_STATE_ROLL:
			if (dynamic_cast<CRacoonTail*>(obj) == nullptr)
				Destroy(obj);
			break;
		case STATE_DIE:
			break;
	}
}

void CKoopaTroopa::Patrol()
{
	nx = -nx;
	vx = nx * fabs(vx);
}

void CKoopaTroopa::Render()
{
	ChangeAnimation();

	CAnimations::GetInstance()->Get(aniID)->Render(x, y);
	//RenderBoundingBox();
}

void CKoopaTroopa::ChangeAnimation()
{
	int action = ANI_ID_KOOPA_WALK;
	switch (state)
	{
		case STATE_LIVE:
			action = ANI_ID_KOOPA_WALK;
			break;
		case KOOPA_STATE_HIDE:
			action = ANI_ID_KOOPA_HIDE;
			break;
		case KOOPA_STATE_POP:
		{
			action = ANI_ID_KOOPA_POP;
			//aniID = ANI_ID_KOOPA + action;
			//DWORD timePerFrame = ( (float)remaining_time / KOOPA_POP_TIME) * KOOPA_POP_MAX_FRAME_TIME;			
			//CAnimations::GetInstance()->Get(aniID)->ChangeTimePerFrame(15);
			break;
		}
		case KOOPA_STATE_ROLL:
			action = ANI_ID_KOOPA_ROLL;
			break;
		case STATE_DIE:
			break;
	}

	int direction = ID_ANI_DIRECTION_LEFT;
	switch (state)
	{
		case STATE_LIVE:
			direction = (nx <= 0) ? ID_ANI_DIRECTION_LEFT : ID_ANI_DIRECTION_RIGHT;
			break;
		case KOOPA_STATE_HIDE:
			direction = (ny < 0) ? ID_ANI_DIRECTION_UP : ID_ANI_DIRECTION_DOWN;
			break;
		case KOOPA_STATE_POP:
		{
			direction = (ny < 0) ? ID_ANI_DIRECTION_UP : ID_ANI_DIRECTION_DOWN;
			break;
		}
		case KOOPA_STATE_ROLL:
			direction = (nx <= 0) ? ID_ANI_DIRECTION_LEFT : ID_ANI_DIRECTION_RIGHT;
			direction += (ny < 0) ? ID_ANI_DIRECTION_UP : ID_ANI_DIRECTION_DOWN;
			break;
		case STATE_DIE:
			break;
	}

	aniID = ANI_ID_KOOPA + action + direction;
}

void CKoopaTroopa::SetState(int state)
{
	/*if (this->state == state)
	{
		return;
	}*/

	this->state = state;

	switch (state)
	{
		case STATE_LIVE:
			y += (bbox_height - KOOPA_BBOX_HEIGHT_FOOT_1) / 2.0f - 0.01f;
			SetBoundingBox(KOOPA_BBOX_WIDTH_LIVE, KOOPA_BBOX_HEIGHT_FOOT_1);
			LookForMario();
			vx = nx * KOOPA_VX;
			AgainstControl();
			LoseHighPower();
			break;
		case KOOPA_STATE_HIDE:
			y += (bbox_height - KOOPA_BBOX_HEIGHT_HIDE) / 2.0f - 0.01f;
			SetBoundingBox(KOOPA_BBOX_WIDTH_HIDE, KOOPA_BBOX_HEIGHT_HIDE);
			recovering_time = 0;
			SetHighPower();
			break;
		case KOOPA_STATE_POP:
			vx = KOOPA_POP_VX;
			break;
		case KOOPA_STATE_ROLL:
			vx = nx * KOOPA_ROLL_VX;
			break;
		case STATE_DIE:
			AgainstControl();
			Delete();
			break;
	}
}

void CKoopaTroopa::ReactionToCarry(CGameObject* by_another)
{
	switch (state)
	{
	case STATE_LIVE:
		Attack(by_another);
		AgainstControl();
		break;
	case KOOPA_STATE_ROLL:
		Destroy(by_another);
		AgainstControl();
		break;
	case KOOPA_STATE_POP:
	case KOOPA_STATE_HIDE:
		vy = 0.0f;
		break;
	}
}

void CKoopaTroopa::ReactionToTouch(CGameObject* by_another)
{
	switch (state)
	{
	case STATE_LIVE:
		Attack(by_another);
		break;
	case KOOPA_STATE_ROLL:
		Destroy(by_another);
		AgainstControl();
		break;
	case KOOPA_STATE_POP:
	case KOOPA_STATE_HIDE:
		if (CMario* mario = dynamic_cast<CMario*>(by_another)) //mario attacks
		{
			nx = (x < mario->GetX()) ? DIRECTION_LEFT : DIRECTION_RIGHT;
			this->SetState(KOOPA_STATE_ROLL);

			Touch(mario);
		}
		else
		{
			Destroy(by_another);
			Die();
		}
		break;
	}
}

void CKoopaTroopa::ReactionToAttack1(CGameObject* by_another)
{
	switch (state)
	{
	case STATE_LIVE:
		UnderAttack(by_another);
		SetState(KOOPA_STATE_HIDE);
		break;
	case KOOPA_STATE_ROLL:
		if (dynamic_cast<CMario*>(by_another)) SetState(KOOPA_STATE_HIDE);
		else Destroy((CHarmfulObject*)by_another);
		break;
	case KOOPA_STATE_POP:
	case KOOPA_STATE_HIDE:
		if (CMario* mario = dynamic_cast<CMario*>(by_another)) //mario attacks
		{
			nx = (x < mario->GetX()) ? DIRECTION_LEFT : DIRECTION_RIGHT;
			this->SetState(KOOPA_STATE_ROLL);

			Touch(mario);
		}
		else
		{
			Destroy(by_another);
			Die();
		}
		break;
	}
}

void CKoopaTroopa::ReactionToAttack2(CGameObject* by_another)
{
	switch (state)
	{
	case STATE_LIVE:
		UnderAttack(by_another);
		SetState(KOOPA_STATE_HIDE);
		break;
	case KOOPA_STATE_ROLL:
		if (dynamic_cast<CMario*>(by_another)) SetState(KOOPA_STATE_HIDE);
		else Destroy((CHarmfulObject*)by_another);
		break;
	case KOOPA_STATE_POP:
	case KOOPA_STATE_HIDE:
		if (CMario* mario = dynamic_cast<CMario*>(by_another)) //mario attacks
		{
			nx = (x < mario->GetX()) ? DIRECTION_LEFT : DIRECTION_RIGHT;
			this->SetState(KOOPA_STATE_ROLL);

			Touch(mario);
		}
		else
		{
			Destroy(by_another);
			Die();
		}
		break;
	}
}

void CKoopaTroopa::ReactionToAttack3(CGameObject* by_another)
{
	/*switch (state)
	{
	case STATE_LIVE:
		UnderAttack(by_another);
		UnderDestructrion(by_another);
		SetState(KOOPA_STATE_HIDE);
		break;
	case KOOPA_STATE_ROLL:
		UnderDestructrion(by_another);
		SetState(KOOPA_STATE_HIDE);
		break;
	case KOOPA_STATE_POP:
	case KOOPA_STATE_HIDE:
		UnderDestructrion(by_another);
		SetState(KOOPA_STATE_HIDE);
		break;
	}*/

	UnderDestructrion(by_another);
	Die();
}

void CKoopaTroopa::UnderAttack(CGameObject* by_another)
{
	if (CMario* mario = dynamic_cast<CMario*>(by_another))
	{
		CCreature::UnderAttack(mario);
	}
}

void CKoopaTroopa::UnderDestructrion(CGameObject* by_another)
{
	float enemy_x, enemy_y;
	by_another->GetPosition(enemy_x, enemy_y);

	int fly_out_direction = (x <= enemy_x) ? DIRECTION_LEFT : DIRECTION_RIGHT;
	FlyOut(fly_out_direction);	
}

void CKoopaTroopa::Bounce()
{
	if (fabs(vy) > 0.16f)
	{
		vx = nx * ATTACK_BOOM_VX;
		vy = ny * KOOPA_BOUNCE_VY;
	}
	else
	{
		vx = 0.0f;
		vy = 0.0f;
	}
}

void CKoopaTroopa::Hide(DWORD dt)
{
	recovering_time += dt;

	if (recovering_time >= KOOPA_HIDE_TIME)
	{
		recovering_time %= KOOPA_HIDE_TIME;
		SetState(KOOPA_STATE_POP);
	}
}

void CKoopaTroopa::Pop(DWORD dt)
{
	recovering_time += dt;

	if (recovering_time >= KOOPA_POP_TIME)
	{
		//recovering_time %= KOOPA_POP_TIME;
		CCreature::Recover();
	}
	else
	{
		vx = -vx;
	}
}

void CKoopaTroopa::LookForMario()
{
	LPPLAYSCENE playScene = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
	CMario* mario = (CMario*)playScene->GetPlayer();

	float mario_x, mario_y;
	mario->GetPosition(mario_x, mario_y);
	nx = (mario_x < x) ? DIRECTION_LEFT: DIRECTION_RIGHT;
}