#include "Collision.h"
#include "GameObject.h"
#include "SuperLeaf.h"

#include "debug.h"

#define BLOCK_PUSH_FACTOR 0.01f

CCollision* CCollision::__instance = NULL;

int CCollisionEvent::WasCollided() {
	return
		t >= 0.0f && t <= 1.0f && obj->IsDirectionColliable(nx, ny) == 1;
}

CCollision* CCollision::GetInstance()
{
	if (__instance == NULL) __instance = new CCollision();
	return __instance;
}

CCollisionTracker* CCollision::GetTracker()
{
	return &coTracker;
}

CCollisionEventPool* CCollision::GetPool()
{
	return &eventPool;
}

/*
	SweptAABB 
*/
void CCollision::SweptAABB(
	float ml, float mt, float mr, float mb,
	float dx, float dy,
	float sl, float st, float sr, float sb,
	float& t, float& nx, float& ny)
{

	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry;
	float t_exit;

	t = -1.0f;			// no collision
	nx = ny = 0;

	//
	// Broad-phase test 
	//

	float bl = dx > 0 ? ml : ml + dx;
	float bt = dy > 0 ? mt : mt + dy;
	float br = dx > 0 ? mr + dx : mr;
	float bb = dy > 0 ? mb + dy : mb;

	if (br < sl || bl > sr || bb < st || bt > sb) 
		return;

	if (dx == 0 && dy == 0)
		return;// moving object is not moving > obvious no collision

	if (dx > 0)
	{
		dx_entry = sl - mr;
		dx_exit = sr - ml;
	}
	else if (dx < 0)
	{
		dx_entry = sr - ml;
		dx_exit = sl - mr;
	}


	if (dy > 0)
	{
		dy_entry = st - mb;
		dy_exit = sb - mt;
	}
	else if (dy < 0)
	{
		dy_entry = sb - mt;
		dy_exit = st - mb;
	}

	if (dx == 0)
	{
		tx_entry = -9999999.0f;
		tx_exit = 99999999.0f;
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}

	if (dy == 0)
	{
		ty_entry = -99999999999.0f;
		ty_exit = 99999999999.0f;
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}


	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) 
		return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (t_entry > t_exit) 
		return;

	t = t_entry;

	if (tx_entry > ty_entry)
	{
		ny = 0.0f;
		dx > 0 ? nx = -1.0f : nx = 1.0f;
	}
	else
	{
		nx = 0.0f;
		dy > 0 ? ny = -1.0f : ny = 1.0f;
	}
}

/*
	Extension of original SweptAABB to deal with two moving objects
*/

//Origin

