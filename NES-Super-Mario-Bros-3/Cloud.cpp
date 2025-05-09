#include "Cloud.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"
#include "Game.h"

void CCloud::Render()
{
	if (this->length <= 0) return;
	float yy = y;

	float xx = x;
	CSprites* s = CSprites::GetInstance();

	s->Get(this->spriteIdBeginEnd)->Draw(xx, yy);
	xx += this->cellWidth;
	for (int i = 1; i < this->length - 1; i++)
	{
		s->Get(this->spriteIdMiddleEnd)->Draw(xx, yy);
		xx += this->cellWidth;
	}
	if (length > 1)
		s->Get(this->spriteIdEndEnd)->Draw(xx, yy);

	yy -= cellHeight;

	xx = x;

	s->Get(this->spriteIdBeginBegin)->Draw(xx, yy);
	xx += this->cellWidth;
	for (int i = 1; i < this->length - 1; i++)
	{
		s->Get(this->spriteIdMiddleBegin)->Draw(xx, yy);
		xx += this->cellWidth;
	}
	if (length > 1)
		s->Get(this->spriteIdEndBegin)->Draw(xx, yy);
}

void CCloud::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - this->cellWidth / 2.0f;
	t = y - this->cellHeight / 2.0f;
	r = l + this->cellWidth * this->length;
	b = t + this->cellHeight;
}