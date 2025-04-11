#include "KoopaTroopa.h"
#include "Platform.h"

#include "debug.h"

CKoopaTroopa::CKoopaTroopa(float x, float y):
	CCreature(x, y, KOOPA_LIFE)
{
	bbox_height = KOOPA_BBOX_HEIGHT_FOOT_1;
	bbox_width = KOOPA_BBOX_WIDTH;

	vx = KOOPA_VX;
	vy = KOOPA_VY;

	nx = DIRECTION_LEFT;
}

void CKoopaTroopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{		
	dt = 16;
	Accelerate(0.0f, GAME_GRAVITY, dt);
	CCollision::GetInstance()->Process(this, dt, coObjects);
	Move(dt);	
	DebugOutTitle(L"vx: %f", vx);
}

void CKoopaTroopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CPlatform*>(e->obj))
	{		
		OnCollisionWithPlatform(e);
	}
}

void CKoopaTroopa::OnCollisionWithPlatform(LPCOLLISIONEVENT e)
{
	if (e->ny)
	{
		vy = 0;
	}

	float left, top, right, bot;
	e->obj->GetBoundingBox(left, top, right, bot);	

	if (e->nx)
	{
		vx = -vx;
	}
	else if (this->state == STATE_LIVE)
	{
		if (x <= left && vx < 0)
		{
			vx = -vx;
		}
		else if (x >= right && vx > 0)
		{
			vx = -vx;
		}
	}
}

void CKoopaTroopa::Patrol()
{

}

void CKoopaTroopa::Render()
{
	ChangeAnimation();

	CAnimations::GetInstance()->Get(aniID)->Render(x, y);
	RenderBoundingBox();
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
		case KOOPA_STATE_ROLL:
			action = ANI_ID_KOOPA_ROLL;
			break;
		case STATE_DIE:
			break;
	}

	int horizontalDirection = (vx <= 0) ? ANI_ID_KOOPA_LEFT : ANI_ID_KOOPA_RIGHT;

	aniID = ANI_ID_KOOPA + action + horizontalDirection;
}

void CKoopaTroopa::SetState(int state)
{
	if (this->state == state)
	{
		return;
	}

	this->state = state;

	switch (state)
	{
		case STATE_LIVE:
			y += (bbox_height - KOOPA_BBOX_HEIGHT_FOOT_1) / 2;
			bbox_height = KOOPA_BBOX_HEIGHT_FOOT_1;
			vx = nx * KOOPA_VX;
			break;
		case KOOPA_STATE_HIDE:
			y += (bbox_height - KOOPA_BBOX_HEIGHT_HIDE) / 2;
			bbox_height = KOOPA_BBOX_HEIGHT_HIDE;
			vx = 0;
			break;
		case KOOPA_STATE_ROLL:
			vx = nx * KOOPA_ROLL_VX;			
			break;
		case STATE_DIE:
			break;
	}
}

void CKoopaTroopa::UnderAttack(CHarmfulObject* by_another)
{
	switch (state)
	{
		case STATE_LIVE:
		{
			life -= 1;
			this->SetState(KOOPA_STATE_HIDE);
			break;
		}
		case KOOPA_STATE_HIDE:
		{
			float mario_x, mario_y;
			by_another->GetPosition(mario_x, mario_y);

			if (x < mario_x)
			{				
				nx = DIRECTION_LEFT;
			}
			else
			{
				nx = DIRECTION_RIGHT;
			}			
			this->SetState(KOOPA_STATE_ROLL);
			break;
		}
		case STATE_DIE:
			break;
	}
}
