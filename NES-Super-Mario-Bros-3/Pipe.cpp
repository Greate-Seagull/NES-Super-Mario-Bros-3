#include "Pipe.h"

void CPipe::Render()
{
	if (this->height <= 0) return;
	CSprites* s = CSprites::GetInstance();

	float xx = x - cell_width / 2;
	
	if (face_direction == 1) //UP
	{
		float yy = y + cell_height;

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

	//RenderBoundingBox();
}

void CPipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (this->face_direction == 1)
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
	}
}