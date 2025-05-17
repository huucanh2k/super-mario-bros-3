#pragma once

#include "GameObject.h"

// 
// The most popular type of object in Mario! 
// 

#define PER_BOX_WIDTH 16
#define PER_BOX_HEIGHT 16

#define ID_ANI_BOX 10000


class CBox : public CGameObject
{
protected:
	// int length;				// Unit: cell 
	// float cellWidth;
	// float cellHeight;
	// int spriteIdBegin, spriteIdMiddle, spriteIdEnd;
	int rowNum, columnNum;

public:
	CBox(float x, float y, int rowNum, int columnNum) :CGameObject(x, y)
	{
		this->x = x;
		this->y = y;
		this->rowNum = rowNum;
		this->columnNum = columnNum;
	}

	float GetStartX() { return x; }
	float GetEndX() { return x + rowNum * PER_BOX_WIDTH; }

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();

	int IsDirectionColliable(float nx, float ny);
};

typedef CBox* LPBOX;