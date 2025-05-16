#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_LEAF_LEFT 11002
#define ID_ANI_LEAF_RIGHT 11003


#define	LEAF_FALL_SPEED_X 0.075f


#define LEAF_GRAVITY 0.0003f
#define SPEED_COME_MARIO 0.0001f
#define	LEAF_WIDTH 10
#define OUT_BRICK 0.2f
#define LEAF_MAX_SPEED_FALL 0.1f
#define LEAF_BBOX_WIDTH 10
#define LEAF_BBOX_HEIGHT 16
#define LEAF_SUMMON_STATE	100
#define LEAF_NOT_SUMMON_STATE	200
#define LEAF_FALL_STATE 300

class CSuperLeaf : public CGameObject {
	int state;
	float ay;
	float ax;
	float startY;
	float limitY;
	float limitL, limitR;

public:
	CSuperLeaf(float x, float y) : CGameObject(x, y) {
		state = LEAF_NOT_SUMMON_STATE;
		ay = 0;
		ax = 0;
		this->startY = y;
		limitY = y - 50;
		limitL = x;
		limitR = x + 25;

	};
	int GetState() { return state; }
	void SetState(int s);
	virtual void OnNoCollision(DWORD dt);
	//bool CanCollect() { return canCollect; }
	//void SetCanCollect(bool b) { canCollect = b; }
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual int IsCollidable() { return 0; };
	int IsBlocking() { return 0; }
};

