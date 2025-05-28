#pragma once

#include "KoopaTroopa.h"

#define ANI_ID_RED_KOOPA 23000

class CRedKoopaTroopa: public CKoopaTroopa
{
public:
	CRedKoopaTroopa(float x, float y, bool haveWings = false);	

	void OnNoCollisionWithBlocking(DWORD dt);
	int GetObjectAniID();
};

