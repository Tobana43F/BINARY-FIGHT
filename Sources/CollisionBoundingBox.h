//==========================================================
// BoundingBox‚Ì“–‚½‚è”»’è‚ðŽæ‚éƒNƒ‰ƒX
#ifndef _COLLISION_BOUNDING_BOX_H
#define _COLLISION_BOUNDING_BOX_H

#include	"Polygon.h"

class CollisionBoundingBox{
public:
	bool getCollisionState(FCoord2D *box1, FCoord2D *box2);
};


#endif // !_COLLISION_BOUNDING_BOX_H