
// XMPDlg.cpp : ʵ���ļ�
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

// ����û����ȫ�жϣ����Ҫ�õ���Ŀ�У����������һ���̵̳Ĵ���
CStringA UnicodeToUTF8(const CStringW& strWide)
{
    CStringA strUTF8;
    int nLen = WideCharToMultiByte(CP_UTF8, 0, strWide, -1, NULL, 0, 0, 0);

    WideCharToMultiByte(CP_UTF8, 0, strWide, -1, strUTF8.GetBuffer(nLen-1), nLen, 0, 0);

    strUTF8.ReleaseBuffer();
    return strUTF8;
}


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CXMPDlg �Ի���




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


// CXMPDlg ��Ϣ�������

BOOL CXMPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������  
    ModifyStyleEx(NULL, WS_EX_ACCEPTFILES);                             // �����ļ���ק
    m_cAVPlayer.SetHWND(GetDlgItem(IDC_STATIC_VIDEO)->GetSafeHwnd());   // ���ò������Ĵ��ھ��
    m_cAVPlayer.SetCallback(CallbackPosChanged);

    // ���ز����б����������ڴ�С
    CRect rcWnd, rcPlaylist;
    GetWindowRect(&rcWnd);  
    m_listPlay.GetClientRect(&rcPlaylist);  
    SetWindowPos(NULL, 0, 0, rcWnd.Width() - rcPlaylist.Width() - GetSystemMetrics(SM_CXFRAME) - GetSystemMetrics(SM_CXBORDER), rcWnd.Height(), SWP_NOMOVE);
    GetDlgItem(IDC_LIST_PLAY)->ShowWindow(SW_HIDE); 

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CXMPDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CXMPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CXMPDlg::OnDropFiles(HDROP hDropInfo)
{
    // ����ֻ������ʾ����Ϊʹ������������������ļ����ͣ��Լ�����Ŀ¼
    ShowPlaylist(TRUE);
    UINT nFileCount = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);

    for (UINT i = 0; i < nFileCount; i++)
    {
        TCHAR szFileName[_MAX_PATH] = _T("");
        ::DragQueryFile(hDropInfo, i, szFileName, _MAX_PATH);
        m_listPlay.AddString(szFileName);
        m_listPlay.SetHorizontalExtent(500); // ����ֻ�Ǽ򵥵�����Ϊ500��ȣ���Ҫ�պ�ƥ��·����ȣ���μ�msdn�������
    }

    ::DragFinish(hDropInfo);
    CDialog::OnDropFiles(hDropInfo);
}

void CXMPDlg::ShowPlaylist( BOOL bShow )
{
    CRect rcWnd, rcPlaylist;

    GetWindowRect(&rcWnd);  
    m_listPlay.GetClientRect(&rcPlaylist);  

    if (m_listPlay.IsWindowVisible() != bShow) // ֻ����ʾ״̬�����仯ʱ���ŵ������ڴ�С
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
    // ע�⣺��Щ�ط���û����֤�������������û���ļ�ʱ�����������������������֤�Ĵ���������һ���̳��г��֡�
    if (m_cAVPlayer.IsPlaying())
    {
        m_cAVPlayer.Pause();
        GetDlgItem(IDC_BUTTON_PLAY)->SetWindowText(_T("����"));
    } 
    else
    {
        m_cAVPlayer.Play();
        GetDlgItem(IDC_BUTTON_PLAY)->SetWindowText(_T("��ͣ"));
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
    GetDlgItem(IDC_BUTTON_PLAY)->SetWindowText(_T("��ͣ"));
}

// ��Щ״̬�ĸ�����Ȼ������һ����ʱ�����£�����û��VLC�ľ�ȷ��VLC�����ǽ���һ֡���ݾͷ�һ���¼����ǳ���ȷ
LRESULT CXMPDlg::OnPosChanged( WPARAM wParam, LPARAM lParam )
{
    m_slider.SetPos(m_cAVPlayer.GetPos());
    return TRUE;
}
