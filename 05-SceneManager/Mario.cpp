#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Koopa.h"
#include "RedKoopa.h"
#include "RedParaGoomba.h"
#include "Coin.h"
#include "Portal.h"
#include "BrickQuestion.h"
#include "PlayScene.h"
#include "MushRoom.h"
#include "SuperLeaf.h"
#include "PlantShoot.h"
#include "BulletPlant.h"

#include "Collision.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (abs(vx) > abs(maxVx)) vx = maxVx;

	if (isKicking && GetTickCount64() - kick_start > MARIO_KICKING_TIME) {
		state = MARIO_STATE_IDLE;
		isKicking = false;
		DebugOut(L">>> Check kicking");
	}

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

float CMario::GetX()
{
	return x;
}

float CMario::GetY()
{
	return y;
}

int CMario::GetDirection() {
	if (state == MARIO_STATE_WALKING_RIGHT)
		return 1;
	else if (state == MARIO_STATE_WALKING_LEFT)
		return -1;
	else
		return 0;
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	isOnPlatform = false;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else 
	if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CRedParaGoomba*>(e->obj))
		OnCollisionWithRedParaGoomba(e);
	if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
	else if (dynamic_cast<CRedKoopa*>(e->obj))
		OnCollisionWithRedKoopa(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CBrickQuestion*>(e->obj))
		OnCollisionWithBrickQuestion(e);
	else if (dynamic_cast<CMushRoom*>(e->obj))
		OnCollisionWithMushRoom(e);
	else if (dynamic_cast<CSuperLeaf*>(e->obj))
		OnCollisionWithSuperLeaf(e);
	else if (dynamic_cast<CPlantShoot*>(e->obj))
		OnCollisionWithPlantShoot(e);
	else if (dynamic_cast<CBulletPlant*>(e->obj))
		OnCollisionWithFireBulletPlant(e);

}

