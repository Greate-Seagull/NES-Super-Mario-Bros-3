#include "BoomEffect.h"

CBoomEffect::CBoomEffect(float x, float y)
	:CGameObject(x, y)
{
	time_lasting = 0;

	bbox_width = 0.0f;
	bbox_height = 0.0f;

	aniID = ID_ANI_ATTACK_EFFECT;
}

void CBoomEffect::Prepare(DWORD dt)
{
	time_lasting += dt;
	if (time_lasting >= TIME_ATTACK_EFFECT)
	{
		Delete();
	}
}
