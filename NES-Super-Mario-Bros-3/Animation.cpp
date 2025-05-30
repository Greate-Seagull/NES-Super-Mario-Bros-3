#include "Animation.h"
#include "debug.h"

void CAnimation::Add(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);
	if (sprite == NULL)
		DebugOut(L"[ERROR] Sprite ID %d not found!\n", spriteId);

	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t);
	frames.push_back(frame);
}

LPANIMATION_FRAME CAnimation::GetCurrentFrame()
{
	return (currentFrame == -1) ? nullptr : frames[currentFrame];
}

void CAnimation::Render(float x, float y, bool is_switched)
{
	if (this != NULL)
	{
		if (is_switched == false)
			SwitchSprite();

		frames[currentFrame]->GetSprite()->Draw(x, y);
	}
}

void CAnimation::ChangeTimePerFrame(DWORD time)
{
	for (LPANIMATION_FRAME frame : frames)
	{
		frame->SetTime(time);
	}
}

void CAnimation::SwitchSprite()
{
	ULONGLONG now = GetTickCount64();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
		}
	}
}

