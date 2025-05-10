#pragma once

#include "Game.h"
#include "PlayScene.h"
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_HUD -7000

#define HUD_WIDTH 256
#define HUD_HEIGHT 48

//#define BACKGROUND_WIDTH 2560
//#define BACKGROUND_HEIGHT 393

class CHud : public CGameObject {
protected:
	float originalX, originalY;
public:
	CHud(float x, float y) : CGameObject(x, y) 
	{ 
		aniID = ID_ANI_HUD;
		originalX = x;
		originalY = y;

		SetBoundingBox(HUD_WIDTH, HUD_HEIGHT);
	}
	int IsCollidable() { return 0; }

	void GetOriginalPos(float& ox, float& oy) { ox = originalX; oy = originalY; }
};