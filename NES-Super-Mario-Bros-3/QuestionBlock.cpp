#include "QuestionBlock.h"

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

void CQuestionBlock::OnReactionTo(LPCOLLISIONEVENT e, int action)
{
	if (state == STATE_DIE)
		return;

	switch (action)
	{
	case ACTION_ATTACK_LEVEL_1:
	case ACTION_ATTACK_LEVEL_2:		
	case ACTION_ATTACK_LEVEL_3:
		SetState(STATE_DIE);
		break;
	case ACTION_TOUCH:
		SetState(QUESTION_BLOCK_STATE_TOGGLE);
		break;
	}

	TriggerItem(e, action);
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

void CQuestionBlock::TakeItem()
{
	if (item)
		return;

	switch (itemID)
	{
	case OBJECT_TYPE_COIN:
	{
		CCoin* coin = new CCoin(x, y);
		coin->SetContained();
		item = coin;
		break;
	}
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

void CQuestionBlock::DetermineItem(CMario* mario)
{
	if (mario->GetLife() < MARIO_LEVEL_BIG)
		itemID = OBJECT_TYPE_SUPER_MUSHROOM;
	else
		itemID = OBJECT_TYPE_SUPER_LEAF;
}

void CQuestionBlock::TriggerItem(LPCOLLISIONEVENT e, int action)
{
	if (item == nullptr)
	{
		LPPLAYSCENE ps = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();

		if (itemID == OBJECT_TYPE_AMBIGUOUS_EFFECT)
		{
			CMario* mario = dynamic_cast<CMario*>(ps->GetPlayer());
			DetermineItem(mario);
		}

		TakeItem();

		ps->Insert(item, ps->Find(this));

		item->OnReactionTo(e, action);

		item = nullptr;
	}
}
