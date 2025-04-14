#include "HelpfulObject.h"

CHelpfulObject::CHelpfulObject(float x, float y) :
	CMovableObject(x, y)
{
}

void CHelpfulObject::LaunchEffect(CCreature* creature)
{
	creature->Reaction(this, effect);
}
