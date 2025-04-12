#include "KoopaTroopa.h"
#include "Platform.h"

#include "debug.h"
#include "PlayScene.h"

CKoopaTroopa::CKoopaTroopa(float x, float y):
	CCreature(x, y)
{	
	bbox_height = KOOPA_BBOX_HEIGHT_FOOT_1;
	bbox_width = KOOPA_BBOX_WIDTH;

	vy = KOOPA_VY;	
	SetState(STATE_LIVE);
	life = KOOPA_LIFE;
}

void CKoopaTroopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{		

	Recover();

	if(!isControl)
	{
		Accelerate(0.0f, GAME_GRAVITY, dt);
	}

	dt = 16;
	CCollision::GetInstance()->Process(this, dt, coObjects);
	Move(dt);
}

void CKoopaTroopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CPlatform*>(e->obj))
	{		
		OnCollisionWithPlatform(e);
	}
	else if (dynamic_cast<CHarmfulObject*>(e->obj))
	{
		OnCollisionWithHarmfulObject(e);
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
		ChangeDirection();
	}
	else if (this->state == STATE_LIVE)
	{
		if (x <= left && vx < 0)
		{
			ChangeDirection();
		}
		else if (x >= right && vx > 0)
		{
			ChangeDirection();
		}
	}
}

void CKoopaTroopa::OnCollisionWithHarmfulObject(LPCOLLISIONEVENT e)
{
	CHarmfulObject* obj = dynamic_cast<CHarmfulObject*>(e->obj);
	MeleeAttack(obj);
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
	bool direction_ignore = false;
	switch (state)
	{
		case STATE_LIVE:
			action = ANI_ID_KOOPA_WALK;
			break;
		case KOOPA_STATE_HIDE:
			action = ANI_ID_KOOPA_HIDE;
			direction_ignore = true;
			break;
		case KOOPA_STATE_POP:
		{
			action = ANI_ID_KOOPA_POP;
			direction_ignore = true;
			aniID = ANI_ID_KOOPA + action;			
			//DWORD timePerFrame = ( (float)remaining_time / KOOPA_POP_TIME) * KOOPA_POP_MAX_FRAME_TIME;			
			CAnimations::GetInstance()->Get(aniID)->ChangeTimePerFrame(15);
			break;
		}
		case KOOPA_STATE_ROLL:
			action = ANI_ID_KOOPA_ROLL;
			break;
		case STATE_DIE:
			break;
	}

	int horizontalDirection = 0;
	if(direction_ignore == false)
		horizontalDirection = (nx <= 0) ? ANI_ID_KOOPA_LEFT : ANI_ID_KOOPA_RIGHT;

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
			LookForMario();
			vx = nx * KOOPA_VX;
			AgainstControl();
			break;
		case KOOPA_STATE_HIDE:
			y += (bbox_height - KOOPA_BBOX_HEIGHT_HIDE) / 2;
			bbox_height = KOOPA_BBOX_HEIGHT_HIDE;
			vx = STOP_V;
			start_time = GetTickCount64();
			break;
		case KOOPA_STATE_POP:
			vx = KOOPA_POP_VX;
			start_time = GetTickCount64();
			break;
		case KOOPA_STATE_ROLL:
			vx = nx * KOOPA_ROLL_VX;			
			break;
		case STATE_DIE:
			break;
	}
}

void CKoopaTroopa::Reaction(CHarmfulObject* by_another, int action)
{	
	switch (state)
	{
		case STATE_LIVE:
		{
			Reaction_LivingState(by_another, action);
			break;
		}
		case KOOPA_STATE_ROLL:
		{
			Reaction_RollingState(by_another, action);
			break;
		}
		case KOOPA_STATE_POP:
		case KOOPA_STATE_HIDE:
		{
			Reaction_HidingState(by_another, action);
		}
		case STATE_DIE:
			break;
	}	
}

void CKoopaTroopa::Reaction_LivingState(CHarmfulObject* by_another, int action)
{
	switch (action)
	{
		case ACTION_ATTACK:
			UnderAttack(by_another);
			SetState(KOOPA_STATE_HIDE);
			break;
		case ACTION_CARRY:
			AgainstControl();
			break;
		default:
			MeleeAttack(by_another);			
	}
}

void CKoopaTroopa::Reaction_RollingState(CHarmfulObject* by_another, int action)
{
	switch (action)
	{
		default:
			MeleeAttack(by_another);
			AgainstControl();
	}
}

void CKoopaTroopa::Reaction_HidingState(CHarmfulObject* by_another, int action)
{
	switch (action)
	{
		case ACTION_CARRY:
			//Powerless;
			break;
		default:
			float mario_x, mario_y;
			by_another->GetPosition(mario_x, mario_y);
			nx = (x < mario_x) ? DIRECTION_LEFT : DIRECTION_RIGHT;
			this->SetState(KOOPA_STATE_ROLL);		
	}
}

void CKoopaTroopa::Recover()
{
	remaining_time = GetTickCount64() - start_time;

	switch (state)
	{
		case KOOPA_STATE_HIDE:
		{
			if (remaining_time >= KOOPA_HIDE_TIME)
			{
				SetState(KOOPA_STATE_POP);
			}
			break;
		}
		case KOOPA_STATE_POP:
		{
			if (remaining_time >= KOOPA_POP_TIME)
			{
				CCreature::Recover();
			}
			else
			{
				remaining_time = KOOPA_POP_TIME - remaining_time;
				vx = -vx;
			}
			break;
		}			
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

const char* CKoopaTroopa::ToString()
{
	return "Koopa";
}
