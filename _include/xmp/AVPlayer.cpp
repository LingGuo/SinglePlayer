#include "Stdafx.h"
#include "AVPlayer.h"
#include <cmath>

#include "../vlc/vlc.h"
#pragma comment(lib, "../_lib/vlc/libvlc.lib")
#pragma comment(lib, "../_lib/vlc/libvlccore.lib")


// VLC���¼����� ʵ���¼��ľ��崦����
void OnVLC_EndReached(const libvlc_event_t *event, void *data);
void OnVLC_PositionChanged(const libvlc_event_t *event, void *data);

//���캯��
CAVPlayer::CAVPlayer(void) :
m_pVLC_Inst(NULL),
m_pVLC_Player(NULL),
m_hWnd(NULL),
m_pfn(NULL)
{
}
//��������
CAVPlayer::~CAVPlayer(void)
{
    Release();
}
//��ʼ����ʵ��  
void CAVPlayer::Init()
{
    if (! m_pVLC_Inst)
    {
        //���Ը�ʵ�������ò���
        m_pVLC_Inst = libvlc_new(0, NULL);
    }
}
//ֹͣ���Ų��ͷŲ���ʵ��
void CAVPlayer::Release()
{
    Stop();

    if (m_pVLC_Inst)
    {
        libvlc_release (m_pVLC_Inst);
        m_pVLC_Inst   = NULL;
    }
}
//����·�������ļ�
bool CAVPlayer::Play(const std::string &strPath)
{
    //����ʵ������Ϊ��
    if (! m_pVLC_Inst)
    {
        Init();
    }
    //·���벥��ʵ������Ϊ��
    if(strPath.empty() || ! m_pVLC_Inst)
    {
        return false;
    }
    //��ֹͣ��ǰ�Ĳ���
    Stop();
    //��ʼ�µĲ���
    libvlc_media_t *m;//����Դ
    m = libvlc_media_new_path(m_pVLC_Inst, strPath.c_str());    /* Create a new item */ 
    //Ϊ����Դ����һ��������
    m_pVLC_Player = libvlc_media_player_new_from_media(m);      /* Create a media player playing environement */     
    libvlc_media_release(m);                                    /* No need to keep the media now */
    //���ò������Ĳ��Ŵ���
    libvlc_media_player_set_hwnd(m_pVLC_Player, m_hWnd);
    //����
    libvlc_media_player_play(m_pVLC_Player);                    /* play the media_player */    
 
    // �¼�����
    libvlc_event_manager_t *vlc_evt_man = libvlc_media_player_event_manager(m_pVLC_Player);
    //�����¼� ���¼���Ϣ�봦����
    libvlc_event_attach(vlc_evt_man, libvlc_MediaPlayerEndReached, ::OnVLC_EndReached, this);
    libvlc_event_attach(vlc_evt_man, libvlc_MediaPlayerPositionChanged, ::OnVLC_PositionChanged, this);

    return true;
}
//����ֹͣ
void CAVPlayer::Stop()
{
    if (m_pVLC_Player)
    {
        libvlc_media_player_stop (m_pVLC_Player);      /* Stop playing */    
        //�ͷŲ�����
        libvlc_media_player_release (m_pVLC_Player);   /* Free the media_player */
        m_pVLC_Player = NULL;
    }

    /* Stop the media */
    //if (libvlc_media_player_get_state(vlcPlayer) == libvlc_Ended)
}
//����
void CAVPlayer::Play()
{
    libvlc_media_player_play(m_pVLC_Player);
}
//��ͣ
void CAVPlayer::Pause()
{
    libvlc_media_player_pause(m_pVLC_Player);
}
//��������
void CAVPlayer::Volume(int nVol)
{
    libvlc_audio_set_volume(m_pVLC_Player,nVol);
}
//��������10%
void CAVPlayer::VolumeIncrease()
{
    int nVol = libvlc_audio_get_volume(m_pVLC_Player);
    Volume(ceil(nVol * 1.1));
}
//��������10%
void CAVPlayer::VolumeReduce()
{
    int nVol = libvlc_audio_get_volume(m_pVLC_Player);
    Volume(floor(nVol * 0.9));
}
//�õ����������Ž���
int CAVPlayer::GetPos()
{
    return 100.0 * libvlc_media_player_get_position(m_pVLC_Player);
}
//���ò��������Ž���
void CAVPlayer::SeekTo(int nPos)
{
    libvlc_media_player_set_position(m_pVLC_Player, (float)nPos/100.0);
}
//���ò��������Ž�����ǰ���ܽ��ȵ�10%
void CAVPlayer::SeekForward()
{
    int nPos = GetPos();
    //SeekTo(ceil(nPos * 1.1));
    SeekTo(nPos + 10);
}
//���ò��������Ž���������ܽ��ȵ�10%
void CAVPlayer::SeekBackward()
{
    int nPos = GetPos();
    //SeekTo(floor(nPos * 0.9));
    SeekTo(nPos - 10);
}
//���ò�����ʾ����
void CAVPlayer::SetHWND( HWND hwnd )
{
    m_hWnd = hwnd;
}
//�õ�������ʾ����
HWND CAVPlayer::GetHWND()
{
    return m_hWnd;
}
//���ûص�����
void CAVPlayer::SetCallback( pfnPosChanged pfn )
{
    m_pfn = pfn;
}
//��ûص�����
pfnPosChanged CAVPlayer::GetCallback()
{
    return m_pfn;
}
//�ж��Ƿ����ڲ���
BOOL CAVPlayer::IsPlaying()
{
    return libvlc_media_player_is_playing(m_pVLC_Player);
}
//��ǰ����ʱ��
__int64 CAVPlayer::GetTime()
{
    return libvlc_media_player_get_time(m_pVLC_Player);
}

//�������ʱ����
void OnVLC_EndReached( const libvlc_event_t *event, void *data )
{

}
//���Ž��ȸı�ʹ����
void OnVLC_PositionChanged( const libvlc_event_t *event, void *data )
{
    switch (event->type)
    {
    case libvlc_MediaPlayerPositionChanged:
        {
            float fPos = event->u.media_player_position_changed.new_position;
            TRACE(_T("%f\n"), fPos);

            CAVPlayer *pAVPlayer = (CAVPlayer *) data;
            (pAVPlayer->GetCallback())(pAVPlayer, fPos * 100);
        }

        break;
    case libvlc_MediaPlayerSnapshotTaken:
        break;
    default:
        break;
    }
}
