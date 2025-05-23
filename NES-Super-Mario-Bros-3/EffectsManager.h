#pragma once

#include "BoomEffect.h"

class CEffectsManager
{
	static CEffectsManager* __instance;
public:
	static CEffectsManager* GetInstance();

	void CreateAttackEffect(float x, float y);
};

