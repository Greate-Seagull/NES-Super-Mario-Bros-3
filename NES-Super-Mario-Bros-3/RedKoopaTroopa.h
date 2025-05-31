#pragma once

#include "KoopaTroopa.h"

#define ANI_ID_RED_KOOPA 23000

class CRedKoopaTroopa: public CKoopaTroopa
{
	//WINGS COMPONENT ----------------------------
	float oscillate_phase;
	//--------------------------------------------

public:
	CRedKoopaTroopa(float x, float y, bool haveWings = false);	

	void LivingPrepare(DWORD dt);

	void OnNoCollisionWithBlocking(DWORD dt);

	int GetObjectAniID();
};

