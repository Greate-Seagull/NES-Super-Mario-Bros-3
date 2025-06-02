#include "Platform.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"
#include "Game.h"

#include "HarmfulObject.h";
#include "Block.h"

void CPlatform::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	float xx = x - this->cellWidth / 2 + rect.right / 2;

	CGame::GetInstance()->Draw(xx - cx, y - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

int CPlatform::IsLinkedTo(CGameObject* obj)
{
	return dynamic_cast<CPiranhaPlant*>(obj) || 
		dynamic_cast<CFireball*>(obj) ||
		dynamic_cast<CPlatform*>(obj) ||
		dynamic_cast<CBlock*>(obj);
}

void CPlatform::Render()
{
	if (this->width <= 0) return;

	float yy = y;
	float xx = x;
	CSprites* s = CSprites::GetInstance();

	s->Get(this->spriteIdBeginEnd)->Draw(xx, yy);
	xx += this->cellWidth;
	for (int j = 1; j < this->width - 1; j++)
	{
		s->Get(this->spriteIdMiddleEnd)->Draw(xx, yy);
		xx += this->cellWidth;
	}
	if (width > 1)
		s->Get(this->spriteIdEndEnd)->Draw(xx, yy);

	yy -= cellHeight;

	for (int i = 2; i < this->height; i++)
	{
		float xx = x;

		s->Get(this->spriteIdBeginMiddle)->Draw(xx, yy);
		xx += this->cellWidth;
		for (int j = 1; j < this->width - 1; j++)
		{
			s->Get(this->spriteIdMiddleMiddle)->Draw(xx, yy);
			xx += this->cellWidth;
		}
		if (width > 1)
			s->Get(this->spriteIdEndMiddle)->Draw(xx, yy);

		yy -= cellHeight;
	}

	if (this->height > 1)
	{
		float xx = x;

		s->Get(this->spriteIdBegin)->Draw(xx, yy);
		xx += this->cellWidth;
		for (int j = 1; j < this->width - 1; j++)
		{
			s->Get(this->spriteIdMiddle)->Draw(xx, yy);
			xx += this->cellWidth;
		}
		if (width > 1)
			s->Get(this->spriteIdEnd)->Draw(xx, yy);

		yy -= cellHeight;
	}

	//RenderBoundingBox();
}

void CPlatform::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - cellWidth / 2.0f;
	b = y + cellHeight / 2.0f;
	r = l + bbox_width;
	t = b - bbox_height;
}

int CPlatform::IsDirectionColliable(float nx, float ny)
{
	/*if (nx == 0 && ny == -1) return 1;
	else return 0;*/

	if (type == PLATFORM_TYPE_BLOCK_ANYWAY || ny == -1) return 1;
	else return 0;
}

void CPlatform::OnCollisionWith(LPCOLLISIONEVENT e)
{
	e->Reverse();
	e->src_obj->OnCollisionWith(e);
}