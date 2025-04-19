#include "Bush.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"
#include "Game.h"

void CBigBush::Render()
{
	if (this->height <= 0) return;
	float yy = y;

	for (int i = this->height; i > 1; i--)
	{
		for (int j = 1; j <= 2; j++)
		{
			float xx = x;
			CSprites* s = CSprites::GetInstance();

			s->Get(this->spriteIdBeginEnd)->Draw(xx, yy);
			xx += this->cellWidth;

			s->Get(this->spriteIdEndEnd)->Draw(xx, yy);
		}

		yy -= cellHeight;
	}

	for (int i = 1; i <= 2; i++)
	{
		float xx = x;
		CSprites* s = CSprites::GetInstance();

		s->Get(this->spriteIdBeginBegin)->Draw(xx, yy);
		xx += this->cellWidth;

		s->Get(this->spriteIdEndBegin)->Draw(xx, yy);
	}

}

void CBigBush::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = 0;
	t = 0;
	r = 0;
	b = 0;
}

void CMiniBush::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_MINI_BUSH)->Render(x, y);
}

void CMiniBush::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = 0;
	t = 0;
	r = 0;
	b = 0;
}