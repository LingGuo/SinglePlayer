
// XMPDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "XMP.h"
#include "XMPDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_USER_POS_CHANGED WM_USER + 1


void CallbackPosChanged(void *data, int iPos)
{
    CAVPlayer *pAVPlayer = (CAVPlayer *) data;
    ::PostMessage(::GetParent(pAVPlayer->GetHWND()), WM_USER_POS_CHANGED, (WPARAM)data, iPos);
}

// 这里没做安全判断，如果要用到项目中，请下载最后一个教程的代码
CStringA UnicodeToUTF8(const CStringW& strWide)
{
    CStringA strUTF8;
    int nLen = WideCharToMultiByte(CP_UTF8, 0, strWide, -1, NULL, 0, 0, 0);

    WideCharToMultiByte(CP_UTF8, 0, strWide, -1, strUTF8.GetBuffer(nLen-1), nLen, 0, 0);

    strUTF8.ReleaseBuffer();
    return strUTF8;
}


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CXMPDlg 对话框




CXMPDlg::CXMPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXMPDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXMPDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_PLAY, m_listPlay);
    DDX_Control(pDX, IDC_SLIDER_PLAY, m_slider);
}

BEGIN_MESSAGE_MAP(CXMPDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON_PLAY, &CXMPDlg::OnBnClickedButtonPlay)
    ON_BN_CLICKED(IDC_BUTTON_PLAY_PREVIOUS, &CXMPDlg::OnBnClickedButtonPlayPrevious)
    ON_BN_CLICKED(IDC_BUTTON_PLAY_NEXT, &CXMPDlg::OnBnClickedButtonPlayNext)
    ON_BN_CLICKED(IDC_BUTTON_PLAY_FAST_BACKFORWARD, &CXMPDlg::OnBnClickedButtonPlayFastBackforward)
    ON_BN_CLICKED(IDC_BUTTON_PLAY_FAST_FORWARD, &CXMPDlg::OnBnClickedButtonPlayFastForward)
    ON_BN_CLICKED(IDC_BUTTON_VOLUME_DECREASE, &CXMPDlg::OnBnClickedButtonVolumeDecrease)
    ON_BN_CLICKED(IDC_BUTTON_VOLUME_INCREASE, &CXMPDlg::OnBnClickedButtonVolumeIncrease)
    ON_BN_CLICKED(IDC_BUTTON_SHOW_PLAY_LIST, &CXMPDlg::OnBnClickedButtonShowPlaylist)
    ON_WM_DROPFILES()
    ON_LBN_DBLCLK(IDC_LIST_PLAY, &CXMPDlg::OnLbnDblclkListPlay)
    ON_MESSAGE(WM_USER_POS_CHANGED, OnPosChanged)
END_MESSAGE_MAP()


// CXMPDlg 消息处理程序

BOOL CXMPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码  
    ModifyStyleEx(NULL, WS_EX_ACCEPTFILES);                             // 接收文件拖拽
    m_cAVPlayer.SetHWND(GetDlgItem(IDC_STATIC_VIDEO)->GetSafeHwnd());   // 设置播放器的窗口句柄
    m_cAVPlayer.SetCallback(CallbackPosChanged);

    // 隐藏播放列表，并调整窗口大小
    CRect rcWnd, rcPlaylist;
    GetWindowRect(&rcWnd);  
    m_listPlay.GetClientRect(&rcPlaylist);  
    SetWindowPos(NULL, 0, 0, rcWnd.Width() - rcPlaylist.Width() - GetSystemMetrics(SM_CXFRAME) - GetSystemMetrics(SM_CXBORDER), rcWnd.Height(), SWP_NOMOVE);
    GetDlgItem(IDC_LIST_PLAY)->ShowWindow(SW_HIDE); 

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CXMPDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CXMPDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CXMPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CXMPDlg::OnDropFiles(HDROP hDropInfo)
{
    // 这里只是做个示范，为使代码简明，并不过滤文件类型，以及遍历目录
    ShowPlaylist(TRUE);
    UINT nFileCount = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);

    for (UINT i = 0; i < nFileCount; i++)
    {
        TCHAR szFileName[_MAX_PATH] = _T("");
        ::DragQueryFile(hDropInfo, i, szFileName, _MAX_PATH);
        m_listPlay.AddString(szFileName);
        m_listPlay.SetHorizontalExtent(500); // 这里只是简单的设置为500宽度，若要刚好匹配路径宽度，请参见msdn里的例子
    }

    ::DragFinish(hDropInfo);
    CDialog::OnDropFiles(hDropInfo);
}

