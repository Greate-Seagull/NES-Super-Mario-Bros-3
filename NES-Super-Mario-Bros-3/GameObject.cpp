#include <d3dx9.h>
#include <algorithm>


#include "debug.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"

CGameObject::CGameObject()
{
	x = y = 0;
	state = STATE_LIVE;
	isDeleted = false;
}

void CGameObject::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - bbox_width / 2;
	top = y - bbox_height / 2;
	right = left + bbox_width;
	bottom = top + bbox_height;
}

void CGameObject::SetState(int state)
{
	switch (state)
	{
		case STATE_DIE:
			//this->Delete();
			break;
	}
}

void CGameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l,t,r,b; 

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy; 
	CGame::GetInstance()->GetCamPos(cx, cy);

	CGame::GetInstance()->Draw(x - cx, y - cy, bbox, &rect, BBOX_ALPHA);
}

CGameObject::~CGameObject()
{

}

string CGameObject::ToString()
{
	return "Game Object";
}
