/*
 * gamelib.h: ���ɮ��x�C��������class��interface
*/

#include <list>
#include <vector>
#include <map>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// �o��class�|�إ�DirectX Audio����A�H���ѭ���
// �C��Public Interface���Ϊk���n���AImplementation�i�H����
// �`�N�ƶ��G�����ɮץi�H��WAVE��(.wav)��MIDI��(.mid)�A�ƭӭ����ɮץi�H
//           �V�X(�P��)����A���O�䤤�u��V�X�@��MIDI�ɡA���MIDI�ɮפ�
//           ��P�ɼ���A�p�G�P�ɼ�����MIDI�ɪ��ܡA�e�@��MIDI�ɷ|�Q��
//           ���C
/////////////////////////////////////////////////////////////////////////////

namespace game_framework
{

class CAudio
{
    public:
        ~CAudio();
        void           Close();						// ����Direct Sound����
        static CAudio* Instance();					// ���oCAudio��Instance
        bool           Load(unsigned, char*);		// ���J�s��i���n���A���wMIDI�ɮ�
        bool           Open();						// �}��Direct Sound����
        void		   Pause();						// �Ȱ�����Ҧ�����
        void           Play(unsigned, bool = false);	// �}�l����s��i���n���A���w�O�_���м���
        void		   Resume();					// �_��Ȱ����񪺭���
        void           SetPowerResume();			// �q����_
        void           Stop(unsigned);				// �����s��i���n��
    private:
        class Info
        {
            public:
                Info()
                {
                    repeat = isGood = false;
                }
                bool repeat, isGood;
                string fileName;
        };
        static void MCIThread(HANDLE);	// MCI thread method
        static void ExecuteMciCommand(char*);  //
        void SendMciCommand(char*);	//
        CAudio();						// private constructor
        static CAudio		audio;		// �۰�destruct
        map<int, Info>		info;
        bool				isOpened;
        HANDLE				hThread;	// MCI command thread
        HANDLE				hWriteEnd;	// Pipe write handle for thread
        const static int	MAX_MCI_COMMAND_SIZE = 7000;
};

}