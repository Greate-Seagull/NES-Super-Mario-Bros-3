#include "RedKoopaTroopa.h"

#include "debug.h"

CRedKoopaTroopa::CRedKoopaTroopa(float x, float y, bool haveWings):
	CKoopaTroopa(x, y, haveWings)
{
}

void CRedKoopaTroopa::LivingPrepare(DWORD dt)
{
	if (wings)
	{
		vx = 0.0f;
		oscillate_phase += M_PI / 2500.0f * dt;
		if (oscillate_phase >= 2.0f * M_PI) oscillate_phase -= 2.0f * M_PI;

		vy = 14.0f * M_PI / 625.0f * sin(oscillate_phase);
	}
	else
	{
		vx = nx * KOOPA_VX;
		ay = GAME_GRAVITY;
		CMovableObject::Prepare(dt);
	}
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