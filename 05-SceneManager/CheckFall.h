#pragma once
#include "GameObject.h"

#define BOX_SMALL_BBOX_WIDTH 7
#define BOX_SMALL_BBOX_HEIGHT 7

#define STATE_LEFT_KOOPA 100
#define STATE_RIGHT_KOOPA 200
#define SMALL_STATE_LEFT_KOOPA 300
#define SMALL_STATE_RIGHT_KOOPA 400

#define SMALL_SPEED_PREVIOUS_KOOPA 0.0128f

#define SPEED_PREVIOUS_KOOPA 0.015f

#define CHECKFALL_NORMAL	1
#define CHECKFALL_SMALL		2

// 
// The most popular type of object in Mario! 
// 
class CCheckFall : public CGameObject
{
protected:
	float ax;
	float ay;

	bool OnTheBox;

	bool isOnPlatformCheck;

	int model; 

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);



public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	int GetModel() { return model; }

	virtual void SetState(int state);

	//void SetIsOnPlatform(bool b) { isOnPlatformCheck = b; }

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	bool GetNoOntheBox() { return OnTheBox; }

	//bool GetIsOnPlatForm2() { return isOnPlatformCheck; };

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithPlatForm(LPCOLLISIONEVENT e);

	void OnCollisionWithBackGround(LPCOLLISIONEVENT e);
	//void OnCollisionWithGlassBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithBox(LPCOLLISIONEVENT e);
	CCheckFall(float x, float y, int model);


	void GetDeleted() {
		isDeleted = true;
	}


	bool GetIntheBox() { return OnTheBox; }
	bool GetIsOnPlatform() { return isOnPlatformCheck; }


	void SetIsOnPlatform(bool b) { b = isOnPlatformCheck; }

	//set ay
	void SETay(float y) { y = ay; }

	/*bool checkfalling()
	{
		if (vy > 0.0004f) return true;
	}*/
};