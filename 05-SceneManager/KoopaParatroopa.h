#pragma once

#include "GameObject.h"
#include "PlayScene.h"

#define KOOPA_PARATROOPA_GRAVITY 0.002f
#define KOOPA_PARATROOPA_WALKING_SPEED 0.05f
#define KOOPA_PARATROOPA_SHELL_MOVING_SPEED 0.2f

// Define models
#define KOOPA_PARATROOPA_MODEL_GREEN 0
#define KOOPA_PARATROOPA_MODEL_RED 1

// Bounding box dimensions
#define KOOPA_PARATROOPA_BBOX_WIDTH 14
#define KOOPA_PARATROOPA_BBOX_HEIGHT 18
#define KOOPA_PARATROOPA_BBOX_HEIGHT_DIE 14
#define KOOPA_PARATROOPA_BBOX_HEIGHT_WINGED 24
#define KOOPA_PARATROOPA_SHELL_BBOX_WIDTH 16
#define KOOPA_PARATROOPA_SHELL_BBOX_HEIGHT 14

// Timeouts
#define KOOPA_PARATROOPA_DIE_TIMEOUT 500
#define KOOPA_PARATROOPA_SHELL_TIMEOUT 6000
#define KOOPA_PARATROOPA_JUMP_TIMEOUT 700

// Vertical flying limits for Red Koopa Paratroopa
#define KOOPA_RED_PARATROOPA_FLY_HEIGHT_MAX 32.0f
#define KOOPA_RED_PARATROOPA_FLY_HEIGHT_MIN 0.0f
#define KOOPA_RED_PARATROOPA_FLY_SPEED 0.08f

// Jump speed for Green Koopa Paratroopa
#define KOOPA_GREEN_PARATROOPA_JUMP_SPEED 0.35f
#define KOOPA_GREEN_PARATROOPA_HOP_SPEED 0.2f

// States
#define KOOPA_PARATROOPA_STATE_WALKING 100
#define KOOPA_PARATROOPA_STATE_JUMPING 200
#define KOOPA_PARATROOPA_STATE_FALLING 300
#define KOOPA_PARATROOPA_STATE_SHELL 400
#define KOOPA_PARATROOPA_STATE_DIE 500
#define KOOPA_PARATROOPA_STATE_SHELL_MOVING 600
#define KOOPA_PARATROOPA_STATE_SHELL_HOLD 700
#define KOOPA_PARATROOPA_STATE_FLYING_UP 800
#define KOOPA_PARATROOPA_STATE_FLYING_DOWN 900

// Animation IDs
#define ID_ANI_GREEN_PARATROOPA_WINGED_WALKING 6200
#define ID_ANI_GREEN_PARATROOPA_WINGED_JUMPING 6201
#define ID_ANI_GREEN_PARATROOPA_WINGED_FALLING 6202

#define ID_ANI_RED_PARATROOPA_WINGED_FLYING 6250
#define ID_ANI_RED_PARATROOPA_WINGED_FLYING_DOWN 6251

#define ID_ANI_KOOPA_PARATROOPA_WALKING 6300
#define ID_ANI_KOOPA_PARATROOPA_SHELL 6301
#define ID_ANI_KOOPA_PARATROOPA_SHELL_MOVING 6302
#define ID_ANI_KOOPA_PARATROOPA_DIE 6303

class CKoopaParatroopa : public CGameObject
{
protected:
    float ax;
    float ay;

    ULONGLONG jump_start;
    ULONGLONG die_start;
    ULONGLONG shell_start;
    
    int model;
    bool isOnPlatform;
    bool isWinged;
    float startY;  // Starting Y position for red paratroopa to maintain vertical limits
    int hopCount;  // Counter for green paratroopa hop pattern

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render();

    virtual int IsCollidable() { return 1; };
    virtual int IsBlocking() { return 0; }

    virtual void OnNoCollision(DWORD dt);
    virtual void OnCollisionWith(LPCOLLISIONEVENT e);
    virtual void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
    virtual void OnCollisionWithBrick(LPCOLLISIONEVENT e);

public:
    CKoopaParatroopa(float x, float y, int model);
    virtual void SetState(int state);
    bool GetIsWinged() { return isWinged; }
    void SetIsWinged(bool winged) { isWinged = winged; }
    int GetModel() { return model; }

    int LeftOrRightMarrio();
};