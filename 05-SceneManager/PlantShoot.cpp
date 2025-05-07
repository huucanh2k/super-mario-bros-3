#include "PlantShoot.h"
#include "Game.h"
#include "PlayScene.h"
#include "GameObject.h"



CPlantShoot::CPlantShoot(float x, float y) :CGameObject(x, y)
{

	startY = y;
	loacationX = x;
	minY = startY - PLANT_BBOX_HEIGHT;

	isShoot = false;
}



void CPlantShoot::GetBoundingBox(float& l, float& t, float& r, float& b)
{

		l = x - PLANT_BBOX_WIDTH / 2;
		t = y - PLANT_BBOX_HEIGHT / 3;
		r = l + PLANT_BBOX_WIDTH;
		b = t + PLANT_BBOX_HEIGHT;
}

void CPlantShoot::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CPlantShoot::OnNoCollision(DWORD dt) {
	x += vx * dt;
	y += vy * dt;
}

float CPlantShoot::distanceMario_PlantEnemies() {
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	return abs(mario->GetX() - x);

}

int CPlantShoot::LeftORightMario() {
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetX() < GetX()) //Mario ben trai cai cay
	{
		return 1;
	}
	else return -1;
}

int CPlantShoot::TopOrBottomYMario()
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetY() < GetY()) //Mario o phia tren cai cay 
	{
		return 1;
	}
	else return -1;
}

void CPlantShoot::Render()
{

	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;


	if (LeftORightMario() == 1 && TopOrBottomYMario() == -1)
		if (!isShoot) aniId = ID_ANI_PLANT_LEFT_UNDER_NOT_SHOOT;
		else {
			aniId = ID_ANI_PLANT_LEFT_UNDER_SHOOT;
		}
	else if (LeftORightMario() == 1 && TopOrBottomYMario() == 1)
		if (!isShoot) aniId = ID_ANI_PLANT_LEFT_TOP_NOT_SHOOT;
		else aniId = ID_ANI_PLANT_LEFT_TOP_SHOOT;
	else if (LeftORightMario() == -1 && TopOrBottomYMario() == 1)
		if (!isShoot) aniId = ID_ANI_PLANT_RIGHT_TOP_NOT_SHOOT;
		else aniId = ID_ANI_PLANT_RIGHT_TOP_SHOOT;
	else {
		if (!isShoot) aniId = ID_ANI_PLANT_RIGHT_UNDER_NOT_SHOOT;
		else aniId = ID_ANI_PLANT_RIGHT_UNDER_SHOOT;
	}

	animations->Get(aniId)->Render(x, y);
}

void CPlantShoot::SetState(int state)
{
}
