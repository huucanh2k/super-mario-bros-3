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
	if (isUp) {
		if (y > minY)
		{
			vy = -SPEED;
		}
		else {

			vy = 0;
			y = minY;
			isShoot = true;

			if (GetTickCount64() - time_out_pipe > TIME_OUT_PIPE)
			{


				//DebugOut(L">>> CAY LAN XUONG lan thu n >>> \n");

				SetState(PLANT_STATE_DOWN);

			}
		}
	}
	else if (isDown) {

		if (y < startY)
		{
			vy = SPEED;
		}
		else {
			//IsActive = false;
			vy = 0;
			y = startY;
			isShoot = true;
			if (GetTickCount64() - time_down_pipe > TIME_IN_PIPE) {

				if ((distanceMario_PlantEnemies() > DISTANCE_MIN_MARIO_PLANT))
				{


					SetState(PLANT_STATE_UP);
				}
				else {

					SetState(PLANT_STATE_NOT_TOUCH);
				}
			}
		}
	}
	else {
		if (y < startY)
		{
			vy = SPEED;
		}
		else {
			vy = 0;
			y = startY;

			if (GetTickCount64() - time_down_pipe > TIME_IN_PIPE_START) {

				SetState(PLANT_STATE_UP);
			}
		}
	}
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
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (state)
	{
	case PLANT_STATE_UP:
		if (distanceMario_PlantEnemies() < DISTANCE_PLANT_ENEMIS_MAYBE_SHOW_UP)
		{
			isUp = true;

		}
		else
		{
			isUp = false;
		}
		isDown = false;
		isShoot = false;
		time_out_pipe = GetTickCount64();
		time_down_pipe = 0;
		break;

	case PLANT_STATE_DOWN:


		isShoot = false;
		isUp = false;
		isDown = true;
		time_down_pipe = GetTickCount64();
		time_out_pipe = 0;
		break;
	}
	CGameObject::SetState(state);
}
