#include "Map.h"
#include "debug.h"


void CMap::Render()
{
	//if (model == 1) {
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_MAP)->Render(x, y);

	//}
	//else {
	//	CAnimations* animations = CAnimations::GetInstance();
	//	animations->Get(ID_ANI_HIDEN_MAP)->Render(x, y);
	//}
}

void CMap::GetBoundingBox(float& l, float& t, float& r, float& b) {}