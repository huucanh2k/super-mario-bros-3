#pragma once

#include "GameObject.h"
#include "PlayScene.h"

#define WINGED_KOOPA_GRAVITY 0.002f
#define WINGED_KOOPA_WALKING_SPEED 0.05f
#define WINGED_KOOPA_SHELL_MOVING_SPEED 0.2f
#define WINGED_KOOPA_JUMP_SPEED 0.1f

// Define models
#define WINGED_KOOPA_MODEL_GREEN 0
#define WINGED_KOOPA_MODEL_RED 1

// Bounding box dimensions
#define WINGED_KOOPA_BBOX_WIDTH 14
#define WINGED_KOOPA_BBOX_HEIGHT 18
#define WINGED_KOOPA_BBOX_HEIGHT_DIE 14
#define WINGED_KOOPA_BBOX_HEIGHT_WINGED 24
#define WINGED_KOOPA_SHELL_BBOX_WIDTH 16
#define WINGED_KOOPA_SHELL_BBOX_HEIGHT 14

// Timeouts
#define WINGED_KOOPA_DIE_TIMEOUT 500
#define WINGED_KOOPA_SHELL_TIMEOUT 6000
#define WINGED_KOOPA_SHELL_SHAKING_TIMEOUT 2000
#define WINGED_KOOPA_JUMP_TIMEOUT 500

// Vertical flying limits for Red Koopa Paratroopa
#define KOOPA_RED_PARATROOPA_FLY_HEIGHT_MAX 32.0f
#define KOOPA_RED_PARATROOPA_FLY_HEIGHT_MIN 0.0f
#define KOOPA_RED_PARATROOPA_FLY_SPEED 0.08f

// Jump speed for Green Koopa Paratroopa
#define KOOPA_GREEN_PARATROOPA_JUMP_SPEED 0.05f
#define KOOPA_GREEN_PARATROOPA_FALL_SPEED 0.05f
#define KOOPA_GREEN_PARATROOPA_JUMP_HEIGHT 60.0f // Maximum height for green paratroopa jumps

// States
#define WINGED_KOOPA_STATE_WALKING 100
#define WINGED_KOOPA_STATE_JUMPING 200
#define WINGED_KOOPA_STATE_FALLING 300
#define WINGED_KOOPA_STATE_SHELL 400
#define WINGED_KOOPA_STATE_DIE 500
#define WINGED_KOOPA_STATE_SHELL_MOVING 600
#define WINGED_KOOPA_STATE_SHELL_HOLD 700
#define WINGED_KOOPA_STATE_FLYING_UP 800
#define WINGED_KOOPA_STATE_FLYING_DOWN 900
#define WINGED_KOOPA_STATE_SHELL_SHAKING 1000

// Animation IDs
#define ID_ANI_GREEN_WINGED_KOOPA_WALKING 9000
#define ID_ANI_GREEN_WINGED_KOOPA_JUMPING 9001
#define ID_ANI_GREEN_WINGED_KOOPA_FALLING 9002

#define ID_ANI_RED_WINGED_KOOPA_FLYING 9100
#define ID_ANI_RED_WINGED_KOOPA_FLYING_DOWN 9101

#define ID_ANI_WINGED_KOOPA_WALKING 9200
#define ID_ANI_WINGED_KOOPA_SHELL 9201
#define ID_ANI_WINGED_KOOPA_SHELL_MOVING 9202
#define ID_ANI_WINGED_KOOPA_DIE 9203
#define ID_ANI_WINGED_KOOPA_SHELL_SHAKING 9204

class CWingedKoopa : public CGameObject
{
protected:
    float ax;
    float ay;

    ULONGLONG jump_start;
    ULONGLONG die_start;
    ULONGLONG shell_start;
    ULONGLONG stateShakingStart;
    
    int model;
    bool isOnPlatform;
    bool isWinged;
    bool isHeld;  // Flag to check if the Koopa is held by Mario
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
    CWingedKoopa(float x, float y, int model);
    virtual void SetState(int state);
    bool SetIsHeld(bool isHeld) { this->isHeld = isHeld; }
    bool GetIsHeld() { return isHeld; }
    bool GetIsWinged() { return isWinged; }
    void SetIsWinged(bool winged) { isWinged = winged; }
    int GetModel() { return model; }

};