#pragma once
#include "Gameobject.h"
#include "BulletPlant.h"



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

#define PLANT_STATE_UP 100
#define PLANT_STATE_DOWN 200
#define PLANT_STATE_NOT_TOUCH 300
#define PLANT_STATE_NOT_UP 400


#define TIME_OUT_PIPE 5000
#define TIME_CONDITION_TO_SHOOT 1500
#define TIME_IN_PIPE 4000
#define TIME_IN_PIPE_START 4500

#define DISTANCE_MIN_MARIO_PLANT 24
#define DISTANCE_PLANT_ENEMIS_MAYBE_SHOW_UP 150



class CPlantShoot : public CGameObject
{
protected:
	float loacationX;
	float startY;
	bool isShoot;
	float minY;
	bool isUp, isDown;

	ULONGLONG time_out_pipe;
	ULONGLONG time_wait_plant_return;
	ULONGLONG time_down_pipe;
	CBulletPlant* bulletPlant;



	float distanceMario_PlantEnemies();
	BOOLEAN isNotCollisMario = false;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	int LeftORightMario();
	int TopOrBottomYMario();


	virtual int IsBlocking() { return 0; }

	virtual void OnNoCollision(DWORD dt);
	void CreateBullet();

public:
	CPlantShoot(float x, float y);

	virtual void SetState(int state);

	void create_bullet(CGameObject* obj) {
		if (!dynamic_cast<CBulletPlant*>(obj)) return;
		else if (!bulletPlant)
		{
			CBulletPlant* new_bullet = dynamic_cast<CBulletPlant*>(obj);
			bulletPlant = new_bullet;
			DebugOut(L">>> check tao bullet >>> \n");

		}
	}

};
