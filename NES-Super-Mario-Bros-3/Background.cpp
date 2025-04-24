#include "Background.h"

#include "Animation.h"
#include "Animations.h"

#define ID_ANI_REWARD_BASE -60

#define REWARD_OFFSET_MAXIMUM_TIME 4

void CBackground::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = 0;
	t = 0;
	r = 0;
	b = 0;
}

void CEndLevel::Render()
{
	if (this->height <= 0 || this->width <= 0) return;
	CSprites* s = CSprites::GetInstance();

	float yy = y;

	for (int i = 1; i <= this->height; i++)
	{
		s->Get(this->spriteIdBegin)->Draw(x, yy);

		for (int j = 2; j <= this->width; j++)
		{
			s->Get(this->spriteIdEnd)->Draw(x + cell_width * (j - 1), yy);
		}

		yy -= cell_height;
	}	
}

void CEndLevel::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x - cell_width / 2.0f;
	bottom = y + cell_height / 2.0f;
	right = left + bbox_width;
	top = bottom - bbox_height;
}

void CRandomCard::Render()
{
	float xx = x;
	float yy = y;

	CSprites* s = CSprites::GetInstance();

	s->Get(this->spriteIdBeginEnd)->Draw(xx, yy);
	s->Get(this->spriteIdMiddleEnd)->Draw(xx + cellWidth, yy);
	s->Get(this->spriteIdEndEnd)->Draw(xx + cellWidth * 2, yy);

	yy -= cellHeight;

	s->Get(this->spriteIdBeginMiddle)->Draw(xx, yy);
	s->Get(this->spriteIdMiddleMiddle)->Draw(xx + cellWidth, yy);
	s->Get(this->spriteIdEndMiddle)->Draw(xx + cellWidth * 2, yy);

	yy -= cellHeight;

	s->Get(this->spriteIdBeginBegin)->Draw(xx, yy);
	s->Get(this->spriteIdMiddleBegin)->Draw(xx + cellWidth, yy);
	s->Get(this->spriteIdEndBegin)->Draw(xx + cellWidth * 2, yy);
}

void CRandomCard::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x + cellWidth / 2;
	t = y - cellHeight * 1.5;
	r = l + cellWidth;
	b = t + cellWidth;
}

void CReward::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (run)
	{
		time_elapsed++;

		if (time_elapsed >= REWARD_OFFSET_MAXIMUM_TIME)
		{
			switch (type)
			{
			case 1:
			{
				type = 2;
				time_elapsed = 0;
				break;
			}
			case 2:
			{
				type = 3;
				time_elapsed = 0;
				break;
			}
			case 3:
			{
				type = 1;
				time_elapsed = 0;
				break;
			}
			}
		}
	}
}

void CReward::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_REWARD_BASE - this->type)->Render(x, y);
}

void CReward::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = 0;
	t = 0;
	r = 0;
	b = 0;
}
