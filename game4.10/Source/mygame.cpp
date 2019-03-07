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
void CGameMap::InitializeBouncingBall(int ini_index, int row, int col)
{
    const int VELOCITY = 10;
    const int BALL_PIC_HEIGHT = 15;
    int floor = Y + (row + 1) * MH - BALL_PIC_HEIGHT;
    bballs[ini_index].LoadBitmap();
    bballs[ini_index].SetFloor(floor);
    bballs[ini_index].SetVelocity(VELOCITY + col);
    bballs[ini_index].SetXY(X + col * MW + MW / 2, floor);
}
void CGameMap::RandomBouncingBall()
{
    const int MAX_RAND_NUM = 10;
    random_num = (rand() % MAX_RAND_NUM) + 1;
    bballs = new CBouncingBall[random_num];
    int ini_index = 0;

    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 5; col++)
        {
            if (map[row][col] != 0 && ini_index < random_num)
            {
                InitializeBouncingBall(ini_index, row, col);
                ini_index++;
            }
        }
}
void CGameMap::OnKeyDown(UINT nChar)
{
    const int KEY_SPACE = 0x20;

    if (nChar == KEY_SPACE)
        RandomBouncingBall();//�H���u���y
}
void CGameMap::OnMove()
{
    for (int i = 0; i < random_num; i++)
    {
        bballs[i].OnMove();
    }
}
CGameMap::~CGameMap()
{
    delete[] bballs;
}

CGameMap::CGameMap()
    : X(20), Y(40), MW(120), MH(100)
{
    /*
    int map_init[4][5] =
    {
        {0, 0, 1, 0, 0},
        {0, 1, 2, 1, 0},
        {1, 2, 1, 2, 1},
        {2, 1, 2, 1, 2}
    };

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 5; ++j)
            map[i][j] = map_init[i][j];
    */
    random_num = 0;
    bballs = NULL;
}

void CGameMap::LoadBitmap()
{
    orange.LoadBitmap(IDB_ORANGE, RGB(255, 255, 255));
    star.LoadBitmap(IDB_STAR, RGB(255, 255, 255));
}
void  CGameMap::OnShow()
{
    /* for (int i = 0; i < 5; ++i)
         for (int j = 0; j < 4; ++j)
         {
             switch (map[j][i])
             {
                 case 0:
                     break;

                 case 1:
                     orange.SetTopLeft(X + (MW * i), Y + (MH * j));
                     orange.ShowBitmap();
                     break;

                 case 2:
                     star.SetTopLeft(X + (MW * i), Y + (MH * j));
                     star.ShowBitmap();
                     break;

                 default:
                     ASSERT(0);
             }
         }*/
    for (int i = 0; i < random_num; i++)
    {
        bballs[i].OnShow();
    }
}

CPractice::CPractice()
{
    x = y = 0;
}
void CPractice::OnMove()
{
    if (y <= SIZE_Y)
    {
        x += 3;
        y += 3;
    }
    else
    {
        x = y = 0;
    }
}
void CPractice::LoadBitmap()
{
    pic.LoadBitmap(IDB_1);
}
void CPractice::OnShow()
{
    pic.SetTopLeft(x, y);
    pic.ShowBitmap();
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
    // Sleep(300);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
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
    //
    //Sleep(300);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
    //
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
    : CGameState(g), NUMBALLS(1)
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
    const int BALL_GAP = 90;
    const int BALL_XY_OFFSET = 45;
    const int BALL_PER_ROW = 7;
    const int HITS_LEFT = 3;
    const int HITS_LEFT_X = 590;
    const int HITS_LEFT_Y = 0;
    const int BACKGROUND_X = 60;
    const int ANIMATION_SPEED = 15;
    /*for (int i = 0; i < NUMBALLS; i++)  				// �]�w�y���_�l�y��
    {
        int x_pos = i % BALL_PER_ROW;
        int y_pos = i / BALL_PER_ROW;
        ball[i].SetXY(x_pos * BALL_GAP + BALL_XY_OFFSET, y_pos * BALL_GAP + BALL_XY_OFFSET);
        ball[i].SetDelay(x_pos);
        ball[i].SetIsAlive(true);
    }*/
    kirby.Initialize();
    background.SetTopLeft(BACKGROUND_X, 0);				// �]�w�I�����_�l�y��
    help.SetTopLeft(0, SIZE_Y - help.Height());			// �]�w�����Ϫ��_�l�y��
    hits_left.SetInteger(HITS_LEFT);					// ���w�ѤU��������
    hits_left.SetTopLeft(HITS_LEFT_X, HITS_LEFT_Y);		// ���w�ѤU�����ƪ��y��
    CAudio::Instance()->Play(AUDIO_KIBY, true);			// ����d��D�D��
}

