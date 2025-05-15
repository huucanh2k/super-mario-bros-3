#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_SUPER_LEAF_LEFT 12000
#define ID_ANI_SUPER_LEAF_RIGHT 12010

#define SUPER_LEAF_GRAVITY 0.001f
#define SUPER_LEAF_OSCILLIATE_SPEED 0.01f
#define	SUPER_LEAF_WIDTH 10
#define OUT_BRICK 0.15f
#define SUPER_LEAF_MAX_SPEED_FALL 0.08f
#define SUPER_LEAF_BBOX_WIDTH 10
#define SUPER_LEAF_BBOX_HEIGHT 16
#define SUPER_LEAF_SUMMON_STATE 100
#define SUPER_LEAF_STATE_BASIC 200

#define SUPER_LEAF_OCSILLATE_TIMEOUT 500

class CSuperLeaf : public CGameObject {
	int state;
	float ay;
	bool canCollect;

	float oscillateTime;
	int oscillateDir;
public:
	CSuperLeaf(float x, float y) : CGameObject(x, y) {
		state = SUPER_LEAF_STATE_BASIC;
		oscillateTime = 0;
        oscillateDir = 1;
		ay = SUPER_LEAF_GRAVITY;
	};
	int GetState() { return state; }
	void SetState(int s);
	virtual void OnNoCollision(DWORD dt);
	bool CanCollect() { return canCollect; }
	void SetCanCollect(bool b) { canCollect = b; }
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};