/*
 * mygame.h: ���ɮ��x�C��������class��interface
*/

#include "CKirby.h"
#include "CBall.h"
#include "CBouncingBall.h"

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////

enum AUDIO_ID  				// �w�q�U�ح��Ī��s��
{
    AUDIO_DING,				// 0
    AUDIO_LAKE,				// 1
    AUDIO_NTUT,				// 2
    AUDIO_KIBY				// 3
};

/////////////////////////////////////////////////////////////////////////////
// �o��class���Ѧa��
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
// �o��class���C�����C���}�Y�e������
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////

class CGameStateInit : public CGameState
{
    public:
        CGameStateInit(CGame* g);
        void OnInit();  								// �C������Ȥιϧγ]�w
        void OnBeginState();							// �]�w�C�������һݪ��ܼ�
        void OnKeyUp(UINT, UINT, UINT); 				// �B�z��LUp���ʧ@
        void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
    protected:
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
    private:
        CMovingBitmap logo;								// csie��logo
};

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////

class CGameStateRun : public CGameState
{
    public:
        CGameStateRun(CGame* g);
        ~CGameStateRun();
        void OnBeginState();							// �]�w�C�������һݪ��ܼ�
        void OnInit();  								// �C������Ȥιϧγ]�w
        void OnKeyDown(UINT, UINT, UINT);
        void OnKeyUp(UINT, UINT, UINT);
        void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
        void OnLButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
        void OnMouseMove(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
        void OnRButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
        void OnRButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@

    protected:
        void OnMove();									// ���ʹC������
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
    private:
        const int		NUMBALLS;	// �y���`��
        CMovingBitmap	background;	// �I����
        CMovingBitmap	help;		// ������
        CBall*			ball;		// �y���}�C
        CMovingBitmap	corner;		// ������
        CKirby			kirby;		// ��l
        CInteger		hits_left;	// �ѤU��������
        CBouncingBall   bball;		// ���мu�����y

        int picX, picY;

        CMovingBitmap gamemap;
};

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����������A(Game Over)
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////

class CGameStateOver : public CGameState
{
    public:
        CGameStateOver(CGame* g);
        void OnBeginState();							// �]�w�C�������һݪ��ܼ�
        void OnInit();
    protected:
        void OnMove();									// ���ʹC������
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
    private:
        int counter;	// �˼Ƥ��p�ƾ�

};

}