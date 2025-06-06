#pragma once
#include "GameObject.h"

#define TIME_ATTACK_EFFECT 150

#define ID_ANI_ATTACK_EFFECT 60010

class CBoomEffect : public CGameObject
{
	DWORD time_lasting;

public:
	CBoomEffect(float x, float y);
	void Prepare(DWORD dt);
	void Refresh() { this->Delete(); }
};

