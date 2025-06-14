#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"
#include "PlayScene.h"

#include "Goomba.h"
#include "Coin.h"
#include "Brick.h"
#include "BrickParticle.h"
#include "Portal.h"
#include "FloatingPlatform.h"
#include "SuperMushroom.h"
#include "SuperLeaf.h"
#include "Background.h"
#include "Pipe.h"

#include "Collision.h"

bool isOnPlatform = false;

float switchSceneTime = 0;
#define MAX_SCENE_TIME 800

float exitPipeTime = 0;
bool finishedExitingPipe = false;
#define MAX_EXIT_PIPE_TIME 800

int sceneDestination;

int flyingPoint = 100;

CMario::CMario(float x, float y):
	CCreature(x, y)
{
	SetState(STATE_LIVE);
	SetLife(MARIO_LEVEL_SMALL);

	coins = 0;
	scores = 0;
	cardIndex = 0;
}

void CMario::Prepare(DWORD dt)
{
	switch (state)
	{
	case STATE_LIVE:
		StartSpecialActions();
		StartNormalActions(dt);
		break;
	case MARIO_STATE_GAIN_POWER:
		GainingPower(dt);
		break;
	case MARIO_STATE_LOSE_POWER:
		LosingPower(dt);
		break;
	case STATE_DIE:		
		changing_state_time += dt;
		if (changing_state_time >= MARIO_DYING_TIME) CMovableObject::Prepare(dt);
		break;
	}
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{	
	switch (state)
	{
	case STATE_LIVE:
		if (isFastTravel) vx = FINISHED_RUN_SPEED;
		Living(dt, coObjects);
		break;
	case STATE_DIE:
		Dying(dt);
		break;
	}	

	if (this->isOnGround) flyingPoint = 100;
}

void CMario::Living(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Move(dt);

	DoSpecialActions(dt, coObjects);

	UpdateMomentum(dt);
	Invulnerable(dt);	
}

int CMario::IsLinkedTo(CGameObject* obj)
{
	return obj == weapon || obj == tail;
}

void CMario::OnNoCollisionWithBlocking(DWORD dt)
{
	isOnGround = false;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{	
	if (dynamic_cast<CPlatform*>(e->obj))
		OnCollisionWithPlatform(e);
	else if (dynamic_cast<CBlock*>(e->obj))
		OnCollisionWithBlock(e);
	else if (dynamic_cast<CHarmfulObject*>(e->obj))
	{
		if (!is_invulnerable) OnCollisionWithHarmfulObject(e);
	}
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CHelpfulObject*>(e->obj))
		OnCollisionWithHelpfulObject(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CReward*>(e->obj))
		OnCollisionWithReward(e);
	/*else if (dynamic_cast<CDeadStateTrigger*>(e->obj))
		OnCollisionWithDeadTrigger(e);*/
}

void CMario::OnReactionToTouching(LPCOLLISIONEVENT e)
{
	KeyStateManager* keyState = CGame::GetInstance()->GetKeyboard();

	if (dynamic_cast<CEnemy*>(e->src_obj))
	{
		e->Reverse();

		if (e->ny < 0) BackJump();
		else if (keyState->IsHold(VK_A)) Grab(e);
		else Kick();
	}
	else if (dynamic_cast<CCoin*>(e->src_obj))
		coins++;
	else if (CPipe* pipe = dynamic_cast<CPipe*>(e->src_obj))
	{
		destination = pipe->GetDestination();
		des_x = pipe->GetDestinationX();
		des_y = pipe->GetDestinationY();
		IntoThePipe(e->ny);
	}
}

void CMario::OnReactionToAttack1(LPCOLLISIONEVENT e)
{
	UnderAttack(e->src_obj);
}

void CMario::OnReactionToAttack2(LPCOLLISIONEVENT e)
{
	UnderAttack(e->src_obj);
}

void CMario::OnReactionToAttack3(LPCOLLISIONEVENT e)
{
	UnderAttack(e->src_obj);
}

void CMario::OnReactionToBigger(LPCOLLISIONEVENT e)
{
	if (life < MARIO_LEVEL_RACOON)
		SetState(MARIO_STATE_GAIN_POWER);
}

void CMario::OnReactionToRacoonize(LPCOLLISIONEVENT e)
{
	if(life < MARIO_LEVEL_RACOON)
		SetState(MARIO_STATE_GAIN_POWER);
}

void CMario::OnCollisionWithHarmfulObject(LPCOLLISIONEVENT e)
{
	KeyStateManager* keyState = CGame::GetInstance()->GetKeyboard();
	CHarmfulObject* enemy = dynamic_cast<CHarmfulObject*>(e->obj);

	if (e->ny < 0) CHarmfulObject::Attack(e);		
	else if (keyState->IsHold(VK_A) && Grab(e)); //case collision on the left or right or below
	else Touch(e);
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	Touch(e);
	coins++;
	InsertScore(50);
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

void CMario::OnCollisionWithPlatform(LPCOLLISIONEVENT e)
{
	isInGround = false;

	if (e->ny)
	{
		//vy = 0.0f;

		if (e->ny < 0)
		{
			fly_cooldown = INT_MAX;
			isOnGround = true;
			is_flying = false;
			on_ground_y = y;
		}

		float vx;
		e->obj->GetSpeed(vx, this->vy);
		//this->vy *= 1.0f - e->t;

		Touch(e);
	}

	if (e->nx)
	{
		//vx = 0.0f;

		float vy;
		e->obj->GetSpeed(this->vx, vy);
		//this->vx *= 1.0f - e->t;
	}	
}

void CMario::OnCollisionWithHelpfulObject(LPCOLLISIONEVENT e)
{
	Touch(e);
}

void CMario::OnCollisionWithBlock(LPCOLLISIONEVENT e)
{
	isInGround = false;

	if (e->ny)
	{
		//vy = 0.0f;

		if (e->ny > 0)
		{			
			is_jumping = false;
			on_ceil_y = y;
		}
		else
		{
			fly_cooldown = INT_MAX;
			isOnGround = true;
			is_flying = false;
			on_ground_y = y;
		}

		float vx;
		e->obj->GetSpeed(vx, this->vy);
		//this->vy *= 1.0f - e->t;

		Touch(e);
	}

	if (e->nx)
	{
		//vx = 0.0f;

		float vy;
		e->obj->GetSpeed(this->vx, vy);
		//this->vx *= 1.0f - e->t;
	}
}

void CMario::OnCollisionWithReward(LPCOLLISIONEVENT e)
{
	if (!isFastTravel)
	{
		CReward* reward = (CReward*)(e->obj);
		cardIndex %= CARD_COUNT;
		cards[cardIndex++] = reward->GetType();
		isFastTravel = true;

		LPPLAYSCENE currentScene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
		currentScene->PushFinishWall();

		Touch(e);
	}
}

void CMario::Render()
{
	if (is_invulnerable && changing_state_time % 120 < 60)
		return;

	//Determine animation
	ChangeAnimation();
	
	//Determine draw position
	float draw_x = x;
	float draw_y = y;

	switch (state)
	{
		case STATE_LIVE:
			if(life == MARIO_LEVEL_RACOON) ChangeDrawX(draw_x);
			break;
		case MARIO_STATE_GAIN_POWER:
			if (life == MARIO_LEVEL_SMALL) ChangeDrawY(draw_y);
			break;
		case MARIO_STATE_LOSE_POWER:
			if (life == MARIO_LEVEL_BIG) ChangeDrawY(draw_y);
			break;
	}		

	//Render
	CAnimations::GetInstance()->Get(aniID)->Render(draw_x, draw_y, true);
}

void CMario::ChangeAnimation()
{
	int level = ID_ANI_SMALL;
	if (life == MARIO_LEVEL_RACOON)
	{
		level = ID_ANI_RACOON;
	}
	else if (life == MARIO_LEVEL_BIG)
	{
		level = ID_ANI_BIG;
	}
	
	int action;
	DWORD timePerFrame = TIME_FRAME;
	switch (state)
	{
		case STATE_LIVE:
			ChangeAnimationInLivingState(action, timePerFrame);
			break;
		case MARIO_STATE_GAIN_POWER:
			action = ID_ANI_LEVEL_UP;
			break;
		case MARIO_STATE_LOSE_POWER:
			action = ID_ANI_LEVEL_DOWN;						
			break;
		case STATE_DIE:
			action = ID_ANI_DIE;
			break;
	}
	
	int direction = 0;
	if (nz)
	{
		direction = (nz < 0) ? ID_ANI_FRONT : ID_ANI_BEHIND;
	}
	else
	{
		direction = (nx <= 0) ? ID_ANI_LEFT : ID_ANI_RIGHT;
	}

	aniID = ID_ANI_MARIO + level + action + direction;

	if(timePerFrame < TIME_FRAME)
		CAnimations::GetInstance()->Get(aniID)->ChangeTimePerFrame(timePerFrame);
	
	CAnimations::GetInstance()->Get(aniID)->SwitchSprite();
}

void CMario::ChangeAnimationInLivingState(int &actionID, DWORD &timePerFrame)
{
	if (isFastTravel && !isInGround)
	{
		actionID = ID_ANI_FINISHED;
		return;
	}

	if (vy > 0 && life > MARIO_LEVEL_SMALL) //Falling
	{		
		if (life >= MARIO_LEVEL_RACOON && fly_cooldown < MARIO_FLY_COOLDOWN && !weapon)
			actionID = ID_ANI_PARACHUTE;
		else if (is_boosting && !weapon)
			actionID = ID_ANI_SUPER_FALL;
		else
			actionID = ID_ANI_FALL;
	}
	else if (vy && isOnGround == false) //Jumping
	{
		if (is_boosting && !weapon)
		{
			if (is_flying && fly_cooldown < MARIO_FLY_COOLDOWN)
				actionID = ID_ANI_FLY;
			else
				actionID = ID_ANI_SUPER_JUMP;
		}
		else
			actionID = ID_ANI_JUMP;
	}
	else if (vx == 0) //Idle
		actionID = ID_ANI_IDLE;
	else if (vx * nx < 0 && !weapon) //Braking
		actionID = ID_ANI_BRACE;
	else //Running
	{
		if (fabs(vx) >= MARIO_SMALL_RUNNING_MAX_VX && !weapon)
			actionID = ID_ANI_SUPER_RUN;
		else
		{
			actionID = ID_ANI_RUN;
			timePerFrame *= (MARIO_SMALL_RUNNING_MAX_VX - abs(vx)) / MARIO_SMALL_RUNNING_MAX_VX;
		}
	}

	//for special actions
	switch (special_action)
	{
	case ID_ANI_IDLE:
	case ID_ANI_CARRY:
		actionID += special_action;
		break;
	default:
		actionID = special_action;
		break;
	}
}

void CMario::ChangeDrawX(float& x)
{
	LPANIMATION_FRAME currentFrame = CAnimations::GetInstance()->Get(aniID)->GetCurrentFrame();

	LPSPRITE sprite = currentFrame->GetSprite();
	float sprite_width = (float)sprite->GetWidth();

	x = this->x + nx * (bbox_width - sprite_width) / 2.0f;
}

void CMario::ChangeDrawY(float& y)
{
	LPANIMATION_FRAME currentFrame = CAnimations::GetInstance()->Get(aniID)->GetCurrentFrame();

	LPSPRITE sprite = currentFrame->GetSprite();
	float sprite_height = (float)sprite->GetHeight();

	y = this->y + (bbox_height - sprite_height) / 2.0f;
}

bool CMario::SetSpecialAction(int actionID)
{
	//validation
	if (special_action == actionID)
		return false;	
	
	switch (actionID)
	{
	case ID_ANI_SIT:
		if (life < MARIO_LEVEL_BIG) return false;
		break;
	case ID_ANI_ATTACK:
		if (life < MARIO_LEVEL_RACOON) return false;
		break;
	}

	//cancel previous action
	switch (special_action)
	{
	case ID_ANI_SIT:
		Stand();
		break;
	case ID_ANI_ATTACK:
		UntriggerTail();
		break;
	case ID_ANI_CARRY:
		Drop();
		break;
	case ID_ANI_INTO_THE_PIPE:
		ny = 0; nz = 0;
		vx = nx * vx; vy = ny * vy;
		break;
	}

	//apply new action
	special_action = actionID;
	return true;
}

void CMario::StartSpecialActions()
{
	if (special_action == ID_ANI_INTO_THE_PIPE)
	{
		isDigging = false;
		return;
	}

	KeyStateManager* keyState = CGame::GetInstance()->GetKeyboard();

	if (keyState->IsPressed(VK_UP))
		SetLife(life + 1.0f);
	/*else if (keyState->IsPressed(VK_DOWN))
		SetLife(life - 1.0f);*/
	
	if (keyState->IsHold(VK_A))
		Run();
	else
		Walk();

	if (keyState->IsReleased(VK_A))
		Tosh();
	else if (special_action == ID_ANI_KICK || special_action == ID_ANI_CARRY);
	else if (keyState->IsHold(VK_DOWN))
		Sit();
	else if (keyState->IsPressed(VK_A))
		Attack();
}

void CMario::DoSpecialActions(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	switch (special_action)
	{
	case ID_ANI_KICK:
		Kicking(dt);
		break;
	case ID_ANI_ATTACK:
		Attacking(dt);
		break;
	case ID_ANI_CARRY:
		Carrying();
		break;
	case ID_ANI_SIT:
		Sitting();
		break;
	case ID_ANI_INTO_THE_PIPE:
		Digging();
		break;
	}
}

void CMario::StartNormalActions(DWORD& t)
{
	if (special_action == ID_ANI_INTO_THE_PIPE)
		return;

	KeyStateManager* keyState = CGame::GetInstance()->GetKeyboard();
	
	float calculated_ax = 0.0f;
	float calculated_ay = GAME_GRAVITY;
	int new_nx = 0;

	//Go left or right
	if (keyState->IsHold(VK_LEFT))
	{
		if (vx <= 0)
		{
			calculated_ax += -ax;
		}
		else
		{		
			float temp_ax = min(MARIO_BRAKE_AX, fabs(vx / t));
			calculated_ax += -temp_ax;
		}

		new_nx += DIRECTION_LEFT;
	}
	if (keyState->IsHold(VK_RIGHT))
	{
		if (vx >= 0)
		{
			calculated_ax += ax;
		}
		else
		{
			float temp_ax = min(MARIO_BRAKE_AX, fabs(vx / t));
			calculated_ax += temp_ax;
		}

		new_nx += DIRECTION_RIGHT;
	}
	
	if (is_flying && keyState->IsPressed(VK_S)) //Flying
	{
		ny = DIRECTION_UP;

		vy = (vy > 0) ? vy : MARIO_FLY_V;
		vy = min(fabs(vy), MARIO_FLY_V);
		vy *= ny;

		vx = min(fabs(vx), MARIO_FLY_V);
		vx *= nx;

		maxVx = MARIO_FLY_V;

		fly_cooldown = 0;

		is_jumping = false;
	}	
	else if (isOnGround && keyState->IsPressed(VK_S)) //Jumping
	{		
		ny = DIRECTION_UP;
		vy = ny * MARIO_START_JUMP_VY;
		calculated_ay = 0.0f;

			is_jumping = true;

		Fly();		
	}
	else if (is_jumping && fabs(y - on_ground_y) < MARIO_MAX_JUMP_HEIGHT && keyState->IsHold(VK_S)) //Jumping
	{
		if (fabs(vy) <= MARIO_SMALL_JUMPING_MAX_VY)
			calculated_ay = 0.0f;
		//else calculated_ay = GAME_GRAVITY;
	}
	else //Falling
	{
		if (life == MARIO_LEVEL_RACOON && keyState->IsPressed(VK_S))
		{
			vy = 0.05f;
			fly_cooldown = 0;
		}

		//calculated_ay = GAME_GRAVITY;

		ny = DIRECTION_DOWN;

		is_jumping = false;
	}

	if (is_flying) 
	{
		total_fly_time += t;	
		if (total_fly_time >= MARIO_FLY_TIME)
			is_flying = false;		
	}

	//Remove ay when flying
	fly_cooldown += t;
	if (fly_cooldown < MARIO_FLY_COOLDOWN)
		calculated_ay = 0.0f;

	//decelerate
	if(new_nx)
		this->nx = new_nx;
	else if (new_nx == 0 && isOnGround)
	{
		calculated_ax = min(MARIO_DECELERATE_AX, fabs(vx) / t);

		if (vx > 0)
			calculated_ax = -calculated_ax;
	}

	//
	ax = calculated_ax;
	ay = calculated_ay;
	Accelerate(calculated_ax, calculated_ay, t);
}

void CMario::Accelerate(float ax, float ay, DWORD t)
{
	CMovableObject::Accelerate(ax, ay, t);

	if (vx > maxVx)
	{
		vx = maxVx;
	}
	else if (vx < -maxVx)
	{
		vx = -maxVx;
	}
}

void CMario::UnderAttack(CGameObject* by_another)
{
	if (is_invulnerable)
		return;

	SetState(MARIO_STATE_LOSE_POWER);
}

bool CMario::Sit()
{
	if (SetSpecialAction(ID_ANI_SIT) == false)
		return false;

	y += (bbox_height - MARIO_BIG_SITTING_BBOX_HEIGHT) / 2.0f;
	bbox_height = MARIO_BIG_SITTING_BBOX_HEIGHT;

	return true;
}

void CMario::Sitting()
{
	KeyStateManager* keyState = CGame::GetInstance()->GetKeyboard();

	if (keyState->IsReleased(VK_DOWN) || keyState->IsHold(VK_LEFT) || keyState->IsHold(VK_RIGHT))
		SetSpecialAction(ID_ANI_IDLE);
}

bool CMario::Stand()
{
	float stand_bbox = (life < MARIO_LEVEL_BIG) ? MARIO_SMALL_BBOX_HEIGHT : MARIO_BIG_BBOX_HEIGHT;
	y += (bbox_height - stand_bbox) / 2.0f;
	bbox_height = stand_bbox;

	return true;
}

bool CMario::Run()
{
	ax = MARIO_SMALL_RUNNING_AX;

	if (isOnGround == false)
	{
		maxVx = max(fabs(vx), MARIO_SMALL_RUNNING_MAX_VX / 1.5f);
		maxVx = min(maxVx, MARIO_SMALL_RUNNING_MAX_VX);
	}
	else
		maxVx = MARIO_SMALL_RUNNING_MAX_VX;

	return true;
}

void CMario::UpdateMomentum(DWORD dt)
{
	if (is_flying)
		return;

	int momentum_increase = max(momentum, (int)((fabs(vx) - MARIO_SMALL_WALKING_MAX_VX) / MARIO_VX_BAND));

	if (ax * vx > 0 && (momentum < momentum_increase || momentum_increase == MARIO_MAX_MOMENTUM) && isOnGround)
	{
		momentum = momentum_increase;
		momentum = min(momentum, MARIO_MAX_MOMENTUM);
		decrease_momentum_time = 0;
	}
	else
	{
		decrease_momentum_time += dt;
		if (decrease_momentum_time >= MARIO_MOMENTUM_TIME)
		{
			momentum = max(0, momentum - 1);
			decrease_momentum_time %= dt;
		}
	}

	is_boosting = fabs(vx) == MARIO_SMALL_RUNNING_MAX_VX;
}

bool CMario::Walk()
{
	ax = MARIO_SMALL_RUNNING_AX;

	if (abs(vx) <= MARIO_SMALL_WALKING_MAX_VX)
	{		
		maxVx = MARIO_SMALL_WALKING_MAX_VX;
	}
	else
	{
		ax = -MARIO_DECELERATE_AX;
	}

	return true;
}

bool CMario::Attack()
{
	if (SetSpecialAction(ID_ANI_ATTACK) == false)
		return false;

	attacking_time = 0;
	attack_phase = -1;
	attack_direction = -nx;
	tail->SetPosition(x, y + MARIO_RACOON_TAIL_Y_OFFSET);
	tail->Refresh();

	return true;
}

void CMario::Attacking(DWORD dt)
{
	//Switching phase
	attacking_time += dt;
	ChangeAttackPhase(attacking_time / MARIO_ATTACK_PHASE_TIME);
	nx = -attack_direction;
	
	//Tail's position should be calculated after processing attacking phase to synchronize with the animation
	if (special_action == ID_ANI_ATTACK) // if attack hasn't ended
	{
		float tail_x, tail_y;
		tail->GetPosition(tail_x, tail_y);
		float newTail_x = x + attack_direction * (bbox_width + tail->GetBBoxWidth()) / 2.0f;
		float newTail_y = y + MARIO_RACOON_TAIL_Y_OFFSET;

		tail->SetMoveDistance(newTail_x - tail_x, newTail_y - tail_y);
	}
}

void CMario::ChangeAttackPhase(int phase)
{
	if (this->attack_phase == phase)
	{
		return;
	}

	this->attack_phase = phase;	

	if (phase == 0)
	{
		nz = 0;
	}
	else if (phase == 1)
	{
		nz = DIRECTION_FRONT;
	}
	else if (phase == 2)
	{
		nz = 0;
		attack_direction *= -1;
	}
	else if (phase == 3)
	{
		nz = DIRECTION_BEHIND;
	}
	else if (phase == 4)
	{
		nz = 0;
		attack_direction *= -1;
	}
	else
	{
		SetSpecialAction(ID_ANI_IDLE);
	}	
}

void CMario::UntriggerTail()
{
	tail->SetPosition(DEFAULT_X, DEFAULT_Y);
	nz = 0;
}

void CMario::BackJump()
{
	fly_cooldown = INT_MAX;
	is_jumping = true;

	vy = MARIO_JUMP_DEFLECT_VY;
	ny = DIRECTION_UP;
}

void CMario::Jump()
{
}

bool CMario::Grab(LPCOLLISIONEVENT e)
{
	if (this->weapon)
		return false;

	CCreature::Carry(e);

	if (this->weapon)
		SetSpecialAction(ID_ANI_CARRY);

	return true;
}

void CMario::Carrying()
{
	if (weapon)
	{
		float weapon_x, weapon_y;

		weapon_x = x + nx * MARIO_CARRY_OFFSET_X;
		weapon_y = y + (bbox_height - weapon->GetBBoxHeight() - 2.0f) / 4.0f;

		weapon->SetPosition(weapon_x, weapon_y);
	}
	else
		SetSpecialAction(ID_ANI_IDLE);
}

bool CMario::Tosh()
{
	if (!weapon)
		return false;

	//PIECE OF SHIT, I just need an event
	CCollisionEventPool* pool = CCollision::GetInstance()->GetPool();
	LPCOLLISIONEVENT e = pool->Allocate(0.0f, nx, ny, nx, ny, weapon, this);
	Touch(e);
	pool->VirtualDelete();

	return Kick();
}

void CMario::Drop()
{
	if(weapon)
		weapon->AgainstControl();
}

bool CMario::Kick()
{
	SetSpecialAction(ID_ANI_KICK);

	attacking_time = 0;	

	return true;
}

void CMario::Kicking(DWORD dt)
{
	attacking_time += dt;
	if (attacking_time >= MARIO_KICK_TIME)
	{
		SetSpecialAction(ID_ANI_IDLE);
	}
}

void CMario::StartInvulnerable()
{
	is_invulnerable = true;
	changing_state_time = 0;
}

void CMario::Invulnerable(DWORD dt)
{
	if (!is_invulnerable)
		return;

	changing_state_time += dt;
	if (changing_state_time >= MARIO_INVULNERABLE_TIME)
	{
		is_invulnerable = false;
	}
}

void CMario::GainingPower(DWORD dt)
{
	changing_state_time += dt;
	if (changing_state_time >= action_duration)
	{
		SetLife(life + 1.0f);
		SetState(STATE_LIVE);
	}
}

void CMario::LosingPower(DWORD dt)
{
	changing_state_time += dt;
	if (changing_state_time > action_duration)
	{
		SetLife(life - 1.0f);
		SetState(STATE_LIVE);
		StartInvulnerable();
	}
}

void CMario::CancelUniqueAction()
{
	if (life == MARIO_LEVEL_RACOON)
	{
		is_flying = false;
		switch (special_action)
		{
		case ID_ANI_ATTACK:
			SetSpecialAction(ID_ANI_IDLE);
			break;
		}
	}
	else if (life == MARIO_LEVEL_BIG)
	{
		switch (special_action)
		{
		case ID_ANI_SIT:
			SetSpecialAction(ID_ANI_IDLE);
			break;
		}
	}
}

void CMario::GetBoundingBoxOfAction(float& width, float& height)
{
	switch (special_action)
	{
	case ID_ANI_SIT:
		width = MARIO_BIG_BBOX_WIDTH;
		height = MARIO_BIG_SITTING_BBOX_HEIGHT;
		break;
	default:
		if (life > MARIO_LEVEL_SMALL)
		{
			width = MARIO_BIG_BBOX_WIDTH;
			height = MARIO_BIG_BBOX_HEIGHT;
		}
		else
		{
			width = MARIO_SMALL_BBOX_WIDTH;
			height = MARIO_SMALL_BBOX_HEIGHT;
		}
	}
}

void CMario::SetLife(float l)
{
	// Adjust position to avoid falling off platform
	if (life == l)
		return;

	life = l;

	float new_bbox_width, new_bbox_height;
	GetBoundingBoxOfAction(new_bbox_width, new_bbox_height);
	y += (bbox_height - new_bbox_height) / 2.0f - 1.0f;
	SetBoundingBox(new_bbox_width, new_bbox_height);

	if(life <= 0.0f)
	{
		SetState(STATE_DIE);
	}
}

void CMario::ToGainingPowerState()
{
	this->state = MARIO_STATE_GAIN_POWER;

	changing_state_time = 0;

	if (life == MARIO_LEVEL_SMALL)
	{
		action_duration = MARIO_BIG_TRANSFORM_TIME;
	}
	else if (life == MARIO_LEVEL_BIG)
	{
		nz = DIRECTION_FRONT; //for changing level up animation
		action_duration = MARIO_RACOON_TRANSFORM_TIME;
	}
}

void CMario::ToLosingPowerState()
{
	this->state = MARIO_STATE_LOSE_POWER;

	changing_state_time = 0;

	CancelUniqueAction();

	if (life == MARIO_LEVEL_RACOON)
	{
		nz = DIRECTION_FRONT; //for changing level down animation
		action_duration = MARIO_RACOON_TRANSFORM_TIME;
	}
	else if (life == MARIO_LEVEL_BIG)
	{
		action_duration = MARIO_SMALL_TRANSFORM_TIME;
	}
	else //Only for small mario
	{
		//SetLevel(life - 1.0f);
		Die();
	}	
}

void CMario::ToLivingState()
{
	//Clear previous state
	switch (state)
	{
	case MARIO_STATE_GAIN_POWER:
	case MARIO_STATE_LOSE_POWER:
		nz = 0;
		break;
	case STATE_DIE:
		break;
	}

	this->state = STATE_LIVE;
}

void CMario::ToDyingState()
{
	this->state = STATE_DIE;

	nx = 0;
	ny = DIRECTION_UP;
	nz = 0;

	vx = 0.0f;
	vy = ny * ATTACK_BOOM_VY;

	ax = 0.0f;
	ay = GAME_GRAVITY;

	changing_state_time = 0;
}

void CMario::Dying(DWORD dt)
{
	if (changing_state_time >= MARIO_DYING_TIME)
		Move(dt);
}

void CMario::Fly()
{
	if (life != MARIO_LEVEL_RACOON)
		return;

	if (is_boosting)
	{
		is_flying = true;
		total_fly_time = 0;
	}
}

bool CMario::IntoThePipe(int direction)
{
	isDigging = true;

	SetSpecialAction(ID_ANI_INTO_THE_PIPE);

	ny = direction ? direction : ny;
	nz = DIRECTION_FRONT;

	vx = 0.0f;
	vy = ny * MARIO_PIPE_ENTRY_SPEED;

	return true;
}

void CMario::Digging()
{
	if (isInGround == false)
	{
		float pipe_gate_y = ny > 0 ? on_ground_y : on_ceil_y;
		if (fabs(y - pipe_gate_y) >= bbox_height)
		{
			isOnGround = false;
			isInGround = true;
			isFastTravel = true;
		}
	}

	if (isDigging == false)
	{
		SetSpecialAction(ID_ANI_IDLE);
		isOnGround = true;
		isInGround = false;
	}
}

void CMario::Refresh()
{
	CCreature::Refresh();

	isFastTravel = false;

	is_invulnerable = false;
	is_jumping = false;
	is_flying = false;

	fly_cooldown = INT_MAX;

	decrease_momentum_time = 0;
	momentum = 0;

	SetSpecialAction(ID_ANI_IDLE);
}

void CMario::CreateItem(CPlayScene* ps)
{
	tail = new CRacoonTail(DEFAULT_X, DEFAULT_Y);
	tail->Refresh();
	tail->AcceptOwner(this);
	tail->CreateItem(ps);
	ps->Insert(tail, -1);
}

void CMario::SetState(int state)
{
	if (this->state == state)
	{
		return;
	}

	switch (state)
	{
		case MARIO_STATE_GAIN_POWER:
			ToGainingPowerState();
			break;
		case MARIO_STATE_LOSE_POWER:
			ToLosingPowerState();
			break;
		case STATE_LIVE:
			ToLivingState();
			break;
		case STATE_DIE:
			ToDyingState();			
			break;
		default: this->state = state;
	}
}

void CMario::InsertScore(int score)
{
	scores += score;
}

void CMario::InsertScoreObject(float x, float y, int score)
{
	InsertScore(score);

	CScore* scoreObj = new CScore(x, y, score);
	LPPLAYSCENE curr = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	curr->Insert(scoreObj, curr->Find(this) - 1);
}

void CMario::IncreaseFlyingPoint()
{
	switch (flyingPoint)
	{
		case 100: flyingPoint = 200; break;
		case 200: flyingPoint = 400; break;
		case 400: flyingPoint = 800; break;
		case 800: flyingPoint = 1000; break;
		case 1000: flyingPoint = 2000; break;
		case 2000: flyingPoint = 4000; break;
		case 4000: flyingPoint = 8000; break;
		case 8000: flyingPoint = 10000; break;
		default: break;
	}
}

void CMario::InsertFlyingScore(float x, float y)
{
	InsertScoreObject(x, y, flyingPoint);
	IncreaseFlyingPoint();
}