#pragma once

#include "GameObject.h"

/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
	int scene_id;	// target scene to switch to 

	float width;
	float height; 
public:
	CPortal(float l, float t, float r, float b, int scene_id);
	virtual void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void Reload();

	void SetActive(bool isActive);

	void RenderBoundingBox(void);
	
	int GetSceneId() { return scene_id;  }
	int IsBlocking() { return 0; }
	int IsCollidable() { return 1; }
	int IsTangible() { return 0; }
};