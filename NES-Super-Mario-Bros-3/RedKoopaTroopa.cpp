#include "RedKoopaTroopa.h"

CRedKoopaTroopa::CRedKoopaTroopa(float x, float y, bool haveWings):
	CKoopaTroopa(x, y, haveWings)
{
}

void CRedKoopaTroopa::OnNoCollisionWithBlocking(DWORD dt)
{
	switch (state)
	{
	case STATE_LIVE:
		if (isOnGround)
		{
			Patrol();
			x = on_ground_x;
			y = on_ground_y;
			vy = 0.0f;
		}
		break;
	}

	CKoopaTroopa::OnNoCollisionWithBlocking(dt);
}

int CRedKoopaTroopa::GetObjectAniID()
{
	return ANI_ID_RED_KOOPA;
}