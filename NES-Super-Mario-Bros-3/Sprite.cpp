#include "Sprite.h"

#include "Game.h"

CSprite::CSprite(int id, int left, int top, int right, int bottom, LPTEXTURE tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;

	float texWidth = (float)tex->getWidth();
	float texHeight = (float)tex->getHeight();

	// Set the sprite’s shader resource view
	sprite.pTexture = tex->getShaderResourceView();

	sprite.TexCoord.x = (float)this->left / texWidth;
	sprite.TexCoord.y = (float)this->top / texHeight;

	int spriteWidth = (this->right - this->left + 1);
	int spriteHeight = (this->bottom - this->top + 1);

	sprite.TexSize.x = (float)spriteWidth / texWidth;
	sprite.TexSize.y = (float)spriteHeight / texHeight;

	sprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	sprite.TextureIndex = 0;

	D3DXMatrixScaling(&this->matScaling, (FLOAT)spriteWidth, (FLOAT)spriteHeight, 1.0f);	
}

void CSprite::Draw(float x, float y)
{
	CGame* g = CGame::GetInstance();
	float cx, cy;
	g->GetCamPos(cx, cy);

	cx = (FLOAT)floor(cx);
	cy = (FLOAT)floor(cy);

	D3DXMATRIX matTranslation;

	//ID3D10Device* d3ddev = g->GetDirect3DDevice();
	//d3ddev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	//d3ddev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

	float drawX = floor(x - cx);
	float drawY = floor(g->GetBackBufferHeight() - y + cy);
	D3DXMatrixTranslation(&matTranslation, drawX, drawY, 0.1f);

	this->sprite.matWorld = (this->matScaling * matTranslation);

	g->SetPointSamplerState();

	g->GetSpriteHandler()->DrawSpritesImmediate(&sprite, 1, 0, 0);
}

int CSprite::GetHeight()
{
	return bottom - top + 1;
}

int CSprite::GetWidth()
{
	return right - left +1;
}

