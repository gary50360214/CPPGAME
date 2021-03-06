namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// 這個class提供可以用鍵盤或滑鼠控制的擦子
// 看懂就可以改寫成自己的程式了
/////////////////////////////////////////////////////////////////////////////

class CKirby
{
    public:
        CKirby();
        int  GetX1();					// 擦子左上角 x 座標
        int  GetY1();					// 擦子左上角 y 座標
        int  GetX2();					// 擦子右下角 x 座標
        int  GetY2();					// 擦子右下角 y 座標
        void Initialize();				// 設定擦子為初始值
        void LoadBitmap();				// 載入圖形
        void OnMove();					// 移動卡比
        void OnShow();					// 將卡比圖形貼到畫面
        void SetMovingDown(bool flag);	// 設定是否正在往下移動
        void SetMovingLeft(bool flag);	// 設定是否正在往左移動
        void SetMovingRight(bool flag); // 設定是否正在往右移動
        void SetMovingUp(bool flag);	// 設定是否正在往上移動
        void SetMovingJump(bool flag);	// 設定是否正在跳躍
        void SetMovingFly(bool flag);	// 設定是否正在飛翔
        void SetXY(int nx, int ny);		// 設定擦子左上角座標
        void Jump();					// 設定跳躍
        void Fly();						// 設定飛翔
        void GoDown();					// 設定回到地面
        bool isMovingDown;			// 是否正在往下移動
        bool isMovingLeft;			// 是否正在往左移動
        bool isMovingRight;			// 是否正在往右移動
        bool isMovingUp;			// 是否正在往上移動
        bool isMovingJump;				//是否正在跳躍
        bool isMovingFly;				//是否正在飛翔
        int x, y;
        CAnimation animation;
    protected:
        // 卡比的動畫
        // 卡比左上角座標

        int floor;				// 地板的Y座標
        bool flying;			// true表上升、false表下降
        bool rising;			// true表上升、false表下降
        int initial_velocity;	// 初始速度
        int velocity;			// 目前的速度(點/次)
        int initial_fly;	// 初始速度
        int fly;				// 飛的高度
};
}