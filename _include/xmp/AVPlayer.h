// ----------------------------------------------------------------------------------------------------------------------
// Build on Oct/14/2013 by Alberl
// Email:   ItsBird@qq.com  
// Website: http://www.cnblogs.com/Alberl
// 此版本为使代码结构更清晰，很多地方没做安全判断，后面的教程将会补上
// ----------------------------------------------------------------------------------------------------------------------

#ifndef __AVPlayer_H__
#define __AVPlayer_H__
#include <iostream>

typedef void (* pfnPosChanged) (void *data, int iPos);  // 控制播放进度位置变化，回调函数

struct libvlc_instance_t;
struct libvlc_media_player_t;
struct libvlc_event_t;

class CAVPlayer
{
public:
    CAVPlayer(void);
    ~CAVPlayer(void);

    void Init();                            // 初始化
    void Release();                         // 清理内存

    bool Play(const std::string &strPath);  // 播放路径为strPath的文件
    void Play ();                           // 播放
    void Stop ();                           // 停止
    void Pause();                           // 暂停

    void Volume(int nVol);                  // 音量设置为nVol
    void VolumeIncrease();                  // 音量增大
    void VolumeReduce();                    // 音量减小 

    void SeekTo(int nPos);                  // 跳到指定位置nPos
    void SeekForward();                     // 快进
    void SeekBackward();                    // 快退

    void SetHWND(HWND hwnd);                //设置播放窗口句柄
    HWND GetHWND();                         //返回播放窗口句柄

    void SetCallback(pfnPosChanged pfn); //设置回调函数
    pfnPosChanged GetCallback();         //得到回调函数

    BOOL    IsPlaying();        // 文件是否正在播放
    int     GetPos();           // 获取文件当前播放的位置
    __int64 GetTime();          // 获取时间

private:
    libvlc_instance_t       *m_pVLC_Inst;       //播放实例
    libvlc_media_player_t   *m_pVLC_Player;     //播放器 播放功能有它实现
    HWND            m_hWnd;     //播放窗口句柄
    pfnPosChanged   m_pfn;      // 文件位置改变时的回调函数
};

#endif
