#include "QuestionBlock.h"
#include "Creature.h"

#include "Coin.h"
#include "SuperMushroom.h"
#include "SuperLeaf.h"

#include "Collision.h"
#include "Game.h"
#include "PlayScene.h"

void CQuestionBlock::Render()
{
	aniID = ID_ANI_QUESTION_BLOCK + (state != STATE_LIVE);
	CAnimations::GetInstance()->Get(aniID)->Render(x, y);
}

void CQuestionBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	InPhase(dt, coObjects);
}

void CQuestionBlock::InPhase(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	switch (state)
	{
		case QUESTION_BLOCK_STATE_TOGGLE:
			Shaking(dt);
			break;
	}
}

void CQuestionBlock::Reaction(CGameObject* by_another, int action)
{
	if (state == STATE_DIE)
		return;

	switch (action)
	{
		case ACTION_DESTROY:
			SetState(STATE_DIE);
			break;
		case ACTION_TOUCH:
			SetState(QUESTION_BLOCK_STATE_TOGGLE);
			break;
	}

	TriggerItem();
}

void CQuestionBlock::SetState(int state)
{
	if (this->state == state)
		return;

	this->state = state;

	switch (state)
	{
		case QUESTION_BLOCK_STATE_TOGGLE:
			vy = QUESTION_BLOCK_SHAKE_VY;
			break;
		case STATE_DIE:
			vy = 0.0f;
			break;
	}
}

void CQuestionBlock::TriggerItem()
{
	switch (itemID)
	{
		case OBJECT_TYPE_COIN:
			item = new CCoin(x, y);			
			break;
		case OBJECT_TYPE_SUPER_MUSHROOM:
			item = new CSuperMushroom(x, y);
			break;
		case OBJECT_TYPE_SUPER_LEAF:
			item = new CSuperLeaf(x, y);
			break;
	}

	item->Reaction(this, ACTION_ATTACK);
	LPPLAYSCENE ps = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	ps->Add(item);
}

void CQuestionBlock::Shaking(DWORD dt)
{
	Accelerate(0.0f, GAME_GRAVITY, dt);
	Move(dt);

	if (y >= origin_y)
	{
		y = origin_y;
		SetState(STATE_DIE);
	}
}