void CMario::OnCollisionWithBrickQuestion(LPCOLLISIONEVENT e)
{
	CBrickQuestion* brick_question = dynamic_cast<CBrickQuestion*>(e->obj);
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	int x = brick_question->GetX();
	int y = brick_question->GetY();

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny > 0) {
		if (!brick_question->GetIsEmpty()) {
			brick_question->SetState(BRICK_QUESTION_STATE_UP);
			brick_question->SetIsUnbox(true);
			if (brick_question->GetModel() == BRICK_QUESTION_COIN) {
				CCoin* coinSummon = new CCoin(x, y - (BRICK_Q_BBOX_HEIGHT - ADJUST_UP_DOWN));
				scene->AddObject(coinSummon);
				coinSummon->SetState(COIN_SUMMON_STATE);
				coin += 50;
			}
			else if (brick_question->GetModel() == BRICK_QUESTION_NOT_COIN) {
				if (level == MARIO_LEVEL_SMALL) {
					CMushRoom* mushroom = new CMushRoom(x, y - (BRICK_Q_BBOX_HEIGHT - ADJUST_UP_DOWN), MODE_RED);
					scene->AddObject(mushroom);
				}
				else if (level == MARIO_LEVEL_BIG) {
					CSuperLeaf* leaf = new CSuperLeaf(x, y - (BRICK_Q_BBOX_HEIGHT - ADJUST_UP_DOWN));
					scene->AddObject(leaf);
					leaf->SetState(LEAF_SUMMON_STATE);
				}
			}
		}
	}

}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
			coin += 100;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithRedParaGoomba(LPCOLLISIONEVENT e)
{
	CRedParaGoomba* redParaGoomba = dynamic_cast<CRedParaGoomba*>(e->obj);
	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (redParaGoomba->GetState() != RED_PARA_GOOMBA_STATE_DIE)
		{
			if (redParaGoomba->GetIsWinged()) {
				redParaGoomba->SetIsWinged(false);
			}
			else {
				redParaGoomba->SetState(RED_PARA_GOOMBA_STATE_DIE);
			}
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		coin += 100;
	}
	else 
	{
		if (untouchable == 0)
		{
			if (redParaGoomba->GetState() != RED_PARA_GOOMBA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	// jump on top >> turn koopa into shell state 
	if (e->ny < 0)
	{
		if (koopa->GetState() == KOOPA_STATE_SHELL) {
			if (e->nx < 0) {
				koopa->SetState(KOOPA_STATE_SHELL_FAST_MOVING_RIGHT);
			}
			else {
				koopa->SetState(KOOPA_STATE_SHELL_FAST_MOVING_LEFT);
			}
			vy = -MARIO_JUMP_DEFLECT_SPEED;
			return;
		}

		if (koopa->GetState() != KOOPA_STATE_DIE && koopa->GetState() != KOOPA_STATE_SHELL)
		{
			koopa->SetState(KOOPA_STATE_SHELL);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}

		coin += 100;
		 
	}
	else if (e->ny > 0)
	{
		if (koopa->GetState() == KOOPA_STATE_WALKING_LEFT || koopa->GetState() == KOOPA_STATE_WALKING_RIGHT)
		{
			if (e->ny > 0)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
	else if (e->nx != 0 && koopa->GetState() == KOOPA_STATE_SHELL) //Mario hit koopa shell
	{

		if (e->nx < 0) {
			koopa->SetState(KOOPA_STATE_SHELL_FAST_MOVING_RIGHT);
		}
		else {
			koopa->SetState(KOOPA_STATE_SHELL_FAST_MOVING_LEFT);
		}
	}
	else // hit by Koopa
	{
		if (untouchable == 0)
		{
			if (koopa->GetState() == KOOPA_STATE_DIE || koopa->GetState() == KOOPA_STATE_SHELL || koopa->GetState() == KOOPA_STATE_SHELL_HOLD)
				return;
			
			if (level > MARIO_LEVEL_SMALL)
			{
				level = MARIO_LEVEL_SMALL;
				StartUntouchable();
			}
			else
			{
				DebugOut(L">>> Mario DIE >>> \n");
				SetState(MARIO_STATE_DIE);
			}
		}
	}
}

void CMario::OnCollisionWithRedKoopa(LPCOLLISIONEVENT e) {
	CRedKoopa* redKoopa = dynamic_cast<CRedKoopa*>(e->obj);

	// Mario hit on the koopa's head
	if (e->ny < 0)
	{
		if (redKoopa->GetState() == KOOPA_STATE_WALKING_LEFT || redKoopa->GetState() == KOOPA_STATE_WALKING_RIGHT)
		{	
			redKoopa->SetState(KOOPA_STATE_SHELL);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (redKoopa->GetState() == KOOPA_STATE_SHELL) {
			if (e->nx < 0) {
				redKoopa->SetState(KOOPA_STATE_SHELL_FAST_MOVING_RIGHT);
			}
			else {
				redKoopa->SetState(KOOPA_STATE_SHELL_FAST_MOVING_LEFT);
			}
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (redKoopa->GetState() == KOOPA_STATE_SHELL_FAST_MOVING_RIGHT || redKoopa->GetState() == KOOPA_STATE_SHELL_FAST_MOVING_LEFT)
		{
			redKoopa->SetState(KOOPA_STATE_SHELL);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}

		coin += 100;
	}
	else if (e->ny > 0)
	{
		
	}
	else {
		if (untouchable == 0)
		{
			if (redKoopa->GetState() == KOOPA_STATE_DIE || redKoopa->GetState() == KOOPA_STATE_SHELL || redKoopa->GetState() == KOOPA_STATE_SHELL_HOLD)
				return;

			if (level > MARIO_LEVEL_SMALL)
			{
				level = MARIO_LEVEL_SMALL;
				StartUntouchable();
			}
			else
			{
				DebugOut(L">>> Mario DIE >>> \n");
				SetState(MARIO_STATE_DIE);
			}
		}
	}
	
}

void CMario::OnCollisionWithMushRoom(LPCOLLISIONEVENT e)
{
	CMushRoom* mush_room = (CMushRoom*)e->obj;


	if (level == MARIO_LEVEL_SMALL) {
		SetLevel(MARIO_LEVEL_BIG);
	}
	e->obj->Delete();
}

void CMario::OnCollisionWithSuperLeaf(LPCOLLISIONEVENT e)
{
	CSuperLeaf* superLeaf = (CSuperLeaf*)e->obj;


	if (level == MARIO_LEVEL_BIG) {
		SetLevel(MARIO_LEVEL_RACOON);
	}
	e->obj->Delete();
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin += 50;
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

void CMario::OnCollisionWithPlantShoot(LPCOLLISIONEVENT e)
{
	CPlantShoot* pshootred = dynamic_cast<CPlantShoot*>(e->obj);



		if (untouchable == 0) {
			if (pshootred->GetState() != PLANT_STATE_NOT_TOUCH) {
				if (level > MARIO_LEVEL_SMALL)
				{

					if (level > MARIO_LEVEL_BIG)
					{
						level = MARIO_LEVEL_BIG;
						StartUntouchable();
					}
					else
					{
						level = MARIO_LEVEL_SMALL;


						StartUntouchable();
					}
				}
				else
				{
					DebugOut(L">>> Mario DIE by Plant Enemies >>> \n");
					SetState(MARIO_STATE_DIE);
					//isDeleted = true; sai
				}
			}
		}
}

void CMario::OnCollisionWithFireBulletPlant(LPCOLLISIONEVENT e)
{
	CBulletPlant* fire_bullet = dynamic_cast<CBulletPlant*>(e->obj);
	if (untouchable == 0) {
		if (level > MARIO_LEVEL_SMALL)
		{
			if (level > MARIO_LEVEL_BIG)
			{
				level = MARIO_LEVEL_BIG;
				StartUntouchable();
			}
			else
			{
				level = MARIO_LEVEL_SMALL;


				StartUntouchable();
			}
		}
		else
		{
			DebugOut(L">>> Mario DIE by Fire_Bullet >>> \n");
			SetState(MARIO_STATE_DIE);
		}
		e->obj->Delete();
	}

}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		} 
		else if (isKicking) {
			if (nx > 0)
				aniId = ID_ANI_MARIO_SMALL_KICK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_KICK_LEFT;
		}
		else if (isHoldingShell) {
			if (vx != 0) {
				if (nx > 0)
					aniId = ID_ANI_MARIO_SMALL_HOLDING_WALK_RIGHT;
				else
					aniId = ID_ANI_MARIO_SMALL_HOLDING_WALK_LEFT;
			}
			else {
				aniId = ID_ANI_MARIO_SMALL_HOLDING_IDLE_RIGHT;
			}
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else if (isKicking) {
			if (nx > 0)
				aniId = ID_ANI_MARIO_KICK_RIGHT;
			else
				aniId = ID_ANI_MARIO_KICK_LEFT;
		}
		else if (isHoldingShell) {
			if (vx != 0) {
				if (nx > 0)
					aniId = ID_ANI_MARIO_HOLDING_WALK_RIGHT;
				else
					aniId = ID_ANI_MARIO_HOLDING_WALK_LEFT;
			}
			else {
				aniId = ID_ANI_MARIO_HOLDING_IDLE_RIGHT;
			}
		}	
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}

int CMario::GetAniIdRacoon()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_RACOON_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_RACOON_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_RACOON_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_RACOON_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_RACOON_SIT_RIGHT;
			else
				aniId = ID_ANI_RACOON_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_RACOON_IDLE_RIGHT;
				else aniId = ID_ANI_RACOON_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_RACOON_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_RACOON_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_RACOON_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_RACOON_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_RACOON_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_RACOON_WALKING_LEFT;
			}

	DebugOut(L"aniId = %lld\n", aniId);

	if (aniId == -1) aniId = ID_ANI_RACOON_IDLE_RIGHT;

	return aniId;
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	else if (level == MARIO_LEVEL_RACOON)
		aniId = GetAniIdRacoon();
	animations->Get(aniId)->Render(x, y);

	//RenderBoundingBox();
	
	DebugOutTitle(L"Coins: %d", coin);
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return; 

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y +=MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_HOLDING_IDLE_LEFT:
		if (isSitting) break;
		maxVx = 0;
		ax = 0;
		nx = -1;
		break;
	
	case MARIO_STATE_HOLDING_IDLE_RIGHT:
		if (isSitting) break;
		maxVx = 0;
		ax = 0;
		nx = 1;
		break;

	case MARIO_STATE_HOLDING_WALK_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;	

	case MARIO_STATE_HOLDING_WALK_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;

	case MARIO_STATE_IS_KICKING:
		if (isSitting) break;
		isKicking = true;
		kick_start = GetTickCount64();
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level==MARIO_LEVEL_BIG || level== MARIO_LEVEL_RACOON)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else 
		{
			left = x - MARIO_BIG_BBOX_WIDTH/2;
			top = y - MARIO_BIG_BBOX_HEIGHT/2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH/2;
		top = y - MARIO_SMALL_BBOX_HEIGHT/2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}

