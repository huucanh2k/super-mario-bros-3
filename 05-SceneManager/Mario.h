#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.2f

#define MARIO_ACCEL_WALK_X	0.0005f
#define MARIO_ACCEL_RUN_X	0.0007f

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_RUN_SPEED_Y	0.6f

#define MARIO_GRAVITY			0.002f

#define MARIO_JUMP_DEFLECT_SPEED  0.4f

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301
#define MARIO_STATE_JUMP_AND_HOLDING	302
#define MARIO_STATE_RELEASE_JUMP_AND_HOLDING	303

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601
#define MARIO_STATE_SIT_AND_HOLD	602
#define MARIO_STATE_SIT_RELEASE_AND_HOLD	603

#define MARIO_STATE_IS_KICKING	700

#define MARIO_STATE_HOLDING_WALK_LEFT	800
#define MARIO_STATE_HOLDING_WALK_RIGHT	801
#define MARIO_STATE_HOLDING_IDLE_LEFT	802
#define MARIO_STATE_HOLDING_IDLE_RIGHT	803

#pragma region ANIMATION_ID

#define ID_ANI_MARIO_IDLE_RIGHT 400
#define ID_ANI_MARIO_IDLE_LEFT 401

#define ID_ANI_MARIO_WALKING_RIGHT 500
#define ID_ANI_MARIO_WALKING_LEFT 501

#define ID_ANI_MARIO_RUNNING_RIGHT 600
#define ID_ANI_MARIO_RUNNING_LEFT 601

#define ID_ANI_MARIO_JUMP_WALK_RIGHT 700
#define ID_ANI_MARIO_JUMP_WALK_LEFT 701

#define ID_ANI_MARIO_JUMP_RUN_RIGHT 800
#define ID_ANI_MARIO_JUMP_RUN_LEFT 801

#define ID_ANI_MARIO_SIT_RIGHT 900
#define ID_ANI_MARIO_SIT_LEFT 901

#define ID_ANI_MARIO_HOLDING_IDLE_LEFT 902
#define ID_ANI_MARIO_HOLDING_IDLE_RIGHT 903
#define ID_ANI_MARIO_HOLDING_WALK_LEFT 904
#define ID_ANI_MARIO_HOLDING_WALK_RIGHT 905

#define ID_ANI_MARIO_KICK_LEFT	950
#define ID_ANI_MARIO_KICK_RIGHT	951	


#define ID_ANI_MARIO_BRACE_LEFT 1000
#define ID_ANI_MARIO_BRACE_RIGHT 1001

#define ID_ANI_MARIO_DIE 999

//RACOON
#define ID_ANI_RACOON_IDLE_RIGHT 2401
#define ID_ANI_RACOON_IDLE_LEFT 2400

#define ID_ANI_RACOON_WALKING_RIGHT 2501
#define ID_ANI_RACOON_WALKING_LEFT 2500

#define ID_ANI_RACOON_RUNNING_RIGHT 2601
#define ID_ANI_RACOON_RUNNING_LEFT 2600

#define ID_ANI_RACOON_RUNNING_PREPARE_RIGHT 2603
#define ID_ANI_RACOON_RUNNING_PREPARE_LEFT 2602

#define ID_ANI_RACOON_JUMP_WALK_RIGHT 2701
#define ID_ANI_RACOON_JUMP_WALK_LEFT 2700

#define ID_ANI_RACOON_JUMP_RUN_RIGHT 2801
#define ID_ANI_RACOON_JUMP_RUN_LEFT 2800

#define ID_ANI_RACOON_SIT_RIGHT 2901
#define ID_ANI_RACOON_SIT_LEFT 2900

#define ID_ANI_RACOON_BRACE_RIGHT 3001
#define ID_ANI_RACOON_BRACE_LEFT 3000

#define ID_ANI_RACOON_HOLD_RUNNING_RIGHT 3011
#define ID_ANI_RACOON_HOLD_RUNNING_LEFT 3010

#define ID_ANI_RACOON_KICK_RIGHT 3021
#define ID_ANI_RACOON_KICK_LEFT 3020

#define ID_ANI_RACOON_HOLD_IDLE_RIGHT 3041
#define ID_ANI_RACOON_HOLD_IDLE_LEFT 3040

#define ID_ANI_RACOON_HOLD_JUMP_RIGHT 3051
#define ID_ANI_RACOON_HOLD_JUMP_LEFT 3050
#define ID_ANI_RACOON_HOLD_JUMP_LEFT 3050

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1100
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1102

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1200
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1201

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1300
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1301

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1400
#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1401

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 1500
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 1501

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 1600
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 1601

#define ID_ANI_MARIO_SMALL_HOLDING_IDLE_LEFT 1700
#define ID_ANI_MARIO_SMALL_HOLDING_IDLE_RIGHT 1701
#define ID_ANI_MARIO_SMALL_HOLDING_WALK_LEFT 1702
#define ID_ANI_MARIO_SMALL_HOLDING_WALK_RIGHT 1703

#define ID_ANI_MARIO_SMALL_KICK_LEFT 1800
#define ID_ANI_MARIO_SMALL_KICK_RIGHT 1801

#pragma endregion

#define GROUND_Y 160.0f

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define	MARIO_LEVEL_RACOON		3

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 12


#define MARIO_UNTOUCHABLE_TIME 2500
#define MARIO_KICKING_TIME 100

class CMario : public CGameObject
{
	BOOLEAN isSitting;
	BOOLEAN isHolding;
	BOOLEAN isHoldingShell;
	BOOLEAN isKicking;
	BOOLEAN isRunning;
	float maxVx;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 

	int level; 
	int untouchable; 
	ULONGLONG untouchable_start;
	ULONGLONG kick_start;
	BOOLEAN isOnPlatform;
	int coin; 
	

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithRedParaGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithRedKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithBrickQuestion(LPCOLLISIONEVENT e);
	void OnCollisionWithMushRoom(LPCOLLISIONEVENT e);
	void OnCollisionWithSuperLeaf(LPCOLLISIONEVENT e);

	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdRacoon();

public:
	CMario(float x, float y) : CGameObject(x, y)
	{
		isSitting = false;
		isHolding = false;
		isKicking = false;
		isRunning = false;
		isHoldingShell = false;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY; 

		level = MARIO_LEVEL_BIG;
		untouchable = 0;
		untouchable_start = -1;
		isOnPlatform = false;
		coin = 0;
		kick_start = -1;
	}
	
	float GetX();
	float GetY();

	int GetDirection();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	int IsCollidable()
	{ 
		return (state != MARIO_STATE_DIE); 
	}

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable==0); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void SetLevel(int l);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }
	void SetIsHolding(bool holding) { isHolding = holding; }
	void SetIsHoldingShell(bool holdingShell) { isHoldingShell = holdingShell; }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	bool GetIsHolding() { return isHolding; }
	bool GetIsHoldingShell() { return isHoldingShell; }
};