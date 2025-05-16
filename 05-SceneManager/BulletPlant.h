#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"
#include "debug.h"


#define ID_ANI_BULLET_RIGHT 71
#define ID_ANI_BULLET_LEFT 72
#define ID_ANI_BULLET_BLUE 73


#define BULLET_BBOX_WIDTH 9
#define BULLET_BBOX_HEIGHT 9

#define BULLET_BLUE_BBOX_WIDTH 12
#define BULLET_BLUE_BBOX_HEIGHT 12

#define BULLET_SPEED_X 0.04f
#define BULLET_SPEED_Y 0.03f

#define ADJUST_VECTOR_Y 30
#define CHANGE_DIRECTION 30
#define TIME_BULLET_DELETE 5000

#define BULLET_LEFT_BOT 2
#define BULLET_LEFT_TOP 1
#define BULLET_RIGHT_BOT 3
#define BULLET_RIGHT_TOP 4


class CBulletPlant : public CGameObject {

protected:



	ULONGLONG start_deleted;


public:



	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; };

	CBulletPlant(float bx, float by, float nx, float ny);


	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void SetState(int state);

};