//LPCOLLISIONEVENT CCollision::SweptAABB(LPGAMEOBJECT objSrc, DWORD dt, LPGAMEOBJECT objDest)
//{
//	float sl, st, sr, sb;		// static object bbox
//	float ml, mt, mr, mb;		// moving object bbox
//	float t, nx, ny;
//
//	float mvx, mvy;
//	objSrc->GetSpeed(mvx, mvy);
//	float mdx = mvx * dt;
//	float mdy = mvy * dt;
//
//	float svx, svy;
//	objDest->GetSpeed(svx, svy);
//	float sdx = svx * dt;
//	float sdy = svy * dt;
//
//	//
//	// NOTE: new m speed = original m speed - collide object speed
//	// 
//	float dx = mdx - sdx;
//	float dy = mdy - sdy;
//
//	objSrc->GetBoundingBox(ml, mt, mr, mb);
//	objDest->GetBoundingBox(sl, st, sr, sb);
//
//	SweptAABB(
//		ml, mt, mr, mb,
//		dx, dy,
//		sl, st, sr, sb,
//		t, nx, ny
//	);
//
//	CCollisionEvent* e = new CCollisionEvent(t, nx, ny, dx, dy, objDest, objSrc);
//	return e;
//}
//
///*
//	Calculate potential collisions with the list of colliable objects
//
//	coObjects: the list of colliable objects
//	coEvents: list of potential collisions
//*/
//void CCollision::Scan(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* objDests, vector<LPCOLLISIONEVENT>& coEvents)
//{
//	for (UINT i = 0; i < objDests->size(); i++)
//	{
//		LPCOLLISIONEVENT e = SweptAABB(objSrc, dt, objDests->at(i));
//
//		if (e->WasCollided() == 1)
//			coEvents.push_back(e);
//		else
//			delete e;
//	}
//
//	//std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
//}
//
//void CCollision::Filter( LPGAMEOBJECT objSrc,
//	vector<LPCOLLISIONEVENT>& coEvents,
//	LPCOLLISIONEVENT &colX,
//	LPCOLLISIONEVENT &colY,
//	int filterBlock = 1,		// 1 = only filter block collisions, 0 = filter all collisions 
//	int filterX = 1,			// 1 = process events on X-axis, 0 = skip events on X 
//	int filterY = 1)			// 1 = process events on Y-axis, 0 = skip events on Y
//{
//	float min_tx, min_ty;
//
//	min_tx = 1.0f;
//	min_ty = 1.0f;
//	int min_ix = -1;
//	int min_iy = -1;
//
//	for (UINT i = 0; i < coEvents.size(); i++)
//	{
//		LPCOLLISIONEVENT c = coEvents[i];
//		if (c->isDeleted) continue;
//		if (c->obj->IsDeleted()) continue; 
//
//		// ignore collision event with object having IsBlocking = 0 (like coin, mushroom, etc)
//		if (filterBlock == 1 && !c->obj->IsBlocking()) 
//		{
//			continue;
//		}
//
//		if (c->t < min_tx && c->nx != 0 && filterX == 1) {
//			min_tx = c->t; min_ix = i;
//		}
//
//		if (c->t < min_ty && c->ny != 0 && filterY == 1) {
//			min_ty = c->t; min_iy = i;
//		}
//	}
//
//	if (min_ix >= 0) colX = coEvents[min_ix];
//	if (min_iy >= 0) colY = coEvents[min_iy];
//}
//
///*
//*  Simple/Sample collision framework 
//*  NOTE: Student might need to improve this based on game logic 
//*/
//void CCollision::Process(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* coObjects) //Adjust to not move object if no collision
//{
//	vector<LPCOLLISIONEVENT> coEvents;
//	LPCOLLISIONEVENT colX = NULL; 
//	LPCOLLISIONEVENT colY = NULL;
//
//	coEvents.clear();
//
//	if (objSrc->IsCollidable())
//	{
//		Scan(objSrc, dt, coObjects, coEvents);
//	}
//
//	// No collision detected
//	if (coEvents.size() == 0)
//	{
//		objSrc->OnNoCollision(dt);
//	}
//	else
//	{
//		Filter(objSrc, coEvents, colX, colY);
//
//		float x, y, vx, vy, dx, dy;
//		objSrc->GetPosition(x, y);
//		objSrc->GetSpeed(vx, vy);
//		dx = vx * dt;
//		dy = vy * dt;
//
//		if (colX != NULL && colY != NULL) 
//		{
//			if (colY->t < colX->t)	// was collision on Y first ?
//			{
//				y += colY->t * dy + colY->ny * BLOCK_PUSH_FACTOR;
//				objSrc->SetPosition(x, y);
//
//				objSrc->OnCollisionWith(colY);
//
//				objSrc->GetPosition(x, y);
//				//
//				// see if after correction on Y, is there still a collision on X ? 
//				//
//				LPCOLLISIONEVENT colX_other = NULL;
//
//				//
//				// check again if there is true collision on X 
//				//
//				colX->isDeleted = true;		// remove current collision event on X
//
//				// replace with a new collision event using corrected location 
//				coEvents.push_back(SweptAABB(objSrc, dt, colX->obj));
//
//				// re-filter on X only
//				Filter(objSrc, coEvents, colX_other, colY, /*filterBlock = */ 1, 1, /*filterY=*/0);
//
//				if (colX_other != NULL)
//				{
//					x += colX_other->t * dx +colX_other->nx * BLOCK_PUSH_FACTOR;	
//
//					objSrc->SetPosition(x, y);
//
//					objSrc->OnCollisionWith(colX_other);
//				}
//				/*else
//				{
//					x += dx;
//				}*/
//			}
//			else // collision on X first
//			{
//				x += colX->t * dx + colX->nx * BLOCK_PUSH_FACTOR;
//				
//				objSrc->SetPosition(x, y);
//
//				objSrc->OnCollisionWith(colX);
//
//				objSrc->GetPosition(x, y);
//				//
//				// see if after correction on X, is there still a collision on Y ? 
//				//
//				LPCOLLISIONEVENT colY_other = NULL;
//
//				//
//				// check again if there is true collision on Y
//				//
//				colY->isDeleted = true;		// remove current collision event on Y
//
//				// replace with a new collision event using corrected location 
//				coEvents.push_back(SweptAABB(objSrc, dt, colY->obj));
//
//				// re-filter on Y only
//				Filter(objSrc, coEvents, colX, colY_other, /*filterBlock = */ 1, /*filterX=*/0, /*filterY=*/1);
//
//				if (colY_other != NULL)
//				{
//					y += colY_other->t * dy + colY_other->ny * BLOCK_PUSH_FACTOR;
//
//					objSrc->SetPosition(x, y);
//
//					objSrc->OnCollisionWith(colY_other);
//				}
//				/*else
//				{
//					y += dy;
//				}*/
//			}
//		}
//		else if (colX != NULL)
//		{
//			x += colX->t * dx + colX->nx * BLOCK_PUSH_FACTOR;
//			//y += dy;
//			objSrc->SetPosition(x, y); //Can be commented
//
//			objSrc->OnCollisionWith(colX);
//		}
//		else if (colY != NULL)
//		{
//			//x += dx;
//			y += colY->t * dy + colY->ny * BLOCK_PUSH_FACTOR;
//
//			objSrc->SetPosition(x, y); //Can be commented
//
//			objSrc->OnCollisionWith(colY);
//		}
//		//else // both colX & colY are NULL 
//		//{
//		//	x += dx;
//		//	y += dy;
//		//}
//		
//		//objSrc->SetPosition(x, y);
//	}
//
//
//	//
//	// Scan all non-blocking collisions for further collision logic
//	//
//	for (UINT i = 0; i < coEvents.size(); i++)
//	{
//		LPCOLLISIONEVENT e = coEvents[i];
//		if (e->isDeleted) continue;
//		if (e->obj->IsBlocking()) continue;  // blocking collisions were handled already, skip them
//
//		objSrc->OnCollisionWith(e);			
//	}
//
//
//	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
//}

