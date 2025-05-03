#pragma once

#include "MovableObject.h"
#include "Creature.h"

//#define EFFECT_BIGGER 10
//#define EFFECT_RACOONIZE 11

class CHelpfulObject : public CMovableObject
{
protected:
	int effect;
public:
	CHelpfulObject(float x, float y);

	virtual int IsCollidable() { return 1; }

	/*virtual void Reaction(CGameObject* by_another, int action);	
	virtual void ReactionToBigger(CGameObject* by_another);	
	virtual void ReactionToRacoonize(CGameObject* by_another);*/

	virtual void LaunchEffect(CCreature* creature);
};