/*
 * mygame.cpp: ���ɮ��x�C��������class��implementation
*/

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"

namespace game_framework
{
void CBouncingBall::SetXY(int x, int y)
{
    this->x = x;
    this->y = y;
}
void CBouncingBall::SetFloor(int floor)
{
    this->floor = floor;
}
void CBouncingBall::SetVelocity(int velocity)
{
    this->velocity = velocity;
    this->initial_velocity = velocity;
}

void CGameMap::OnMove()
{
}
CGameMap::~CGameMap()
{
    delete[] bballs;
}

CGameMap::CGameMap()
    : X(20), Y(40), MW(120), MH(100)
{
    random_num = 0;
    bballs = NULL;
}

void CGameMap::LoadBitmap()
{
}
void  CGameMap::OnShow()
{
    bg.ShowBitmap();
}

CPractice::CPractice()
{
    x = y = 0;
}
void CPractice::OnMove()
{
}
void CPractice::LoadBitmap()
{
}
void CPractice::OnShow()
{
}
CGameStateInit::CGameStateInit(CGame* g)
    : CGameState(g)
{
}
/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C���}�Y�e������
/////////////////////////////////////////////////////////////////////////////
void CGameStateInit::OnInit()
{
    //
    // ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
    //     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
    //
    ShowInitProgress(0);	// �@�}�l��loading�i�׬�0%
    //
    // �}�l���J���
    //
    logo.LoadBitmap(IDB_BACKGROUND);
    //
    // ��OnInit�ʧ@�|����CGameStaterRun::OnInit()�A�ҥH�i���٨S��100%
    //
}

void CGameStateInit::OnBeginState()
{
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_ESC = 27;
    const char KEY_SPACE = ' ';

    if (nChar == KEY_SPACE)
        GotoGameState(GAME_STATE_RUN);						// ������GAME_STATE_RUN
    else if (nChar == KEY_ESC)								// Demo �����C������k
        PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// �����C��
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
    GotoGameState(GAME_STATE_RUN);		// ������GAME_STATE_RUN
}

void CGameStateInit::OnShow()
{
    //
    // �K�Wlogo
    //
    logo.SetTopLeft((SIZE_X - logo.Width()) / 2, SIZE_Y / 8);
    logo.ShowBitmap();
    //
    // Demo�ù��r�����ϥΡA���L�}�o�ɽкɶq�קK�����ϥΦr���A���CMovingBitmap����n
    //
    CDC* pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC
    CFont f, *fp;
    f.CreatePointFont(160, "Times New Roman");	// ���� font f; 160���16 point���r
    fp = pDC->SelectObject(&f);					// ��� font f
    pDC->SetBkColor(RGB(0, 0, 0));
    pDC->SetTextColor(RGB(255, 255, 0));
    pDC->TextOut(120, 220, "Please click mouse or press SPACE to begin.");
    pDC->TextOut(5, 395, "Press Ctrl-F to switch in between window mode and full screen mode.");

    if (ENABLE_GAME_PAUSE)
        pDC->TextOut(5, 425, "Press Ctrl-Q to pause the Game.");

    pDC->TextOut(5, 455, "Press Alt-F4 or ESC to Quit.");
    pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
    CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
}

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����������A(Game Over)
/////////////////////////////////////////////////////////////////////////////

CGameStateOver::CGameStateOver(CGame* g)
    : CGameState(g)
{
}

void CGameStateOver::OnMove()
{
    counter--;

    if (counter < 0)
        GotoGameState(GAME_STATE_INIT);
}

void CGameStateOver::OnBeginState()
{
    counter = 30 * 5; // 5 seconds
}

void CGameStateOver::OnInit()
{
    //
    // ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
    //     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
    //
    ShowInitProgress(66);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���66%
    //
    // �}�l���J���
    // �̲׶i�׬�100%
    //
    ShowInitProgress(100);
}

void CGameStateOver::OnShow()
{
    CDC* pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC
    CFont f, *fp;
    f.CreatePointFont(160, "Times New Roman");	// ���� font f; 160���16 point���r
    fp = pDC->SelectObject(&f);					// ��� font f
    pDC->SetBkColor(RGB(0, 0, 0));
    pDC->SetTextColor(RGB(255, 255, 0));
    char str[80];								// Demo �Ʀr��r�ꪺ�ഫ
    sprintf(str, "Game Over ! (%d)", counter / 30);
    pDC->TextOut(240, 210, str);
    pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
    CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
}
/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame* g)
    : CGameState(g), NUMBALLS(0)
{
    ball = new CBall [NUMBALLS];
    picX = picY = 0;
}

