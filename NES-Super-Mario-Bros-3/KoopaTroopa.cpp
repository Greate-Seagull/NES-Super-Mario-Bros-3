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
	dt = 16;
	InPhase(dt, coObjects);
}

void CKoopaTroopa::InPhase(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	switch (state)
	{
		case STATE_LIVE:
			InPhaseLivingState(dt, coObjects);
			break;
		case KOOPA_STATE_HIDE:
			InPhaseHidingState(dt, coObjects);
			break;
		case KOOPA_STATE_ROLL:
			InPhaseRollingState(dt, coObjects);
			break;
		case KOOPA_STATE_POP:
			InPhasePopingState(dt, coObjects);
			break;
		case STATE_DIE:
			break;
	}
}

void CKoopaTroopa::InPhaseLivingState(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{	
	ThinkOfFalling();

	Accelerate(0.0f, GAME_GRAVITY, dt);
	CCollision::GetInstance()->Process(this, dt, coObjects);

	Patrol();

	Move(dt);
}

void CKoopaTroopa::InPhaseHidingState(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Hide(dt);
	if (!IsControlled())
	{
		Accelerate(0.0f, GAME_GRAVITY, dt);
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
	/*else
	{
		CCollision::GetInstance()->ProcessOverlap(this, coObjects);
	}*/
	Move(dt);
}

void CKoopaTroopa::InPhaseRollingState(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Accelerate(0.0f, GAME_GRAVITY, dt);
	CCollision::GetInstance()->Process(this, dt, coObjects);
	Move(dt);
}

void CKoopaTroopa::InPhasePopingState(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Pop(dt);
	if (!IsControlled())
	{
		Accelerate(0.0f, GAME_GRAVITY, dt);
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
	//else
	//{
	//	CCollision::GetInstance()->ProcessOverlap(this, coObjects);
	//}
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
		switch (state)
		{
			case KOOPA_STATE_HIDE:
				Bounce();
				break;
			case STATE_LIVE:
				AwareOfNotFalling();
			default:
				vy = 0.0f;
				break;
		}
	}

	if (e->nx)
	{
		nx = -nx;
		vx = -vx;
	}
}

void CKoopaTroopa::OnCollisionWithHarmfulObject(LPCOLLISIONEVENT e)
{
	CHarmfulObject* obj = dynamic_cast<CHarmfulObject*>(e->obj);

	switch (state)
	{
		case STATE_LIVE:
			MeleeAttack(obj);
			break;
		case KOOPA_STATE_HIDE:
		case KOOPA_STATE_POP:
			Destroy(obj);
			Die();
			break;
		case KOOPA_STATE_ROLL:
			Destroy(obj);
			break;
		case STATE_DIE:
			break;
	}
}

void CKoopaTroopa::InitiateFallSensor()
{
	start_falling = true;
	is_falling = true;
}

void CKoopaTroopa::ThinkOfFalling()
{
	start_falling = true;
}

void CKoopaTroopa::AwareOfNotFalling()
{
	start_falling = false;
	is_falling = false;
}

void CKoopaTroopa::Falling()
{
	if (start_falling)
	{
		is_falling = true;
	}
}

bool CKoopaTroopa::IsGoingToFall()
{
	return start_falling && is_falling == false;
}

void CKoopaTroopa::Patrol()
{
	if (IsGoingToFall())
	{
		nx = -nx;
		vx = -vx;
		vy = 0;
	}
	else
	{
		Falling();
	}
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
			y += (bbox_height - KOOPA_BBOX_HEIGHT_FOOT_1) / 2;
			bbox_height = KOOPA_BBOX_HEIGHT_FOOT_1;
			LookForMario();
			vx = nx * KOOPA_VX;
			InitiateFallSensor();
			AgainstControl();
			LoseHighPower();
			break;
		case KOOPA_STATE_HIDE:
			y += (bbox_height - KOOPA_BBOX_HEIGHT_HIDE) / 2;
			bbox_height = KOOPA_BBOX_HEIGHT_HIDE;
			recovering_time = 0;
			SetHighPower();
			break;
		case KOOPA_STATE_POP:
			vx = KOOPA_POP_VX;
			break;
		case KOOPA_STATE_ROLL:
			vx = nx * KOOPA_ROLL_VX;
			AgainstControl();
			break;
		case STATE_DIE:
			break;
	}
}

void CKoopaTroopa::Reaction(CGameObject* by_another, int action)
{	
	switch (state)
	{
		case STATE_LIVE:
			ReactionInLivingState(by_another, action);
			break;
		case KOOPA_STATE_ROLL:
			ReactionInRollingState(by_another, action);
			break;
		case KOOPA_STATE_POP:
		case KOOPA_STATE_HIDE:
			ReactionInHidingState(by_another, action);
			break;
		case STATE_DIE:
			break;
	}	
}

void CKoopaTroopa::ReactionInLivingState(CGameObject* by_another, int action)
{
	switch (action)
	{
		case ACTION_ATTACK:
			UnderAttack(by_another);
			SetState(KOOPA_STATE_HIDE);
			break;
		case ACTION_DESTROY:
			UnderAttack(by_another);
			UnderDestructrion(by_another);
			SetState(KOOPA_STATE_HIDE);
			break;
		case ACTION_CARRY:
			AgainstControl();
			MeleeAttack(by_another);
			break;
		default:
			MeleeAttack(by_another);
	}
}

void CKoopaTroopa::ReactionInRollingState(CGameObject* by_another, int action)
{
	switch (action)
	{
		case ACTION_ATTACK:
			if (dynamic_cast<CMario*>(by_another)) SetState(KOOPA_STATE_HIDE);
			break;
		case ACTION_DESTROY:
			UnderDestructrion(by_another);
			SetState(KOOPA_STATE_HIDE);
			break;
		default:
			Destroy(by_another);
			AgainstControl();
	}
}

void CKoopaTroopa::ReactionInHidingState(CGameObject* by_another, int action)
{
	switch (action)
	{
		case ACTION_CARRY:
			//Powerless;
			break;
		case ACTION_DESTROY:
			UnderDestructrion(by_another);
			SetState(KOOPA_STATE_HIDE);
			break;
		default:			
			if (CMario* mario = dynamic_cast<CMario*>(by_another)) //mario attacks
			{
				float mario_x, mario_y;
				mario->GetPosition(mario_x, mario_y);
				nx = (x < mario_x) ? DIRECTION_LEFT : DIRECTION_RIGHT;
				this->SetState(KOOPA_STATE_ROLL);

				DoPowerless(mario);
			}
			else
			{
				Destroy(by_another);
				Die();
			}
	}
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
		vy = KOOPA_BOUNCE_VY;
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