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
    const int INITIAL_VELOCITY = 20;	// ��������
    const int INITIAL_FLY = 23;	// �}�B�̰�������
    const int FLOOR = 400;				// �a�O�y��
    floor = FLOOR;
    x = 95;
    y = FLOOR - 1;				// y�y�Ф�a�O��1�I(���b�a�O�W)
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
    if (rising)  			// �W�ɪ��A
    {
        if (velocity > 0)
        {
            y -= velocity;	// ��t�� > 0�ɡAy�b�W��(����velocity���I�Avelocity����쬰 �I/��)
            velocity--;		// �����O�v�T�A�U�����W�ɳt�׭��C
        }
        else
        {
            rising = false; // ��t�� <= 0�A�W�ɲפ�A�U���אּ�U��
            velocity = 1;	// �U������t(velocity)��1
        }
    }
    else  				// �U�����A
    {
        if (y < floor - 1)    // ��y�y���٨S�I��a�O
        {
            y += velocity;	// y�b�U��(����velocity���I�Avelocity����쬰 �I/��)
            velocity++;		// �����O�v�T�A�U�����U���t�׼W�[
        }
        else
        {
            y = floor - 1;  // ��y�y�ЧC��a�O�A�󥿬��a�O�W
            rising = true;	// �����ϼu�A�U���אּ�W��
            velocity = initial_velocity; // ���]�W�ɪ�l�t��
            isMovingJump = false;
        }
    }
}

void CKirby::Fly()
{
    if (flying)  			// �W�ɪ��A
    {
        if (fly > 0)
        {
            y -= fly;	// ��t�� > 0�ɡAy�b�W��(����velocity���I�Avelocity����쬰 �I/��)
            fly--;		// �����O�v�T�A�U�����W�ɳt�׭��C
        }
        else
        {
            flying = false; // ��t�� <= 0�A�W�ɲפ�A�U���אּ�U��
            fly = 1;	// �U������t(velocity)��1
        }
    }
}

void CKirby::GoDown()
{
    while (y < floor - 1)    // ��y�y���٨S�I��a�O
    {
        y += fly;	// y�b�U��(����velocity���I�Avelocity����쬰 �I/��)
        fly++;		// �����O�v�T�A�U�����U���t�׼W�[
    }

    y = floor - 1;  // ��y�y�ЧC��a�O�A�󥿬��a�O�W
    flying = true;	// �����ϼu�A�U���אּ�W��
    fly = initial_fly; // ���]�W�ɪ�l�t��
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