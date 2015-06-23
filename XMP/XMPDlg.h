
// XMPDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "../_include/xmp/AVPlayer.h"
#include "afxcmn.h"


// CXMPDlg �Ի���
class CXMPDlg : public CDialog
{
// ����
public:
	CXMPDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_XMP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
    void ShowPlaylist(BOOL bShow);  // ��ʾ�����ز����б�

private:
    CAVPlayer   m_cAVPlayer;        // ��������


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
