// ----------------------------------------------------------------------------------------------------------------------
// Build on Oct/14/2013 by Alberl
// Email:   ItsBird@qq.com  
// Website: http://www.cnblogs.com/Alberl
// �˰汾Ϊʹ����ṹ���������ܶ�ط�û����ȫ�жϣ�����Ľ̳̽��Ჹ��
// ----------------------------------------------------------------------------------------------------------------------

#ifndef __AVPlayer_H__
#define __AVPlayer_H__
#include <iostream>

typedef void (* pfnPosChanged) (void *data, int iPos);  // ���Ʋ��Ž���λ�ñ仯���ص�����

struct libvlc_instance_t;
struct libvlc_media_player_t;
struct libvlc_event_t;

class CAVPlayer
{
public:
    CAVPlayer(void);
    ~CAVPlayer(void);

    void Init();                            // ��ʼ��
    void Release();                         // �����ڴ�

    bool Play(const std::string &strPath);  // ����·��ΪstrPath���ļ�
    void Play ();                           // ����
    void Stop ();                           // ֹͣ
    void Pause();                           // ��ͣ

    void Volume(int nVol);                  // ��������ΪnVol
    void VolumeIncrease();                  // ��������
    void VolumeReduce();                    // ������С 

    void SeekTo(int nPos);                  // ����ָ��λ��nPos
    void SeekForward();                     // ���
    void SeekBackward();                    // ����

    void SetHWND(HWND hwnd);                //���ò��Ŵ��ھ��
    HWND GetHWND();                         //���ز��Ŵ��ھ��

    void SetCallback(pfnPosChanged pfn); //���ûص�����
    pfnPosChanged GetCallback();         //�õ��ص�����

    BOOL    IsPlaying();        // �ļ��Ƿ����ڲ���
    int     GetPos();           // ��ȡ�ļ���ǰ���ŵ�λ��
    __int64 GetTime();          // ��ȡʱ��

private:
    libvlc_instance_t       *m_pVLC_Inst;       //����ʵ��
    libvlc_media_player_t   *m_pVLC_Player;     //������ ���Ź�������ʵ��
    HWND            m_hWnd;     //���Ŵ��ھ��
    pfnPosChanged   m_pfn;      // �ļ�λ�øı�ʱ�Ļص�����
};

#endif