//New version

void CCollision::SweptAABB(LPGAMEOBJECT objSrc, DWORD dt, LPGAMEOBJECT objDest)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	float mvx, mvy;
	objSrc->GetSpeed(mvx, mvy);
	float mdx = mvx * dt;
	float mdy = mvy * dt;

	float svx, svy;
	objDest->GetSpeed(svx, svy);
	float sdx = svx * dt;
	float sdy = svy * dt;

	//
	// NOTE: new m speed = original m speed - collide object speed
	// 
	float dx = mdx - sdx;
	float dy = mdy - sdy;

	/*float dx = mdx;
	float dy = mdy;*/

	objSrc->GetBoundingBox(ml, mt, mr, mb);
	objDest->GetBoundingBox(sl, st, sr, sb);

	SweptAABB(
		ml, mt, mr, mb,
		dx, dy,
		sl, st, sr, sb,
		t, nx, ny
	);

	eventPool.Allocate(t, nx, ny, dx, dy, objDest, objSrc);
}

/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void CCollision::Scan(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* objDests)
{
	for (UINT i = 0; i < objDests->size(); i++)
	{
		if (objSrc == objDests->at(i))
			continue;
		if (objSrc->IsLinkedTo(objDests->at(i)))
			continue;
		if (coTracker.IsResolved(objSrc, objDests->at(i)))
			continue;

		SweptAABB(objSrc, dt, objDests->at(i));
		LPCOLLISIONEVENT e = eventPool.GetLast();

		if (e->WasCollided() != 1)
			eventPool.VirtualDelete();
	}

	//std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CCollision::Filter(LPGAMEOBJECT objSrc,
	LPCOLLISIONEVENT& colX,
	LPCOLLISIONEVENT& colY,
	int filterBlock = 1,		// 1 = only filter block collisions, 0 = filter all collisions 
	int filterX = 1,			// 1 = process events on X-axis, 0 = skip events on X 
	int filterY = 1)			// 1 = process events on Y-axis, 0 = skip events on Y
{
	float min_tx, min_ty;

	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	for (UINT i = 0; i < eventPool.Size(); i++)
	{
		LPCOLLISIONEVENT c = eventPool.Get(i);
		if (c->isDeleted) continue;
		if (c->obj->IsDeleted()) continue;

		// ignore collision event with object having IsBlocking = 0 (like coin, mushroom, etc)
		if (filterBlock == 1 && !c->obj->IsBlocking())
		{
			continue;
		}

		if (c->t < min_tx && c->nx != 0 && filterX == 1) {
			min_tx = c->t; min_ix = i;
		}

		if (c->t < min_ty && c->ny != 0 && filterY == 1) {
			min_ty = c->t; min_iy = i;
		}
	}

	if (min_ix >= 0) colX = eventPool.Get(min_ix);
	if (min_iy >= 0) colY = eventPool.Get(min_iy);
}

