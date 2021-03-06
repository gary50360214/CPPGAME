/*
 * mygame.h: 本檔案儲遊戲本身的class的interface
*/

#include "CKirby.h"
#include "CBall.h"
#include "CBouncingBall.h"

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////

enum AUDIO_ID  				// 定義各種音效的編號
{
    AUDIO_DING,				// 0
    AUDIO_LAKE,				// 1
    AUDIO_NTUT,				// 2
    AUDIO_KIBY				// 3
};

/////////////////////////////////////////////////////////////////////////////
// 這個class提供地圖
/////////////////////////////////////////////////////////////////////////////

class CGameMap
{
    public:
        CGameMap();
        void LoadBitmap();
        void OnShow();
        void OnMove();

        ~CGameMap();
    private:

        CMovingBitmap orange, star, bg;
        int map[4][5];
        const int X, Y;
        const int MW, MH;
        CBouncingBall* bballs;
        int random_num;
};
class CPractice
{
    public:
        CPractice();
        void LoadBitmap();
        void OnMove();
        void OnShow();
    private:
        CMovingBitmap pic;
        int x, y;
};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateInit : public CGameState
{
    public:
        CGameStateInit(CGame* g);
        void OnInit();  								// 遊戲的初值及圖形設定
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
        void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
    protected:
        void OnShow();									// 顯示這個狀態的遊戲畫面
    private:
        CMovingBitmap logo;								// csie的logo
};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateRun : public CGameState
{
    public:
        CGameStateRun(CGame* g);
        ~CGameStateRun();
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnInit();  								// 遊戲的初值及圖形設定
        void OnKeyDown(UINT, UINT, UINT);
        void OnKeyUp(UINT, UINT, UINT);
        void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
        void OnLButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
        void OnMouseMove(UINT nFlags, CPoint point);	// 處理滑鼠的動作
        void OnRButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
        void OnRButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作

    protected:
        void OnMove();									// 移動遊戲元素
        void OnShow();									// 顯示這個狀態的遊戲畫面
    private:
        const int		NUMBALLS;	// 球的總數
        CMovingBitmap	background;	// 背景圖
        CMovingBitmap	help;		// 說明圖
        CBall*			ball;		// 球的陣列
        CMovingBitmap	corner;		// 角落圖
        CKirby			kirby;		// 拍子
        CInteger		hits_left;	// 剩下的撞擊數
        CBouncingBall   bball;		// 反覆彈跳的球

        int picX, picY;

        CMovingBitmap gamemap;
};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateOver : public CGameState
{
    public:
        CGameStateOver(CGame* g);
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnInit();
    protected:
        void OnMove();									// 移動遊戲元素
        void OnShow();									// 顯示這個狀態的遊戲畫面
    private:
        int counter;	// 倒數之計數器

};

}