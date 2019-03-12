namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// �o��class���ѥi�H����L�ηƹ�������l
// �����N�i�H��g���ۤv���{���F
/////////////////////////////////////////////////////////////////////////////

class CKirby
{
    public:
        CKirby();
        int  GetX1();					// ���l���W�� x �y��
        int  GetY1();					// ���l���W�� y �y��
        int  GetX2();					// ���l�k�U�� x �y��
        int  GetY2();					// ���l�k�U�� y �y��
        void Initialize();				// �]�w���l����l��
        void LoadBitmap();				// ���J�ϧ�
        void OnMove();					// ���ʥd��
        void OnShow();					// �N�d��ϧζK��e��
        void SetMovingDown(bool flag);	// �]�w�O�_���b���U����
        void SetMovingLeft(bool flag);	// �]�w�O�_���b��������
        void SetMovingRight(bool flag); // �]�w�O�_���b���k����
        void SetMovingUp(bool flag);	// �]�w�O�_���b���W����
        void SetMovingJump(bool flag);	// �]�w�O�_���b���D
        void SetMovingFly(bool flag);	// �]�w�O�_���b����
        void SetXY(int nx, int ny);		// �]�w���l���W���y��
        void Jump();					// �]�w���D
        void Fly();						// �]�w����
        void GoDown();					// �]�w�^��a��
        bool isMovingDown;			// �O�_���b���U����
        bool isMovingLeft;			// �O�_���b��������
        bool isMovingRight;			// �O�_���b���k����
        bool isMovingUp;			// �O�_���b���W����
        bool isMovingJump;				//�O�_���b���D
        bool isMovingFly;				//�O�_���b����
        int x, y;
        CAnimation animation;
    protected:
        // �d�񪺰ʵe
        // �d�񥪤W���y��

        int floor;				// �a�O��Y�y��
        bool flying;			// true��W�ɡBfalse��U��
        bool rising;			// true��W�ɡBfalse��U��
        int initial_velocity;	// ��l�t��
        int velocity;			// �ثe���t��(�I/��)
        int initial_fly;	// ��l�t��
        int fly;				// ��������
};
}