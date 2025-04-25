#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BRICK_QUESTION 113000
#define ID_ANI_BRICK_EMPTY 13000


#define BRICK_Q_BBOX_WIDTH 15
#define BRICK_Q_BBOX_HEIGHT 16
#define BRICK_Q_SPEED_UP 0.04f

#define BRICK_Q_SPEED_DOWN 0.04f

#define BRICK_Q_SPEED_UP 0.04f
#define BRICK_Q_DEFLECT_SPEED 0.1f

#define BRICK_QUESTION_STATE_EMPTY 7000
#define BRICK_QUESTION_STATE_UP 100

#define BRICK_QUESTION_COIN 1
#define BRICK_QUESTION_MUSHROOM 2
#define BRICK_QUESTION_LEAF 3
#define BRICK_QUESTION_MUSHROOM_GREEN 4

#define BRICK_QUESTION_NOT_COIN 5

#define ADJUST_UP_DOWN 13

class CBrickQuestion : public CGameObject {

protected:
	float ay;

	float positionDefaultX, positionDefaultY;

	float minY;
	float startY;
	float startX;
	int model;
	BOOLEAN isUnbox = false;
	BOOLEAN isEmpty = false;



	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 0; }//!isUnbox || !isEmpty; }
	virtual int IsBlocking() { return 1; }
	virtual void OnNoCollision(DWORD dt);


public:
	CBrickQuestion(float positionDefaultX, float positionDefaultY) : CGameObject(x, y) {
		this->positionDefaultX = positionDefaultX;
		this->positionDefaultY = positionDefaultY;
	}

	CBrickQuestion(float x, float y, int model);

	virtual void SetState(int state);

	BOOLEAN GetIsUnbox() { return isUnbox; }
	BOOLEAN GetIsEmpty() { return isEmpty; }

	void SetIsUnbox(BOOLEAN b) { isUnbox = b; }
	void SetIsEmpty(BOOLEAN b) { isEmpty = b; }
	void SetModel(int model) { this->model = model; }
	int GetModel() { return model; }
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);


	float GetMinY() { return minY; }
	CGameObject* item = NULL;
};