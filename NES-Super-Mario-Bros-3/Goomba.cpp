#include "Goomba.h"

#include "Mario.h"
#include "Block.h"
#include "Platform.h"
#include "PlayScene.h"

CGoomba::CGoomba(float x, float y, bool haveWings) :
	CEnemy(x, y)
{
	bornWithWings = haveWings;	
}

void CGoomba::SetPosition(float x, float y)
{
	this->x = x; this->y = y;

	if(wings)
		wings->SetPosition(x, y + WINGS_Y_OFFSET);
}

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	//if (!e->obj->IsBlocking()) return; 
	if (dynamic_cast<CPlatform*>(e->obj))
		OnCollisionWithPlatform(e);
	else if (dynamic_cast<CBlock*>(e->obj))
		OnCollisionWithBlock(e);
	else if (dynamic_cast<CMario*>(e->obj))
		OnCollisionWithMario(e);
	else if (dynamic_cast<CEnemy*>(e->obj))
		OnCollisionWithEnemy(e);
}

void CGoomba::OnCollisionWithPlatform(LPCOLLISIONEVENT e)
{
	if (e->ny)
	{
		vy = 0.0f;
		isOnGround = true;
	}
	if (e->nx)
	{
		vx = -vx;
	}
}

void CGoomba::OnCollisionWithBlock(LPCOLLISIONEVENT e)
{
	if (e->ny)
	{
		vy = 0.0f;
		isOnGround = true;
	}
	if (e->nx)
	{
		vx = -vx;
	}
}

void CGoomba::OnCollisionWithEnemy(LPCOLLISIONEVENT e)
{
	/*nx = -nx;
	vx = nx * GOOMBA_VX;*/
	Touch(e);
	vx = -vx;
}

void CGoomba::OnCollisionWithMario(LPCOLLISIONEVENT e)
{
	Attack(e);
}

void CGoomba::OnReactionToTouching(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CMario*>(e->src_obj))
	{
		e->Reverse();
		Attack(e);
	}
	else
	{
		/*nx = -nx;
		vx = nx * GOOMBA_VX;*/
		vx = -vx;
	}
}

void CGoomba::OnReactionToAttack1(LPCOLLISIONEVENT e)
{
	/*if (dynamic_cast<CMario*>(by_another))
	{
		CMario* m = (CMario*)by_another;
		float mX, mY;
		m->GetPosition(mX, mY);
		m->InsertFlyingScore(mX, mY - 16);
	}*/
	if (CMario* player = dynamic_cast<CMario*>(e->src_obj))
		player->InsertFlyingScore(x, y);
	
	e->Reverse();
	Touch(e);

	if (e->ny)
		vy = 0.0f;
	
	if (wings)
		LoseWings();		
	else
		Die();
}

void CGoomba::OnReactionToAttack2(LPCOLLISIONEVENT e)
{
	if (wings)
		LoseWings();
	CHarmfulObject::OnReactionToAttack2(e);
	SetLife(life - 1.0f);
}

//void CGoomba::ReactionToAttack3(CGameObject* by_another)
//{
//	if (dynamic_cast<CKoopaTroopa*>(by_another)
//		|| dynamic_cast<CRacoonTail*>(by_another))
//	{
//		LPPLAYSCENE currentScene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
//		currentScene->InsertScore(x, y - 16, 100);
//	}
//	Die();
//}

void CGoomba::OnReactionToAttack3(LPCOLLISIONEVENT e)
{	
	LPPLAYSCENE currScene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	CMario* player = (CMario*)currScene->GetPlayer();
	player->InsertScoreObject(x, y - 16, 100);

	if (wings)
		LoseWings();
	CHarmfulObject::OnReactionToAttack3(e);
	SetLife(life - 1.0f);
}

void CGoomba::SetState(int state)
{
	/*if (this->state == state)
	{
		return;
	}*/

	this->state = state;

	switch (state)
	{
	case STATE_LIVE:
		ToStateLiving();
		break;
	case STATE_DIE:
		ToStateDying();
		break;
	}
}

