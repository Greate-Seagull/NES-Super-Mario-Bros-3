#include "Cloud.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"
#include "Game.h"

void CCloud::Render()
{
	float yy = y;
	float xx = x;

	CSprites* s = CSprites::GetInstance();

	if (this->type == 0)
	{
		if (this->width <= 0) return;
	
		s->Get(this->spriteIdBeginEnd)->Draw(xx, yy);
		xx += this->cellWidth;
		for (int i = 1; i < this->width - 1; i++)
		{
			s->Get(this->spriteIdMiddleEnd)->Draw(xx, yy);
			xx += this->cellWidth;
		}
		if (width > 1)
			s->Get(this->spriteIdEndEnd)->Draw(xx, yy);

		yy -= cellHeight;

		xx = x;
	}

	s->Get(this->spriteIdBeginBegin)->Draw(xx, yy);
	xx += this->cellWidth;
	for (int i = 1; i < this->width - 1; i++)
	{
		s->Get(this->spriteIdMiddleBegin)->Draw(xx, yy);
		xx += this->cellWidth;
	}
	if (width > 1)
		s->Get(this->spriteIdEndBegin)->Draw(xx, yy);
}

void CCloud::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - this->cellWidth / 2.0f;
	t = y - this->cellHeight / 2.0f;
	r = l + this->cellWidth * this->width;
	b = t + this->cellHeight;
}