void CXMPDlg::ShowPlaylist( BOOL bShow )
{
    CRect rcWnd, rcPlaylist;

    GetWindowRect(&rcWnd);  
    m_listPlay.GetClientRect(&rcPlaylist);  

    if (m_listPlay.IsWindowVisible() != bShow) // 只有显示状态发生变化时，才调整窗口大小
    {
        if(bShow)
        {
            SetWindowPos(NULL, 0, 0, rcWnd.Width() + rcPlaylist.Width(), rcWnd.Height(), SWP_NOMOVE);
        }
        else
        {
            SetWindowPos(NULL, 0, 0, rcWnd.Width() - rcPlaylist.Width(), rcWnd.Height(), SWP_NOMOVE);
        }
    }

    m_listPlay.ShowWindow( bShow ? SW_SHOW : SW_HIDE);
}

void CXMPDlg::OnBnClickedButtonPlayFastBackforward()
{
    m_cAVPlayer.SeekBackward();
}

void CXMPDlg::OnBnClickedButtonPlayFastForward()
{
    m_cAVPlayer.SeekForward();
}

void CXMPDlg::OnBnClickedButtonVolumeDecrease()
{
    m_cAVPlayer.VolumeReduce();
}

void CXMPDlg::OnBnClickedButtonVolumeIncrease()
{
    m_cAVPlayer.VolumeIncrease();
}

void CXMPDlg::OnBnClickedButtonShowPlaylist()
{
    ShowPlaylist(! m_listPlay.IsWindowVisible());
}

void CXMPDlg::OnBnClickedButtonPlay()
{
    // 注意：这些地方都没有验证参数，所以如果没有文件时，点击这里会崩溃。带参数验证的代码会在最后一个教程中出现。
    if (m_cAVPlayer.IsPlaying())
    {
        m_cAVPlayer.Pause();
        GetDlgItem(IDC_BUTTON_PLAY)->SetWindowText(_T("播放"));
    } 
    else
    {
        m_cAVPlayer.Play();
        GetDlgItem(IDC_BUTTON_PLAY)->SetWindowText(_T("暂停"));
    }
}

void CXMPDlg::OnBnClickedButtonPlayPrevious()
{
    int nIndex = m_listPlay.GetCurSel();
    if (LB_ERR == nIndex)
    {
        return;
    }

    if (nIndex - 1 < 0)
    {
        nIndex = m_listPlay.GetCount();
    } 

    m_listPlay.SetCurSel(nIndex - 1);
    OnLbnDblclkListPlay();
}

void CXMPDlg::OnBnClickedButtonPlayNext()
{
    int nIndex = m_listPlay.GetCurSel();
    if (LB_ERR == nIndex)
    {
        return;
    }

    if (nIndex + 1 >= m_listPlay.GetCount())
    {
        nIndex = -1;
    } 

    m_listPlay.SetCurSel(nIndex + 1);
    OnLbnDblclkListPlay();
}

void CXMPDlg::OnLbnDblclkListPlay()
{
    int nIndex = m_listPlay.GetCurSel();
    if (LB_ERR == nIndex)
    {
        return;
    }

    CString strPath;
    CRect   rcVideo; 

    GetDlgItem(IDC_STATIC_VIDEO)->GetClientRect(&rcVideo);  
    InvalidateRect(rcVideo);
    m_listPlay.GetText(nIndex, strPath);
    m_cAVPlayer.Play((LPCSTR)UnicodeToUTF8(strPath));
    GetDlgItem(IDC_BUTTON_PLAY)->SetWindowText(_T("暂停"));
}

// 这些状态的更新虽然可以用一个定时器更新，但是没有VLC的精确，VLC基本是解析一帧数据就发一个事件，非常精确
LRESULT CXMPDlg::OnPosChanged( WPARAM wParam, LPARAM lParam )
{
    m_slider.SetPos(m_cAVPlayer.GetPos());
    return TRUE;
}