void CGoomba::ToStateLiving()
{
	SetBoundingBox(GOOMBA_BBOX_WIDTH, GOOMBA_BBOX_HEIGHT);
	vx = nx * GOOMBA_VX;
	vy = GOOMBA_VY;
}

void CGoomba::ToStateDying()
{
	die_start = 0;
	y += (bbox_height - GOOMBA_BBOX_HEIGHT_DIE) / 2.0f;
	bbox_height = GOOMBA_BBOX_HEIGHT_DIE;
	vx = 0.0f;
	vy = 0.0f;
}

void CGoomba::Living(DWORD dt)
{
	Move(dt);

	if(wings)
		Flutter();
}

void CGoomba::Dying(DWORD dt)
{
	die_start += dt;
	if (die_start >= GOOMBA_DIE_TIMEOUT)
	{
		this->Delete();
	}
}

void CGoomba::Refresh()
{
	CEnemy::Refresh();

	maxVx = GOOMBA_VX;
	LookForMario();
	life = GOOMBA_LIFE;	
	if (bornWithWings)
	{	
		GrowWings();
		SetMomentum(0);
	}
	SetState(STATE_LIVE);
}

void CGoomba::GrowWings()
{
	if(wings == nullptr)
		wings = new CWing(x, y - WINGS_Y_OFFSET, 2, WINGS_DISTANCE_BETWEEN);
}

void CGoomba::LoseWings()
{
	if (wings)
	{
		delete wings;
		wings = nullptr;
	}
}

void CGoomba::Flutter()
{
	wings->SetPosition(x, y + WINGS_Y_OFFSET);

	if (vy > 0)
	{
		isOnGround = false;
		if (wings)
			wings->SetState(WINGS_STATE_OPEN);
	}
	else
	{
		isOnGround = (vy == 0);
		if (wings)
			wings->SetState(WINGS_STATE_CLOSE);
	}
}

void CGoomba::ChaseMario(DWORD dt)
{
	if (momentum < 1)
	{
		start_momentum += dt;
		if (isOnGround && start_momentum >= CHASING_DURATION)
		{
			SetMomentum(momentum + 1);
		}
	}
	else if (momentum < CHASING_MAX_MOMENTUM)
	{
		if (isOnGround)
		{
			SetMomentum(momentum + 1);
		}
	}
	else
	{
		if (isOnGround)
		{
			SetMomentum(momentum + 1);
		}
	}
}

void CGoomba::SetMomentum(int m)
{
	if (momentum == m)
	{
		return;
	}

	momentum = m % (CHASING_MAX_MOMENTUM + 1);

	if (momentum < 1)
	{
		LookForMario();
		vx = nx * GOOMBA_VX;
		vy = 0.0f;
		start_momentum = 0;
	}
	else if (momentum < CHASING_MAX_MOMENTUM)
	{
		start_y = y;
		vy = CHASING_VY;
	}
	else
	{
		vy = WINGS_JUMP_VY;
	}
}

void CGoomba::Prepare(DWORD dt)
{
	switch (state)
	{
	case STATE_LIVE:
		if(wings) ChaseMario(dt);
		CMovableObject::Prepare(dt);
		break;
	case STATE_DIE:
		Dying(dt);
		break;
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	switch (state)
	{
	case STATE_LIVE:
		Living(dt);
		break;
	case STATE_DIE:
		break;
	default:
		DefaultUpdate(dt, coObjects);
		break;
	}	
}

void CGoomba::ChangeAnimation()
{
	int object = GetObjectAniID();
	int action;

	switch (state)
	{
	case STATE_DIE:
		action = ANI_ID_GOOMBA_DIE;
		break;
	default:
		action = ANI_ID_GOOMBA_WALK;
		break;
	}

	if (isFliedOut) 
		action += ID_ANI_DIRECTION_UP;

	aniID = object + action;
}

void CGoomba::Render()
{
	if (wings)
		wings->Render();

	ChangeAnimation();
	CAnimations::GetInstance()->Get(aniID)->Render(x, y);
	//RenderBoundingBox();
}