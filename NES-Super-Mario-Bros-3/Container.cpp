#include "Container.h"

void CContainer::Render()
{
	if (this->length <= 0) return;

	float yy = y;
	float xx = x;
	CSprites* s = CSprites::GetInstance();

	s->Get(this->spriteIdBeginEnd)->Draw(xx, yy);
	xx += this->cellWidth;
	for (int j = 1; j < this->length - 1; j++)
	{
		s->Get(this->spriteIdMiddleEnd)->Draw(xx, yy);
		xx += this->cellWidth;
	}
	if (length > 1)
		s->Get(this->spriteIdEndEnd)->Draw(xx, yy);

	yy -= cellHeight;

	for (int i = 2; i < this->height; i++)
	{
		float xx = x;
		CSprites* s = CSprites::GetInstance();

		s->Get(this->spriteIdBeginMiddle)->Draw(xx, yy);
		xx += this->cellWidth;
		for (int j = 1; j < this->length - 1; j++)
		{
			s->Get(this->spriteIdMiddleMiddle)->Draw(xx, yy);
			xx += this->cellWidth;
		}
		if (length > 1)
			s->Get(this->spriteIdEndMiddle)->Draw(xx, yy);

		yy -= cellHeight;
	}

	if (this->height > 1)
	{
		float xx = x;
		CSprites* s = CSprites::GetInstance();

		s->Get(this->spriteIdBegin)->Draw(xx, yy);
		xx += this->cellWidth;
		for (int j = 1; j < this->length - 1; j++)
		{
			s->Get(this->spriteIdMiddle)->Draw(xx, yy);
			xx += this->cellWidth;
		}
		if (length > 1)
			s->Get(this->spriteIdEnd)->Draw(xx, yy);

		yy -= cellHeight;
	}
}

void CContainer::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - cellWidth / 2.0f;// -length * cellWidth / 2;
	t = y + cellHeight / 2.0f - bbox_height;
	r = l + (bbox_width - 1.0f);
	b = t;
}
