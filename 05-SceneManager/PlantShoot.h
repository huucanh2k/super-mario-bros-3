#pragma once
#include "Gameobject.h"



#define SPEED 0.025f

#define PLANT_BBOX_WIDTH 12
#define PLANT_BBOX_HEIGHT_SMALL 24
#define PLANT_BBOX_HEIGHT 32

#define ID_ANI_PLANT_LEFT_UNDER_NOT_SHOOT	301
#define ID_ANI_PLANT_LEFT_UNDER_SHOOT 302
#define ID_ANI_PLANT_LEFT_TOP_NOT_SHOOT 303
#define ID_ANI_PLANT_LEFT_TOP_SHOOT 304
#define ID_ANI_PLANT_RIGHT_UNDER_NOT_SHOOT	305
#define ID_ANI_PLANT_RIGHT_UNDER_SHOOT 306
#define ID_ANI_PLANT_RIGHT_TOP_NOT_SHOOT 307
#define ID_ANI_PLANT_RIGHT_TOP_SHOOT 308



class CPlantShoot : public CGameObject
{
protected:
	float loacationX;
	float startY;
	bool isShoot;
	float minY;


	float distanceMario_PlantEnemies();
	BOOLEAN isNotCollisMario = false;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	int LeftORightMario();
	int TopOrBottomYMario();


	virtual int IsBlocking() { return 0; }

	virtual void OnNoCollision(DWORD dt);

public:
	CPlantShoot(float x, float y);

	virtual void SetState(int state);

};
