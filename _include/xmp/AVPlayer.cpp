#include "Stdafx.h"
#include "AVPlayer.h"
#include <cmath>

#include "../vlc/vlc.h"
#pragma comment(lib, "../_lib/vlc/libvlc.lib")
#pragma comment(lib, "../_lib/vlc/libvlccore.lib")


// VLC的事件管理 实现事件的具体处理函数
void OnVLC_EndReached(const libvlc_event_t *event, void *data);
void OnVLC_PositionChanged(const libvlc_event_t *event, void *data);

//构造函数
CAVPlayer::CAVPlayer(void) :
m_pVLC_Inst(NULL),
m_pVLC_Player(NULL),
m_hWnd(NULL),
m_pfn(NULL)
{
}
//析构函数
CAVPlayer::~CAVPlayer(void)
{
    Release();
}
//初始播放实例  
void CAVPlayer::Init()
{
    if (! m_pVLC_Inst)
    {
        //可以给实例传配置参数
        m_pVLC_Inst = libvlc_new(0, NULL);
    }
}
//停止播放并释放播放实例
void CAVPlayer::Release()
{
    Stop();

    if (m_pVLC_Inst)
    {
        libvlc_release (m_pVLC_Inst);
        m_pVLC_Inst   = NULL;
    }
}
//根据路径播放文件
bool CAVPlayer::Play(const std::string &strPath)
{
    //播放实例不能为空
    if (! m_pVLC_Inst)
    {
        Init();
    }
    //路径与播放实例不能为空
    if(strPath.empty() || ! m_pVLC_Inst)
    {
        return false;
    }
    //先停止以前的播放
    Stop();
    //开始新的播放
    libvlc_media_t *m;//播放源
    m = libvlc_media_new_path(m_pVLC_Inst, strPath.c_str());    /* Create a new item */ 
    //为播放源创建一个播放器
    m_pVLC_Player = libvlc_media_player_new_from_media(m);      /* Create a media player playing environement */     
    libvlc_media_release(m);                                    /* No need to keep the media now */
    //设置播放器的播放窗口
    libvlc_media_player_set_hwnd(m_pVLC_Player, m_hWnd);
    //播放
    libvlc_media_player_play(m_pVLC_Player);                    /* play the media_player */    
 
    // 事件管理
    libvlc_event_manager_t *vlc_evt_man = libvlc_media_player_event_manager(m_pVLC_Player);
    //附加事件 绑定事件信息与处理函数
    libvlc_event_attach(vlc_evt_man, libvlc_MediaPlayerEndReached, ::OnVLC_EndReached, this);
    libvlc_event_attach(vlc_evt_man, libvlc_MediaPlayerPositionChanged, ::OnVLC_PositionChanged, this);

    return true;
}
//播放停止
void CAVPlayer::Stop()
{
    if (m_pVLC_Player)
    {
        libvlc_media_player_stop (m_pVLC_Player);      /* Stop playing */    
        //释放播发器
        libvlc_media_player_release (m_pVLC_Player);   /* Free the media_player */
        m_pVLC_Player = NULL;
    }

    /* Stop the media */
    //if (libvlc_media_player_get_state(vlcPlayer) == libvlc_Ended)
}
//播放
void CAVPlayer::Play()
{
    libvlc_media_player_play(m_pVLC_Player);
}
//暂停
void CAVPlayer::Pause()
{
    libvlc_media_player_pause(m_pVLC_Player);
}
//设置音量
void CAVPlayer::Volume(int nVol)
{
    libvlc_audio_set_volume(m_pVLC_Player,nVol);
}
//音量增加10%
void CAVPlayer::VolumeIncrease()
{
    int nVol = libvlc_audio_get_volume(m_pVLC_Player);
    Volume(ceil(nVol * 1.1));
}
//音量减少10%
void CAVPlayer::VolumeReduce()
{
    int nVol = libvlc_audio_get_volume(m_pVLC_Player);
    Volume(floor(nVol * 0.9));
}
//得到播放器播放进度
int CAVPlayer::GetPos()
{
    return 100.0 * libvlc_media_player_get_position(m_pVLC_Player);
}
//设置播发器播放进度
void CAVPlayer::SeekTo(int nPos)
{
    libvlc_media_player_set_position(m_pVLC_Player, (float)nPos/100.0);
}
//设置播发器播放进度向前进总进度的10%
void CAVPlayer::SeekForward()
{
    int nPos = GetPos();
    //SeekTo(ceil(nPos * 1.1));
    SeekTo(nPos + 10);
}
//设置播发器播放进度向后退总进度的10%
void CAVPlayer::SeekBackward()
{
    int nPos = GetPos();
    //SeekTo(floor(nPos * 0.9));
    SeekTo(nPos - 10);
}
//设置播放显示窗口
void CAVPlayer::SetHWND( HWND hwnd )
{
    m_hWnd = hwnd;
}
//得到播放显示窗口
HWND CAVPlayer::GetHWND()
{
    return m_hWnd;
}
//设置回调函数
void CAVPlayer::SetCallback( pfnPosChanged pfn )
{
    m_pfn = pfn;
}
//获得回调函数
pfnPosChanged CAVPlayer::GetCallback()
{
    return m_pfn;
}
//判断是否正在播放
BOOL CAVPlayer::IsPlaying()
{
    return libvlc_media_player_is_playing(m_pVLC_Player);
}
//当前播放时间
__int64 CAVPlayer::GetTime()
{
    return libvlc_media_player_get_time(m_pVLC_Player);
}

//播放完成时处理
void OnVLC_EndReached( const libvlc_event_t *event, void *data )
{

}
//播放进度改变使处理
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
