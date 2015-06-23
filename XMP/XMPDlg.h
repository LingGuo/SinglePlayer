
// XMPDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "../_include/xmp/AVPlayer.h"
#include "afxcmn.h"


// CXMPDlg 对话框
class CXMPDlg : public CDialog
{
// 构造
public:
	CXMPDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_XMP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
    void ShowPlaylist(BOOL bShow);  // 显示或隐藏播放列表

private:
    CAVPlayer   m_cAVPlayer;        // 播放器类


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonPlay();
    afx_msg void OnBnClickedButtonPlayPrevious();
    afx_msg void OnBnClickedButtonPlayNext();
    afx_msg void OnBnClickedButtonPlayFastBackforward();
    afx_msg void OnBnClickedButtonPlayFastForward();
    afx_msg void OnBnClickedButtonVolumeDecrease();
    afx_msg void OnBnClickedButtonVolumeIncrease();
    afx_msg void OnBnClickedButtonShowPlaylist();
    afx_msg void OnDropFiles(HDROP hDropInfo);
    afx_msg void OnLbnDblclkListPlay();
    afx_msg LRESULT OnPosChanged(WPARAM wParam, LPARAM lParam);

    CListBox m_listPlay;
    CSliderCtrl m_slider;
};
