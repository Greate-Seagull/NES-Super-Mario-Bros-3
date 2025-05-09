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
	state = -1;
	isDeleted = false;
}

void CGameObject::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - (bbox_width / 2.0f);
	top = y - (bbox_height / 2.0f);
	right = left + bbox_width;
	bottom = top + bbox_height;
}

void CGameObject::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniID)->Render(x, y);
	//RenderBoundingBox();
}

void CGameObject::SetState(int state)
{
	this->state = state;
}

void CGameObject::SetBoundingBox(float width, float height)
{
	this->bbox_width = width;
	this->bbox_height = height;
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

const char* CGameObject::ToString()  
{  
   return "Game Object";  
}
