#include "Pipe.h"

#include "VenusFireTrap.h"

#include "PlayScene.h"
#include "Textures.h"

void CPipe::Render()
{
	if (this->height <= 0) return;
	CSprites* s = CSprites::GetInstance();

	//float xx = x - cell_width / 2;
	float xx = x;
	
	if (face_direction == 1) //UP
	{
		//float yy = y + cell_height;
		float yy = y;

		for (int i = 1; i < this->height; i++)
		{
			CSprites* s = CSprites::GetInstance();

			s->Get(this->spriteIdBeginEnd)->Draw(xx, yy);
			s->Get(this->spriteIdEndEnd)->Draw(xx + cell_width, yy);

			yy -= cell_height;
		}

		s->Get(this->spriteIdBeginBegin)->Draw(xx, yy);
		s->Get(this->spriteIdEndBegin)->Draw(xx + cell_width, yy);
	}
	else if (face_direction == -1) //DOWN
	{
		float yy = y - cell_height;
		
		for (int i = 1; i < this->height; i++)
		{
			CSprites* s = CSprites::GetInstance();

			s->Get(this->spriteIdBeginEnd)->Draw(xx, yy);
			s->Get(this->spriteIdEndEnd)->Draw(xx + cell_width, yy);

			yy += cell_height;
		}

		s->Get(this->spriteIdBeginBegin)->Draw(xx, yy);
		s->Get(this->spriteIdEndBegin)->Draw(xx + cell_width, yy);
	}
}

void CPipe::RenderBoundingBox()
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

	float xx = x + this->cell_width / 2.0f;
	float yy = y + this->cell_height / 2.0f - this->bbox_height / 2.0f;

	CGame::GetInstance()->Draw(xx - cx, yy - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
	//CGame::GetInstance()->Draw(x - cx, y - cy, bbox, &rect, BBOX_ALPHA);
}

void CPipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	/*if (this->face_direction == 1)
	{
		l = x - PIPE_BBOX_WIDTH / 2.0f;
		b = y + cell_height * 1.5f;
		r = l + PIPE_BBOX_WIDTH;
		t = b - height * cell_height;
	}
	else if (this->face_direction == -1)
	{
		l = x - PIPE_BBOX_WIDTH / 2.0f;
		t = y - cell_height * 1.5f;
		r = l + PIPE_BBOX_WIDTH;
		b = t + height * cell_height;
	}*/

	l = x - cell_width / 2.0f + 1.0f;
	t = y + cell_height / 2.0f - bbox_height;
	r = l + bbox_width - 1.0f;
	b = t + bbox_height - 1.0f;
}

void CPipe::TakeItem()
{
	if (item)
		return;

	switch (itemID)
	{
	case OBJECT_TYPE_VENUS_FIRE_TRAP:
		item = new CVenusFireTrap(x, y);
		UseDefaultItemPosition();
		break;
	default:
		item = nullptr;
		break;
	}
}

void CPipe::UseDefaultItemPosition()
{
	float l, t, r, b;
	this->GetBoundingBox(l, t, r, b);

	float item_x = l + bbox_width / 2.0f;
	float item_y = t + item->GetBBoxHeight() / 2.0f;

	PutItem(item_x, item_y);
}

void CPipe::TriggerItem()
{
	if (item)
	{
		LPPLAYSCENE ps = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
		ps->Insert(item, ps->Find(this));

		item->Reaction(this, ACTION_CARRY);
	}
}
