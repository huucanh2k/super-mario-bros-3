#pragma once
#include"GameObject.h"

#define ID_ANI_LONG_PIPE 202
#define ID_ANI_MEDIUM_PIPE 201
#define ID_ANI_EMPTY_PIPE 203
#define ID_ANI_EMPTY_M_PIPE 204
#define ID_ANI_EMPTY_PORTAL_PIPE 205
#define ID_ANI_EMPTY_PIPE_NOTACTIVE 206
#define ID_ANI_PIPE_HIDDEN_MAP 207

#define PIPE_BBOX_WIDTH 32
#define PIPE_BBOX_L_HEIGHT 48
#define PIPE_BBOX_M_HEIGHT 32
#define PIPE_BBOX_PORTAL_HEIGHT 192

#define MODEL_L_PIPE 1	//L:LONG
#define MODEL_M_PIPE 2
#define MODEL_EMPTY_PIPE 3	//M:MEDIUM
#define MODEL_EMPTY_M_PIPE 4
#define MODEL_EMPTY_PORTAL_PIPE 5 //PORTAL
#define MODE_6	6
#define MODE_HIDEN_MAP	7


class CPipeline : public CGameObject {
protected:
	int model;

	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void Update(DWORD dt) {}

	//CPlantShootRed* plantshoot;

	void CreatePlantShoot();

public:
	CPipeline(float x, float y, int model);

	virtual int IsColliable() { return 1; }
	virtual int IsBlocking() { return 1; }
	void SetModel(int model) { this->model = model; }
	int GetModel() { return model; }
};
