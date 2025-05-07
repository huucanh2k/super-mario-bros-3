#pragma once
#include "Gameobject.h"



#define SPEED 0.025f

#define PLANT_BBOX_WIDTH 12
#define PLANT_BBOX_HEIGHT_SMALL 24
#define PLANT_BBOX_HEIGHT 32

#define ID_ANI_PLANT_LEFT_UNDER_SHOOT 302



class CPlantShoot : public CGameObject
{
protected:
	float loacationX;
	float startY;




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
