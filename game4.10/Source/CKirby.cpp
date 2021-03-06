#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CKirby.h"

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// CKirby: kirby class
/////////////////////////////////////////////////////////////////////////////

CKirby::CKirby()
{
    Initialize();
    const int INITIAL_VELOCITY = 20;	// 跳的高度
    const int INITIAL_FLY = 23;	// 漂浮最高的高度
    const int FLOOR = 400;				// 地板座標
    floor = FLOOR;
    x = 95;
    y = FLOOR - 1;				// y座標比地板高1點(站在地板上)
    rising = true;
    initial_velocity = INITIAL_VELOCITY;
    velocity = initial_velocity;
    initial_fly = INITIAL_FLY;
    fly = initial_fly;
}

int CKirby::GetX1()
{
    return x;
}

int CKirby::GetY1()
{
    return y;
}

int CKirby::GetX2()
{
    return x + animation.Width();
}

int CKirby::GetY2()
{
    return y + animation.Height();
}

void CKirby::Initialize()
{
    const int X_POS = 0;
    const int Y_POS = 400;
    x = X_POS;
    y = Y_POS;
    isMovingLeft = isMovingRight = isMovingUp = isMovingDown = isMovingJump = isMovingFly = false;
}

void CKirby::LoadBitmap()
{
    animation.AddBitmap(IDB_KIRBY);
}

void CKirby::OnMove()
{
    const int STEP_SIZE = 10;
    animation.OnMove();

    if (isMovingLeft)
        x -= STEP_SIZE;

    if (isMovingRight)
        x += STEP_SIZE;

    /*if (isMovingUp)
        y -= STEP_SIZE;

    if (isMovingDown)
        y += STEP_SIZE;*/
    if (isMovingJump)
        Jump();

    if (isMovingFly)
        Fly();
}

void CKirby::Jump()
{
    if (rising)  			// 上升狀態
    {
        if (velocity > 0)
        {
            y -= velocity;	// 當速度 > 0時，y軸上升(移動velocity個點，velocity的單位為 點/次)
            velocity--;		// 受重力影響，下次的上升速度降低
        }
        else
        {
            rising = false; // 當速度 <= 0，上升終止，下次改為下降
            velocity = 1;	// 下降的初速(velocity)為1
        }
    }
    else  				// 下降狀態
    {
        if (y < floor - 1)    // 當y座標還沒碰到地板
        {
            y += velocity;	// y軸下降(移動velocity個點，velocity的單位為 點/次)
            velocity++;		// 受重力影響，下次的下降速度增加
        }
        else
        {
            y = floor - 1;  // 當y座標低於地板，更正為地板上
            rising = true;	// 探底反彈，下次改為上升
            velocity = initial_velocity; // 重設上升初始速度
            isMovingJump = false;
        }
    }
}

void CKirby::Fly()
{
    if (flying)  			// 上升狀態
    {
        if (fly > 0)
        {
            y -= fly;	// 當速度 > 0時，y軸上升(移動velocity個點，velocity的單位為 點/次)
            fly--;		// 受重力影響，下次的上升速度降低
        }
        else
        {
            flying = false; // 當速度 <= 0，上升終止，下次改為下降
            fly = 1;	// 下降的初速(velocity)為1
        }
    }
}

void CKirby::GoDown()
{
    while (y < floor - 1)    // 當y座標還沒碰到地板
    {
        y += fly;	// y軸下降(移動velocity個點，velocity的單位為 點/次)
        fly++;		// 受重力影響，下次的下降速度增加
    }

    y = floor - 1;  // 當y座標低於地板，更正為地板上
    flying = true;	// 探底反彈，下次改為上升
    fly = initial_fly; // 重設上升初始速度
}

void CKirby::SetMovingDown(bool flag)
{
    isMovingDown = flag;
}

void CKirby::SetMovingLeft(bool flag)
{
    isMovingLeft = flag;
}

void CKirby::SetMovingRight(bool flag)
{
    isMovingRight = flag;
}

void CKirby::SetMovingUp(bool flag)
{
    isMovingUp = flag;
}

void CKirby::SetMovingJump(bool flag)
{
    isMovingJump = flag;
}

void CKirby::SetMovingFly(bool flag)
{
    isMovingFly = flag;
}

void CKirby::SetXY(int nx, int ny)
{
    x = nx;
    y = ny;
}

void CKirby::OnShow()
{
    animation.SetTopLeft(x, y);
    animation.OnShow();
}
}