/*
*  Simple/Sample collision framework
*  NOTE: Student might need to improve this based on game logic
*/
void CCollision::Process(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* coObjects) //Adjust to not move object if no collision
{
	LPCOLLISIONEVENT colX = NULL;
	LPCOLLISIONEVENT colY = NULL;	

	if (objSrc->IsCollidable())
	{
		Scan(objSrc, dt, coObjects);
	}

	// No collision detected
	if (eventPool.Size() == 0)
	{
		objSrc->OnNoCollision(dt);
	}
	else
	{
		Filter(objSrc, colX, colY);

		float x, y, vx, vy, dx, dy;
		objSrc->GetPosition(x, y);
		objSrc->GetSpeed(vx, vy);
		dx = vx * dt;
		dy = vy * dt;

		if (colX != NULL && colY != NULL)
		{
			if (colY->t < colX->t)	// was collision on Y first ?
			{
				y += colY->t * dy + colY->ny * BLOCK_PUSH_FACTOR;

				objSrc->SetPosition(x, y);

				objSrc->OnCollisionWith(colY);

				objSrc->GetPosition(x, y);
				//
				// see if after correction on Y, is there still a collision on X ? 
				//
				LPCOLLISIONEVENT colX_other = NULL;

				//
				// check again if there is true collision on X 
				//
				colX->isDeleted = true;		// remove current collision event on X

				// replace with a new collision event using corrected location 
				SweptAABB(objSrc, dt, colX->obj);

				// re-filter on X only
				Filter(objSrc, colX_other, colY, /*filterBlock = */ 1, 1, /*filterY=*/0);

				if (colX_other != NULL)
				{
					x += colX_other->t * dx + colX_other->nx * BLOCK_PUSH_FACTOR;

					objSrc->SetPosition(x, y);

					objSrc->OnCollisionWith(colX_other);
				}
			}
			else // collision on X first
			{
				x += colX->t * dx + colX->nx * BLOCK_PUSH_FACTOR;

				objSrc->SetPosition(x, y);

				objSrc->OnCollisionWith(colX);

				objSrc->GetPosition(x, y);
				//
				// see if after correction on X, is there still a collision on Y ? 
				//
				LPCOLLISIONEVENT colY_other = NULL;

				//
				// check again if there is true collision on Y
				//
				colY->isDeleted = true;		// remove current collision event on Y

				// replace with a new collision event using corrected location 
				SweptAABB(objSrc, dt, colY->obj);

				// re-filter on Y only
				Filter(objSrc, colX, colY_other, /*filterBlock = */ 1, /*filterX=*/0, /*filterY=*/1);

				if (colY_other != NULL)
				{
					y += colY_other->t * dy + colY_other->ny * BLOCK_PUSH_FACTOR;

					objSrc->SetPosition(x, y);

					objSrc->OnCollisionWith(colY_other);
				}
			}
		}
		else if (colX != NULL)
		{
			x += colX->t * dx + colX->nx * BLOCK_PUSH_FACTOR;

			objSrc->SetPosition(x, y);
			
			objSrc->OnCollisionWith(colX);
		}
		else if (colY != NULL)
		{
			y += colY->t * dy + colY->ny * BLOCK_PUSH_FACTOR;

			objSrc->SetPosition(x, y);
			
			objSrc->OnCollisionWith(colY);
		}
	}


	//
	// Scan all non-blocking collisions for further collision logic
	//
	for (UINT i = 0; i < eventPool.Size(); i++)
	{
		LPCOLLISIONEVENT e = eventPool.Get(i);
		if (e->isDeleted) continue;
		if (e->obj->IsBlocking()) continue;  // blocking collisions were handled already, skip them

		objSrc->OnCollisionWith(e);
	}

	eventPool.Refresh();
}

