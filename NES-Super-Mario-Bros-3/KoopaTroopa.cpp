#include "KoopaTroopa.h"
#include "Platform.h"

#include "PlayScene.h"

CKoopaTroopa::CKoopaTroopa(float x, float y, bool haveWings):
	CEnemy(x, y)
{	
	this->bornWithWings = haveWings;

	SetBoundingBox(KOOPA_BBOX_WIDTH_LIVE, KOOPA_BBOX_HEIGHT_LIVE);
}

void CKoopaTroopa::SetPosition(float x, float y)
{
	this->x = x; this->y = y;

	if (wings)
		wings->SetPosition(x - nx * WINGS_X_OFFSET, y + WINGS_Y_OFFSET);
}

void CKoopaTroopa::Prepare(DWORD dt)
{
	switch (state)
	{
	case STATE_LIVE:
		LivingPrepare(dt);
		break;
	case KOOPA_STATE_HIDE:
		Hide(dt);
		if (carrier == nullptr) CMovableObject::Prepare(dt);
		break;
	case KOOPA_STATE_ROLL:
		CMovableObject::Prepare(dt);
		break;
	case KOOPA_STATE_POP:
		Pop(dt);
		if (carrier == nullptr) CMovableObject::Prepare(dt);
		break;
	}
}

void CKoopaTroopa::LivingPrepare(DWORD dt)
{
	vx = nx * (wings ? WINGS_VX : KOOPA_VX);
	CMovableObject::Prepare(dt);
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

	if (wings)
		Flutter();
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
}

void CKoopaTroopa::OnCollisionWith(LPCOLLISIONEVENT e)
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

void CKoopaTroopa::OnCollisionWithPlatform(LPCOLLISIONEVENT e)
{
	if (e->ny)
	{		
		if (isOnGround == false && (state == KOOPA_STATE_HIDE || state == KOOPA_STATE_POP))
			Bounce();
		else
			vy = 0.0f;

		if (e->ny < 0)
		{
			isOnGround = true;
			on_ground_x = x;
			on_ground_y = y;

			if (wings)
				vy = WINGS_JUMP_VY;
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
		if (isOnGround == false && (state == KOOPA_STATE_HIDE || state == KOOPA_STATE_POP))
			Bounce();
		else
			vy = 0.0f;

		if (e->ny < 0)
		{
			isOnGround = true;
			on_ground_x = x;
			on_ground_y = y;

			if (wings)
				vy = WINGS_JUMP_VY;
		}
	}

	if (e->nx)
	{
		Patrol();

		switch (state)
		{		
		case KOOPA_STATE_ROLL:
			HigherAttack(e);
			break;
		}
	}
}

void CKoopaTroopa::OnCollisionWithMario(LPCOLLISIONEVENT e)
{
	switch (state)
	{
	case STATE_LIVE:
		Attack(e);
		break;
	case KOOPA_STATE_ROLL:
		HigherAttack(e);
		break;
	default:
		e->Reverse();
		e->obj->OnCollisionWith(e);
		break;
	}
}

void CKoopaTroopa::OnCollisionWithEnemy(LPCOLLISIONEVENT e)
{
	switch (state)
	{
	case KOOPA_STATE_ROLL:
		HigherAttack(e);
		break;
	default:
		if (carrier)
		{
			HigherAttack(e);
			e->Reverse();
			e->src_obj = this;
			OnReactionToAttack2(e);
		}
		else
		{
			Patrol();
			Touch(e);
		}
	}
}

void CKoopaTroopa::Patrol()
{
	nx = -nx;
	vx = nx * fabs(vx);
}

void CKoopaTroopa::Refresh()
{
	CEnemy::Refresh();

	maxVx = KOOPA_VX;
	vy = KOOPA_VY;
	if (bornWithWings) GrowWings();
	SetState(STATE_LIVE);
	life = KOOPA_LIFE;
}

void CKoopaTroopa::GrowWings()
{
	if (wings == nullptr)
		wings = new CWing(x, y);
}

void CKoopaTroopa::LoseWings()
{
	if (wings)
	{
		delete wings;
		wings = nullptr;
	}
}

void CKoopaTroopa::Flutter()
{
	wings->SetPosition(x - nx * WINGS_X_OFFSET, y + WINGS_Y_OFFSET);
	wings->SetNx(this->nx * -1);
}

void CKoopaTroopa::Render()
{
	ChangeAnimation();

	CAnimations::GetInstance()->Get(aniID)->Render(x, y);

	if (wings)
		wings->Render();
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

	aniID = GetObjectAniID() + action + direction;
}

int CKoopaTroopa::GetObjectAniID()
{
	return ANI_ID_KOOPA;
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
			y += (bbox_height - KOOPA_BBOX_HEIGHT_LIVE) / 2.0f - 0.01f;
			SetBoundingBox(KOOPA_BBOX_WIDTH_LIVE, KOOPA_BBOX_HEIGHT_LIVE);
			LookForMario();
			vx = nx * KOOPA_VX;
			if (carrier) AgainstControl();
			break;
		case KOOPA_STATE_HIDE:
			y += (bbox_height - KOOPA_BBOX_HEIGHT_HIDE) / 2.0f - 0.01f;
			SetBoundingBox(KOOPA_BBOX_WIDTH_HIDE, KOOPA_BBOX_HEIGHT_HIDE);
			recovering_time = 0;
			break;
		case KOOPA_STATE_POP:
			vx = KOOPA_POP_VX;
			break;
		case KOOPA_STATE_ROLL:
			vx = nx * KOOPA_ROLL_VX;
			break;
		case STATE_DIE:
			if (carrier) AgainstControl();
			Delete();
			break;
	}
}

