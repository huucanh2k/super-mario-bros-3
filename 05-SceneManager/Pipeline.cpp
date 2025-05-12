#include "Pipeline.h"
#include "Animation.h"
#include "Animations.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Game.h"
#include "AssetIDs.h"



CPipeline::CPipeline(float x, float y, int model) :CGameObject(x, y)
{
	this->x = x;
	this->y = y;
	this->model = model;

}

void CPipeline::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (model == MODEL_L_PIPE) //L:LONG - M:MEDIUM
	{
		l = x - PIPE_BBOX_WIDTH / 2;
		t = y - PIPE_BBOX_L_HEIGHT / 2;
		r = l + PIPE_BBOX_WIDTH;
		b = t + PIPE_BBOX_L_HEIGHT;
	}
	else if (model == MODEL_EMPTY_PORTAL_PIPE)
	{
		l = x - PIPE_BBOX_WIDTH / 2;
		t = y - PIPE_BBOX_PORTAL_HEIGHT / 2;
		r = l + PIPE_BBOX_WIDTH;
		b = t + PIPE_BBOX_PORTAL_HEIGHT;
	}
	else {
		l = x - PIPE_BBOX_WIDTH / 2;
		t = y - PIPE_BBOX_M_HEIGHT / 2;
		r = l + PIPE_BBOX_WIDTH;
		b = t + PIPE_BBOX_M_HEIGHT;
	}
}




void CPipeline::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{


	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();


	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CPipeline::Render()
{

	CAnimations* animations = CAnimations::GetInstance();
	int aniId;
	if (model == MODEL_L_PIPE) aniId = ID_ANI_LONG_PIPE;
	else if (model == MODEL_M_PIPE) aniId = ID_ANI_MEDIUM_PIPE;
	else if (model == MODEL_EMPTY_M_PIPE) aniId = ID_ANI_EMPTY_M_PIPE;
	else if (model == MODEL_EMPTY_PORTAL_PIPE) aniId = ID_ANI_EMPTY_PORTAL_PIPE;
	else if (model == 6) aniId = 206;
	else if (model == 7) aniId = 207;
	else aniId = ID_ANI_EMPTY_PIPE;

	animations->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}