void CCollision::SolveCollisionWithBlocking(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* blockingObjects)
{
	LPCOLLISIONEVENT colX = NULL;
	LPCOLLISIONEVENT colY = NULL;

	Scan(objSrc, dt, blockingObjects);

	// No collision detected
	if (eventPool.Size() == 0)
	{
		objSrc->OnNoCollisionWithBlocking(dt);
	}
	else
	{
		Filter(objSrc, colX, colY);

		float x, y, vx, vy, dx, dy;
		objSrc->GetPosition(x, y);
		objSrc->GetSpeed(vx, vy);
		dx = vx * dt;
		dy = vy * dt;

		if (colX != NULL && colY != NULL)
		{
			if (colY->t < colX->t)	// was collision on Y first ?
			{
				y += colY->t * dy + colY->ny * BLOCK_PUSH_FACTOR;

				objSrc->SetPosition(x, y);

				objSrc->OnCollisionWith(colY);

				objSrc->GetPosition(x, y);
				//
				// see if after correction on Y, is there still a collision on X ? 
				//
				LPCOLLISIONEVENT colX_other = NULL;

				//
				// check again if there is true collision on X 
				//
				colX->isDeleted = true;		// remove current collision event on X

				// replace with a new collision event using corrected location 
				SweptAABB(objSrc, dt, colX->obj);

				// re-filter on X only
				Filter(objSrc, colX_other, colY, /*filterBlock = */ 1, 1, /*filterY=*/0);

				if (colX_other != NULL)
				{
					x += colX_other->t * dx + colX_other->nx * BLOCK_PUSH_FACTOR;

					objSrc->SetPosition(x, y);

					objSrc->OnCollisionWith(colX_other);
				}
			}
			else // collision on X first
			{
				x += colX->t * dx + colX->nx * BLOCK_PUSH_FACTOR;

				objSrc->SetPosition(x, y);

				objSrc->OnCollisionWith(colX);

				objSrc->GetPosition(x, y);
				//
				// see if after correction on X, is there still a collision on Y ? 
				//
				LPCOLLISIONEVENT colY_other = NULL;

				//
				// check again if there is true collision on Y
				//
				colY->isDeleted = true;		// remove current collision event on Y

				// replace with a new collision event using corrected location 
				SweptAABB(objSrc, dt, colY->obj);

				// re-filter on Y only
				Filter(objSrc, colX, colY_other, /*filterBlock = */ 1, /*filterX=*/0, /*filterY=*/1);

				if (colY_other != NULL)
				{
					y += colY_other->t * dy + colY_other->ny * BLOCK_PUSH_FACTOR;

					objSrc->SetPosition(x, y);

					objSrc->OnCollisionWith(colY_other);
				}
			}
		}
		else if (colX != NULL)
		{
			x += colX->t * dx + colX->nx * BLOCK_PUSH_FACTOR;

			objSrc->SetPosition(x, y);

			objSrc->OnCollisionWith(colX);
		}
		else if (colY != NULL)
		{
			y += colY->t * dy + colY->ny * BLOCK_PUSH_FACTOR;

			objSrc->SetPosition(x, y);

			objSrc->OnCollisionWith(colY);
		}
	}

	if(colX)
		coTracker.MarkAsResolved(objSrc, colX->obj);
	if (colY)
		coTracker.MarkAsResolved(objSrc, colY->obj);

	eventPool.Refresh();
}

