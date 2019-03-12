/*
 * mygame.cpp: 本檔案儲遊戲本身的class的implementation
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
// 這個class為遊戲的遊戲開頭畫面物件
/////////////////////////////////////////////////////////////////////////////
void CGameStateInit::OnInit()
{
    //
    // 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
    //     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
    //
    ShowInitProgress(0);	// 一開始的loading進度為0%
    //
    // 開始載入資料
    //
    logo.LoadBitmap(IDB_BACKGROUND);
    //
    // 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
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
        GotoGameState(GAME_STATE_RUN);						// 切換至GAME_STATE_RUN
    else if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
        PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
    GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
}

void CGameStateInit::OnShow()
{
    //
    // 貼上logo
    //
    logo.SetTopLeft((SIZE_X - logo.Width()) / 2, SIZE_Y / 8);
    logo.ShowBitmap();
    //
    // Demo螢幕字型的使用，不過開發時請盡量避免直接使用字型，改用CMovingBitmap比較好
    //
    CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
    CFont f, *fp;
    f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
    fp = pDC->SelectObject(&f);					// 選用 font f
    pDC->SetBkColor(RGB(0, 0, 0));
    pDC->SetTextColor(RGB(255, 255, 0));
    pDC->TextOut(120, 220, "Please click mouse or press SPACE to begin.");
    pDC->TextOut(5, 395, "Press Ctrl-F to switch in between window mode and full screen mode.");

    if (ENABLE_GAME_PAUSE)
        pDC->TextOut(5, 425, "Press Ctrl-Q to pause the Game.");

    pDC->TextOut(5, 455, "Press Alt-F4 or ESC to Quit.");
    pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
    CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
}

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
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
    // 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
    //     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
    //
    ShowInitProgress(66);	// 接個前一個狀態的進度，此處進度視為66%
    //
    // 開始載入資料
    // 最終進度為100%
    //
    ShowInitProgress(100);
}

void CGameStateOver::OnShow()
{
    CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
    CFont f, *fp;
    f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
    fp = pDC->SelectObject(&f);					// 選用 font f
    pDC->SetBkColor(RGB(0, 0, 0));
    pDC->SetTextColor(RGB(255, 255, 0));
    char str[80];								// Demo 數字對字串的轉換
    sprintf(str, "Game Over ! (%d)", counter / 30);
    pDC->TextOut(240, 210, str);
    pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
    CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
}
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
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
    CAudio::Instance()->Play(AUDIO_KIBY, true);			// 播放卡比主題曲
}
int GameAction = 0;
void CGameStateRun::OnMove()							// 移動遊戲元素
{
    #pragma region Level1
    //
    // 移動卡比
    //
    kirby.OnMove();
    ////////////////////////////
    //遊戲地圖移動
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

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
    //
    // 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
    //     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
    //
    ShowInitProgress(33);	// 接個前一個狀態的進度，此處進度視為33%
    //
    // 開始載入資料
    //
    kirby.LoadBitmap();
    gamemap.LoadBitmap(IDB_BG);
    //
    // 完成部分Loading動作，提高進度
    //
    ShowInitProgress(50);
    //
    // 繼續載入其他資料
    //
    CAudio::Instance()->Load(AUDIO_KIBY, "sounds\\kiby.wav"); // 載入音樂
    //
    // 此OnInit動作會接到CGameStaterOver::OnInit()，所以進度還沒到100%
    //
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_LEFT  = 0x25; // keyboard左箭頭
    const char KEY_UP    = 0x26; // keyboard上箭頭
    const char KEY_RIGHT = 0x27; // keyboard右箭頭
    const char KEY_DOWN  = 0x28; // keyboard下箭頭
    const char KEY_SPACE = 0x20; // 空白鍵跳躍漂浮

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
    const char KEY_LEFT  = 0x25; // keyboard左箭頭
    const char KEY_UP    = 0x26; // keyboard上箭頭
    const char KEY_RIGHT = 0x27; // keyboard右箭頭
    const char KEY_DOWN  = 0x28; // keyboard下箭頭
    const char KEY_SPACE = 0x20; // 空白鍵漂浮狀態
    const char KEY_Z = 90;	// // z鍵跳躍

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

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnShow()
{
    //
    //  注意：Show裡面千萬不要移動任何物件的座標，移動座標的工作應由Move做才對，
    //        否則當視窗重新繪圖時(OnDraw)，物件就會移動，看起來會很怪。換個術語
    //        說，Move負責MVC中的Model，Show負責View，而View不應更動Model。
    gamemap.ShowBitmap();			// 貼上背景地圖
    kirby.OnShow();					// 貼上卡比
}

}