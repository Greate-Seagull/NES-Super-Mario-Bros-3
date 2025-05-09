#include "Pipe.h"

#include "Creature.h"
#include "Mario.h"

#include "Textures.h"
#include "Game.h"
#include "PlayScene.h"

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

void CPipe::Reaction(CGameObject* by_another, int action)
{
	switch (action)
	{
	case ACTION_TOUCH:
		if (dynamic_cast<CMario*>(by_another))
		{
			float mX, mY, mLife;

			CMario* m = (CMario*)by_another;
			m->GetPosition(mX, mY);
			mLife = m->GetLife();

			if (mX >= this->x && mX <= this->x + PIPE_WIDTH / 2)
			{
				m->PipeEntry(this->warp_direction, this->scene_destination);
				LPPLAYSCENE curr = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
				curr->LoadWarpedMario(newX, newY, mLife, this->warp_direction);
			}
		}
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

	l = x - cell_width / 2.0f;
	t = y + cell_height / 2.0f  - bbox_height;
	r = l + bbox_width;
	b = t + bbox_height;
}