CGameStateRun::~CGameStateRun()
{
    delete [] ball;
}

void CGameStateRun::OnBeginState()
{
    kirby.Initialize();
    CAudio::Instance()->Play(AUDIO_KIBY, true);			// ����d��D�D��
}
int GameAction = 0;
void CGameStateRun::OnMove()							// ���ʹC������
{
    #pragma region Level1
    //
    // ���ʥd��
    //
    kirby.OnMove();
    ////////////////////////////
    //�C���a�ϲ���
    ////////////////////////////

    if (kirby.GetX2() > 320 + 80 - (kirby.animation.Width() / 2) && kirby.isMovingRight == true && gamemap.x >= -5000)
    {
        kirby.x -= 10;
        gamemap.x -= 10;
        gamemap.SetTopLeft(gamemap.x, 0);
    }

    if (kirby.GetX2() < 320 - 80 + (kirby.animation.Width() / 2) && kirby.isMovingLeft == true && gamemap.x < 0)
    {
        kirby.x += 10;
        gamemap.x += 10;
        gamemap.SetTopLeft(gamemap.x, 0);
    }

    #pragma endregion
}

void CGameStateRun::OnInit()  								// �C������Ȥιϧγ]�w
{
    //
    // ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
    //     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
    //
    ShowInitProgress(33);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���33%
    //
    // �}�l���J���
    //
    kirby.LoadBitmap();
    gamemap.LoadBitmap(IDB_BG);
    //
    // ��������Loading�ʧ@�A�����i��
    //
    ShowInitProgress(50);
    //
    // �~����J��L���
    //
    CAudio::Instance()->Load(AUDIO_KIBY, "sounds\\kiby.wav"); // ���J����
    //
    // ��OnInit�ʧ@�|����CGameStaterOver::OnInit()�A�ҥH�i���٨S��100%
    //
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_LEFT  = 0x25; // keyboard���b�Y
    const char KEY_UP    = 0x26; // keyboard�W�b�Y
    const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
    const char KEY_DOWN  = 0x28; // keyboard�U�b�Y
    const char KEY_SPACE = 0x20; // �ť�����D�}�B

    if (nChar == KEY_LEFT)
        kirby.SetMovingLeft(true);

    if (nChar == KEY_RIGHT)
        kirby.SetMovingRight(true);

    if (nChar == KEY_UP)
        kirby.SetMovingUp(true);

    if (nChar == KEY_DOWN)
        kirby.SetMovingDown(true);

    if (nChar == KEY_SPACE)
    {
        kirby.SetMovingJump(true);
        kirby.SetMovingFly(true);
    }
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_LEFT  = 0x25; // keyboard���b�Y
    const char KEY_UP    = 0x26; // keyboard�W�b�Y
    const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
    const char KEY_DOWN  = 0x28; // keyboard�U�b�Y
    const char KEY_SPACE = 0x20; // �ť���}�B���A
    const char KEY_Z = 90;	// // z����D

    if (nChar == KEY_LEFT)
        kirby.SetMovingLeft(false);

    if (nChar == KEY_RIGHT)
        kirby.SetMovingRight(false);

    if (nChar == KEY_UP)
        kirby.SetMovingUp(false);

    if (nChar == KEY_DOWN)
        kirby.SetMovingDown(false);

    if (nChar == KEY_SPACE)
    {
        kirby.SetMovingFly(false);
        kirby.GoDown();
    }
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
{
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
{
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
{
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
{
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
{
}

void CGameStateRun::OnShow()
{
    //
    //  �`�N�GShow�̭��d�U���n���ʥ��󪫥󪺮y�СA���ʮy�Ъ��u�@����Move���~��A
    //        �_�h��������sø�Ϯ�(OnDraw)�A����N�|���ʡA�ݰ_�ӷ|�ܩǡC���ӳN�y
    //        ���AMove�t�dMVC����Model�AShow�t�dView�A��View�������Model�C
    gamemap.ShowBitmap();			// �K�W�I���a��
    kirby.OnShow();					// �K�W�d��
}

}