void CCollision::SolveCollisionWithNonBlocking(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* nonBlockingObjects)
{
	Scan(objSrc, dt, nonBlockingObjects);

	if (eventPool.Size() == 0)
	{
		objSrc->OnNoCollisionWithNonBlocking(dt);
		return;
	}

	for (UINT i = 0; i < eventPool.Size(); i++)
	{
		LPCOLLISIONEVENT e = eventPool.Get(i);
		if (e->isDeleted) continue;

		objSrc->OnCollisionWith(e);

		coTracker.MarkAsResolved(objSrc, e->obj);
	}

	eventPool.Refresh();
}

void CCollision::SolveOverlap(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (objSrc == coObjects->at(i))
			continue;
		if (objSrc->IsLinkedTo(coObjects->at(i)))
			continue;
		if (coTracker.IsResolved(objSrc, coObjects->at(i)))
			continue;

		if (Overlap(objSrc, coObjects->at(i)))
		{
			eventPool.Allocate(-1, 0, 0, 0, 0, coObjects->at(i), objSrc);
			CCollisionEvent* e = eventPool.GetLast();
			objSrc->OnCollisionWith(e);

			coTracker.MarkAsResolved(objSrc, coObjects->at(i));
		}
	}

	eventPool.Refresh();
}

bool CCollision::Overlap(LPGAMEOBJECT objSrc, LPGAMEOBJECT objDst)
{
	/*float min_width = (objSrc->GetBBoxWidth() + objDst->GetBBoxWidth()) / 2.0f;
	float min_height = (objSrc->GetBBoxHeight() + objDst->GetBBoxHeight()) / 2.0f;

	float objSrc_x, objSrc_y;
	float objDst_x, objDst_y;

	objSrc->GetPosition(objSrc_x, objSrc_y);
	objDst->GetPosition(objDst_x, objDst_y);*/

	float objSrc_l, objSrc_t, objSrc_r, objSrc_b;
	float objDst_l, objDst_t, objDst_r, objDst_b;

	objSrc->GetBoundingBox(objSrc_l, objSrc_t, objSrc_r, objSrc_b);
	objDst->GetBoundingBox(objDst_l, objDst_t, objDst_r, objDst_b);

	bool horizontally_inside = (objSrc_l <= objDst_r) && (objDst_l <= objSrc_r);
	bool vertically_inside = (objSrc_t <= objDst_b) && (objDst_t <= objSrc_b);

	return horizontally_inside && vertically_inside;

	//return fabs(objSrc_x - objDst_x) < min_width && fabs(objSrc_y - objDst_y) < min_height;
}

CCollisionEventPool::CCollisionEventPool()
{
	current_event = 0;
}

LPCOLLISIONEVENT CCollisionEventPool::Allocate(float t, float nx, float ny, float dx, float dy, LPGAMEOBJECT objDest, LPGAMEOBJECT objSrc)
{
	if (current_event >= co_events.size())
	{
		co_events.emplace_back(t, nx, ny, dx, dy, objDest, objSrc);
	}
	else
	{
		co_events[current_event] = CCollisionEvent(t, nx, ny, dx, dy, objDest, objSrc);
	}

	return &co_events[current_event++];
}

void CCollisionEventPool::VirtualDelete()
{
	current_event = max(0, current_event - 1);
}

void CCollisionEventPool::Refresh()
{
	current_event = 0;
}

LPCOLLISIONEVENT CCollisionEventPool::Get(int i)
{
	if (i < 0 || i >= current_event)
		return nullptr;

	return &co_events[i];
}

LPCOLLISIONEVENT CCollisionEventPool::GetLast()
{
	if (current_event == 0)
		return nullptr;

	return &co_events[current_event - 1];
}

int CCollisionEventPool::Size()
{
	return current_event;
}

void CCollisionTracker::Allocate(LPGAMEOBJECT object)
{
	tracker[object] = unordered_map<LPGAMEOBJECT, bool>();
}

void CCollisionTracker::MarkAsResolved(LPGAMEOBJECT key, LPGAMEOBJECT collider)
{
	tracker[key][collider] = true;
	tracker[collider][key] = true;
}

bool CCollisionTracker::IsResolved(LPGAMEOBJECT key, LPGAMEOBJECT collider)
{
	return tracker[key][collider];
}