void CKoopaTroopa::OnReactionToCarrying(LPCOLLISIONEVENT e)
{
	switch (state)
	{
	case STATE_LIVE:
		e->Reverse();
		Attack(e);
		break;
	case KOOPA_STATE_ROLL:
		e->Reverse();
		HigherAttack(e);
		break;
	case KOOPA_STATE_POP:
	case KOOPA_STATE_HIDE:
		AcceptControl((CCreature*)e->src_obj);
		Stop();
		break;
	}
}

void CKoopaTroopa::OnReactionToTouching(LPCOLLISIONEVENT e)
{
	switch (state)
	{
	case KOOPA_STATE_ROLL:
		e->Reverse();
		HigherAttack(e);
		break;
	case STATE_LIVE:
		if (dynamic_cast<CMario*>(e->src_obj))
		{
			e->Reverse();
			Attack(e);
		}
		else
		{
			Patrol();
		}
		break;
	case KOOPA_STATE_POP:
	case KOOPA_STATE_HIDE:
		if (CMario* mario = dynamic_cast<CMario*>(e->src_obj))
		{
			e->Reverse();
			Touch(e);
			SetNx(this->x < e->src_obj->GetX() ? DIRECTION_LEFT : DIRECTION_RIGHT);
			SetState(KOOPA_STATE_ROLL);
		}
		else if (CEnemy* enemy = dynamic_cast<CEnemy*>(e->src_obj))
		{
			if (carrier) 
			{
				e->Reverse();
				HigherAttack(e);
				e->src_obj = this;
				OnReactionToAttack2(e);
			}
		}
		break;
	}
}

void CKoopaTroopa::OnReactionToAttack1(LPCOLLISIONEVENT e)
{
	switch (state)
	{
	case KOOPA_STATE_POP:
	case KOOPA_STATE_HIDE:
		if (CMario* mario = dynamic_cast<CMario*>(e->src_obj))
		{
			SetNx(this-> x < e->src_obj->GetX() ? DIRECTION_LEFT: DIRECTION_RIGHT);
			SetState(KOOPA_STATE_ROLL);
		}
		else if (CEnemy* enemy = dynamic_cast<CEnemy*>(e->src_obj))
		{
			if (carrier)
			{
				e->Reverse();
				HigherAttack(e);
				e->src_obj = this;
				OnReactionToAttack2(e);
			}
		}
		break;
	default:
		e->Reverse();
		Touch(e);

		if (wings)
			LoseWings();
		else
		{
			ny = -1 * e->ny;
			SetState(KOOPA_STATE_HIDE);
			Stop();
		}
		break;
	}	
}

void CKoopaTroopa::OnReactionToAttack2(LPCOLLISIONEVENT e)
{
	if (wings)
		LoseWings();
	CHarmfulObject::OnReactionToAttack2(e);
	SetState(KOOPA_STATE_HIDE);
	SetLife(0.0f); //Be killed
	if (carrier) AgainstControl();
}

void CKoopaTroopa::OnReactionToAttack3(LPCOLLISIONEVENT e)
{
	if (wings)
		LoseWings();
	SetState(KOOPA_STATE_HIDE);
	CHarmfulObject::OnReactionToAttack3(e);
	isFliedOut = false; //Not be killed
}

void CKoopaTroopa::UnderAttack(CGameObject* by_another)
{
	if (CMario* mario = dynamic_cast<CMario*>(by_another))
	{
		/*float mX, mY;
		mario->GetPosition(mX, mY);
		mario->InsertFlyingScore(mX, mY - 16);*/

		CCreature::UnderAttack(mario);
	}
}

void CKoopaTroopa::Bounce()
{
	if (fabs(vy) > 0.16f)
	{
		vx = nx * fabs(maxVx);
		vy = ny * KOOPA_BOUNCE_VY;
	}
	else
	{
		Stop();
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