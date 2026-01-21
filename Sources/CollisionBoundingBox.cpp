#include	"CollisionBoundingBox.h"
#include	"Polygon.h"

//----------------------------------------------------------
// 2‚Â‚ÌBoundingBox‚Ì“–‚½‚è”»’è‚ğæ‚éŠÖ”
bool CollisionBoundingBox::getCollisionState(FCoord2D *box1, FCoord2D *box2){
	if (box1->x >= box2->x - box1->w && 
		box1->x <= box2->x + box2->w &&
		box1->y >= box2->y - box1->h &&
		box1->y <= box2->y + box2->h)
	{
		return true;
	}

	return false;
}