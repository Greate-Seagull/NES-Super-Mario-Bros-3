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

int CPipe::IsAllowDigging(CGameObject* obj)
{
	if (warp_direction)
	{
		if (CMario* mario = dynamic_cast<CMario*>(obj))
		{
			float mario_left, mario_top, mario_right, mario_bot;
			mario->GetBoundingBox(mario_left, mario_top, mario_right, mario_bot);

			float pipe_left, pipe_top, pipe_right, pipe_bot;
			this->GetBoundingBoxInside(pipe_left, pipe_top, pipe_right, pipe_bot);

			return mario_left > pipe_left && mario_right < pipe_right;
		}
	}

	return false;
}

int CPipe::IsTouching(CGameObject* obj)
{
	if (warp_direction)
	{
		if (CMario* mario = dynamic_cast<CMario*>(obj))
		{
			float mario_left, mario_top, mario_right, mario_bot;
			mario->GetBoundingBox(mario_left, mario_top, mario_right, mario_bot);

			float pipe_left, pipe_top, pipe_right, pipe_bot;
			this->GetBoundingBoxInside(pipe_left, pipe_top, pipe_right, pipe_bot);

			bool isInside = false;
			if (warp_direction < 0)
				isInside = mario_top <= pipe_bot;
			else if (warp_direction > 0)
				isInside = mario_bot >= pipe_top;

			return isInside;
		}
	}

	return false;
}

int CPipe::IsGoingThrough(CGameObject* obj)
{
	return IsTouching(obj);
}

void CPipe::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (IsAllowDigging(e->obj))
	{
		e->ny = warp_direction;
		e->obj->OnReactionTo(e, ACTION_TOUCH);
	}
	else
		CBlock::OnCollisionWith(e);
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

void CPipe::GetBoundingBoxInside(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y + cell_height / 2.0f - bbox_height;
	r = l + PIPE_BBOX_INSIDE_WIDTH;
	b = t + bbox_height - 1.0f;
}

void CPipe::OnReactionTo(LPCOLLISIONEVENT e, int action)
{
	if (e->ny * warp_direction < 0)
	{
		if (IsAllowDigging(e->src_obj))
		{
			e->Reverse();
			e->ny = warp_direction;
			e->obj->OnReactionTo(e, ACTION_TOUCH);
		}
	}
}

void CPipe::TakeItem()
{
	if (item)
		return;

	switch (itemID)
	{
	case OBJECT_TYPE_VENUS_FIRE_TRAP:
		item = new CVenusFireTrap(x, y, PIRANHA_TYPE_RED);
		UseDefaultItemPosition();
		break;
	case OBJECT_TYPE_GREEN_VENUS:
		item = new CVenusFireTrap(x, y, PIRANHA_TYPE_GREEN);
		UseDefaultItemPosition();
		break;
	case OBJECT_TYPE_PIRANHA_PLANT:
		item = new CPiranhaPlant(x, y);
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
	CCollisionEventPool* pool = CCollision::GetInstance()->GetPool();
	LPCOLLISIONEVENT e = pool->Allocate(0.0f, nx, ny, 0.0f, 0.0f, item, this);
	item->OnReactionTo(e, ACTION_CARRY);
	pool->VirtualDelete();
}

void CPipe::CreateItem(CPlayScene* ps)
{
	TakeItem();
	if (item)
	{
		item->Refresh();
		TriggerItem();
		item->CreateItem(ps);
		ps->Insert(item, -1);
	}
}
