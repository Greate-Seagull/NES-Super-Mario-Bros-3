#include "QuestionBlock.h"

#include "Coin.h"
#include "SuperMushroom.h"
#include "SuperLeaf.h"

#include "PlayScene.h"

void CQuestionBlock::Render()
{
	aniID = ID_ANI_QUESTION_BLOCK + (state != STATE_LIVE);
	CAnimations::GetInstance()->Get(aniID)->Render(x, y);
}

void CQuestionBlock::Prepare(DWORD dt)
{
	switch (state)
	{
	case QUESTION_BLOCK_STATE_TOGGLE:
		CMovableObject::Prepare(dt);
		break;
	}
}

void CQuestionBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	switch (state)
	{
	case QUESTION_BLOCK_STATE_TOGGLE:
		Move(dt);
		if (y >= origin_y)
		{
			y = origin_y;
			SetState(STATE_DIE);
		}
		break;
	}
}

void CQuestionBlock::Reaction(CGameObject* by_another, int action)
{
	if (state == STATE_DIE)
		return;

	switch (action)
	{
	case ACTION_ATTACK_LEVEL_2:		
	case ACTION_ATTACK_LEVEL_3:
		SetState(STATE_DIE);
		break;
	case ACTION_TOUCH:
		SetState(QUESTION_BLOCK_STATE_TOGGLE);
		if (itemID == OBJECT_TYPE_COIN)
		{
			LPPLAYSCENE currentScene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
			currentScene->CollectCoin();
		}
		break;
	}	
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
			TriggerItem();
			break;
		case STATE_DIE:
			vy = 0.0f;
			TriggerItem();
			break;
	}
}

void CQuestionBlock::TakeItem()
{
	if (item)
		return;

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
	default:
		item = nullptr;
	}
}

void CQuestionBlock::TriggerItem()
{
	if (item)
	{
		LPPLAYSCENE ps = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
		ps->Insert(item, ps->Find(this));

		item->Reaction(this, ACTION_TOUCH);

		item = nullptr;
	}
}
