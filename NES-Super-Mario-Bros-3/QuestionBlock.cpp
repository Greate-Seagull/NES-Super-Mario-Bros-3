#include "QuestionBlock.h"
#include "Collision.h"

void CQuestionBlock::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state == STATE_QUESTION_BLOCK_ON)
	{
		animations->Get(ID_ANI_QUESTION_BLOCK_TOGGLE_ON)->Render(x, y);
	}
	else if (state == STATE_QUESTION_BLOCK_OFF)
	{
		animations->Get(ID_ANI_QUESTION_BLOCK_TOGGLE_OFF)->Render(x, y);
	}
}

void CQuestionBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - QUESTION_BLOCK_BBOX_WIDTH / 2;
	t = y - QUESTION_BLOCK_BBOX_HEIGHT / 2;
	r = l + QUESTION_BLOCK_BBOX_WIDTH;
	b = t + QUESTION_BLOCK_BBOX_HEIGHT;
}

void CQuestionBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isToggle)
	{
		if (time_elapsed < SHAKE_TIME && !isOverBound)
		{
			time_elapsed += dt;
			this->y -= SHAKE_VELOCITY * dt;
			return;
		}
		else if (time_elapsed >= SHAKE_TIME && !isOverBound)
		{
			this->isOverBound = true;
			return;
		}

		if (time_elapsed > 0 && isOverBound)
		{
			time_elapsed -= dt;
			this->y += SHAKE_VELOCITY * dt;
			return;
		}
		else
		{
			isToggle = false;
			this->y = this->y_legacy;
			time_elapsed = 0;
		}
	}
}

void CQuestionBlock::ShakeToggle()
{
	this->isToggle = true;
}