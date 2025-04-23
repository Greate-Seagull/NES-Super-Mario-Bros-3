#pragma once

#include <Windows.h>
#include <unordered_map>

#include "Sprites.h"
#include "AnimationFrame.h"

#define ID_ANI_DIRECTION_LEFT 0
#define ID_ANI_DIRECTION_RIGHT 1
#define ID_ANI_DIRECTION_FRONT 2
#define ID_ANI_DIRECTION_REAR 3

#define ID_ANI_DIRECTION_DOWN 0
#define ID_ANI_DIRECTION_UP 10

using namespace std;

class CAnimation
{
	ULONGLONG lastFrameTime;
	int defaultTime;
	int currentFrame;
	vector<LPANIMATION_FRAME> frames;
public:
	CAnimation(int defaultTime = 100) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
	void Add(int spriteId, DWORD time = 0);
	LPANIMATION_FRAME GetCurrentFrame();
	void Render(float x, float y, bool is_switched = false);
	void ChangeTimePerFrame(DWORD time);
	void SwitchSprite();
};

typedef CAnimation* LPANIMATION;