void CGameStateRun::OnMove()							// ���ʹC������
{
    //
    // �p�G�Ʊ�ק�cursor���˦��A�h�N�U���{����commment�����Y�i
    //
    // SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
    //
    // ���ʭI���Ϫ��y��
    //
    gamemap.OnMove();

    if (picX <= SIZE_Y)
    {
        picX += 5;
        picY += 5;
    }
    else
    {
        picX = picY = 0;
    }

    if (background.Top() > SIZE_Y)
        background.SetTopLeft(60, -background.Height());

    background.SetTopLeft(background.Left(), background.Top() + 1);
    //
    // ���ʲy
    //
    int i;

    for (i = 0; i < NUMBALLS; i++)
        ball[i].OnMove();

    //
    // ���ʥd��
    //
    kirby.OnMove();

    //
    // �P�_���l�O�_�I��y
    //
    for (i = 0; i < NUMBALLS; i++)
        if (ball[i].IsAlive() && ball[i].HitKirby(&kirby))
        {
            ball[i].SetIsAlive(false);
            CAudio::Instance()->Play(AUDIO_DING);
            hits_left.Add(-1);

            //
            // �Y�Ѿl�I�����Ƭ�0�A�h����Game Over���A
            //
            if (hits_left.GetInteger() <= 0)
            {
                CAudio::Instance()->Stop(AUDIO_KIBY);	// �����
                GotoGameState(GAME_STATE_OVER);
            }
        }

    //
    // ���ʼu�����y
    //
    bball.OnMove();
}

void CGameStateRun::OnInit()  								// �C������Ȥιϧγ]�w
{
    //
    // ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
    //     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
    //
    ShowInitProgress(33);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���33%
    //���J�a��
    gamemap.LoadBitmap();
    //
    // �}�l���J���
    //
    int i;

    for (i = 0; i < NUMBALLS; i++)
        ball[i].LoadBitmap();								// ���J��i�Ӳy���ϧ�

    kirby.LoadBitmap();
    background.LoadBitmap(IDB_KIRBY);					// ���J�I�����ϧ�
    //
    // ��������Loading�ʧ@�A�����i��
    //
    ShowInitProgress(50);
    //Sleep(300); // ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
    //
    // �~����J��L���
    //
    help.LoadBitmap(IDB_HELP, RGB(255, 255, 255));				// ���J�������ϧ�
    corner.LoadBitmap(IDB_CORNER);							// ���J�����ϧ�
    corner.ShowBitmap(background);							// �Ncorner�K��background
    bball.LoadBitmap();										// ���J�ϧ�
    hits_left.LoadBitmap();
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

    if (nChar == KEY_LEFT)
        kirby.SetMovingLeft(true);

    if (nChar == KEY_RIGHT)
        kirby.SetMovingRight(true);

    if (nChar == KEY_UP)
        kirby.SetMovingUp(true);

    if (nChar == KEY_DOWN)
        kirby.SetMovingDown(true);

    gamemap.OnKeyDown(nChar);
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_LEFT  = 0x25; // keyboard���b�Y
    const char KEY_UP    = 0x26; // keyboard�W�b�Y
    const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
    const char KEY_DOWN  = 0x28; // keyboard�U�b�Y

    if (nChar == KEY_LEFT)
        kirby.SetMovingLeft(false);

    if (nChar == KEY_RIGHT)
        kirby.SetMovingRight(false);

    if (nChar == KEY_UP)
        kirby.SetMovingUp(false);

    if (nChar == KEY_DOWN)
        kirby.SetMovingDown(false);
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
{
    kirby.SetMovingLeft(true);
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
{
    kirby.SetMovingLeft(false);
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
{
    // �S�ơC�p�G�ݭn�B�z�ƹ����ʪ��ܡA�gcode�b�o��
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
{
    kirby.SetMovingRight(true);
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
{
    kirby.SetMovingRight(false);
}

void CGameStateRun::OnShow()
{
    //
    //  �`�N�GShow�̭��d�U���n���ʥ��󪫥󪺮y�СA���ʮy�Ъ��u�@����Move���~��A
    //        �_�h��������sø�Ϯ�(OnDraw)�A����N�|���ʡA�ݰ_�ӷ|�ܩǡC���ӳN�y
    //        ���AMove�t�dMVC����Model�AShow�t�dView�A��View�������Model�C
    //
    //
    //  �K�W�I���ϡB�����ơB�y�B���l�B�u�����y
    //
    gamemap.OnShow();
    background.ShowBitmap();			// �K�W�I����
    help.ShowBitmap();					// �K�W������
    hits_left.ShowBitmap();

    for (int i = 0; i < NUMBALLS; i++)
        ball[i].OnShow();				// �K�W��i���y

    bball.OnShow();						// �K�W�u�����y
    kirby.OnShow();					// �K�W�d��
    //
    //  �K�W���W�Υk�U��������
    //
    corner.SetTopLeft(0, 0);
    corner.ShowBitmap();
    corner.SetTopLeft(SIZE_X - corner.Width(), SIZE_Y - corner.Height());
}
}