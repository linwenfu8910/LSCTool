// ShellProgressMsg.cpp : 实现文件
//
#include "stdafx.h"
#include "LSCTool.h"
#include "ShellProgressMsg.h"
#include "LSCToolDlg.h"


// CShellProgressMsg 对话框

IMPLEMENT_DYNAMIC(CShellProgressMsg, CDialog)

CShellProgressMsg::CShellProgressMsg(CWnd* pParent /*=NULL*/)
	: CDialog(CShellProgressMsg::IDD, pParent)
	, m_strWarnMsg(_T(""))
	, m_nProgressRange(0)
	, m_bQuitStatus(false)
	, m_bStopQuitStatus(false)
	, m_bContinueStatus(false)
{

}

CShellProgressMsg::~CShellProgressMsg()
{
	
}

BOOL CShellProgressMsg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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

	// initial the button
	if(m_bStopQuitStatus)
	{
		m_bContinueStatus = FALSE;
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		GetDlgItem(IDCANCEL)->ShowWindow(FALSE);
		GetDlgItem(IDC_PROGRESS_LOAD)->EnableWindow(FALSE);
		GetDlgItem(IDC_PROGRESS_LOAD)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_WARNMSG)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_WARNMSG)->ShowWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QUIT)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_WARNMSG2)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_WARNMSG2)->ShowWindow(FALSE);
	}

	// TODO: 在此添加额外的初始化代码
	ShowWarnMsg();

	// Init the Progress max range
	m_proLoad.SetRange(0, m_nProgressRange);
	m_proLoad.SetPos( 0 );
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CShellProgressMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_LOAD, m_proLoad);
}


BEGIN_MESSAGE_MAP(CShellProgressMsg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CShellProgressMsg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CShellProgressMsg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CShellProgressMsg::OnBnClickedButtonQuit)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CShellProgressMsg 消息处理程序
void CShellProgressMsg::SetWarnMsg(CString strMsg)
{
	m_strWarnMsg = strMsg;
}

CString CShellProgressMsg::GetWarnMsg(void)
{
	return m_strWarnMsg;
}

void CShellProgressMsg::ShowWarnMsg(void)
{
	if(m_bStopQuitStatus)
	{
		SetDlgItemText(IDC_STATIC_WARNMSG2, m_strWarnMsg);
	}
	else
	{
		SetDlgItemText(IDC_STATIC_WARNMSG, m_strWarnMsg);
	}
}

void CShellProgressMsg::SetProgressRange(int nRange)
{
	m_nProgressRange = nRange;
}

int CShellProgressMsg::UpdateProgress(void)
{
	int nPos, nMaxRange;
	m_proLoad.OffsetPos( 1 );//PROGRESS_INCREASE宏定义设置每次递增量  
    m_proLoad.GetRange( nPos, nMaxRange );  
    nPos = m_proLoad.GetPos();  
    if( nPos >= nMaxRange ) m_proLoad.SetPos( m_nProgressRange ); 
	return 0;
}

void CShellProgressMsg::UpdateProgressFull(void)
{
	m_proLoad.SetPos( m_nProgressRange );
}

void CShellProgressMsg::UpdateProgress(int nCurrentPos)
{
	int nPos, nMaxRange;
    m_proLoad.GetRange( nPos, nMaxRange );   
    if( nCurrentPos >= nMaxRange ) m_proLoad.SetPos( m_nProgressRange ); 
	else if(nCurrentPos <= nPos) m_proLoad.SetPos( m_nProgressRange ); 
	else m_proLoad.SetPos( nCurrentPos );
}

void CShellProgressMsg::SetQuitStatus(bool bStatus)
{
	m_bQuitStatus =  bStatus;
}

bool CShellProgressMsg::GetQuitStatus(void)
{
	return m_bQuitStatus;
}

void CShellProgressMsg::SetStopQuitStatus(bool bStatus)
{
	m_bStopQuitStatus = bStatus;
}

bool CShellProgressMsg::GetContinueStatus(void)
{
	// True means current shell was stop, display text "继续脚本"
	// False means current shell was run, dipslay text "暂停脚本"
	// See the function OnBnClickedButtonStop for detailed.
	return m_bContinueStatus;
}

void CShellProgressMsg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_bQuitStatus == FALSE)
	{
		if( IDOK == MessageBox(_T("确定要终止脚本？"), _T("终止脚本"), MB_OKCANCEL))
		{
			if(m_bQuitStatus == FALSE)
			{
				CLSCToolDlg *dlg = (CLSCToolDlg *)GetParent();
				dlg->ShellStop();
				OnCancel();
				m_bStopQuitStatus = FALSE;
			}
			else
			{
				OnCancel();
				m_bStopQuitStatus = FALSE;
			}
		}
	}
	else
	{
		OnCancel();
		m_bStopQuitStatus = FALSE;
	}
}


void CShellProgressMsg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_bQuitStatus == FALSE)
	{
		CLSCToolDlg *dlg = (CLSCToolDlg *)GetParent();
		
		if(m_bContinueStatus)
		{
			GetDlgItem(IDC_BUTTON_STOP)->SetWindowText(_T("暂停脚本"));
			m_bContinueStatus = FALSE;
			dlg->ShellSendModeContinue();
		}
		else
		{
			GetDlgItem(IDC_BUTTON_STOP)->SetWindowText(_T("继续脚本"));
			m_bContinueStatus = TRUE;
			dlg->ShellSendModeStop();
		}
	}
	else
	{
		MessageBox(_T("脚本已经执行结束，请确认"), _T("终止脚本"), MB_OK);
	}
}

void CShellProgressMsg::OnBnClickedButtonQuit()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_bQuitStatus == FALSE)
	{
		if( IDOK == MessageBox(_T("确定要终止脚本？"), _T("终止脚本"), MB_OKCANCEL))
		{
			if(m_bQuitStatus == FALSE)
			{
				CLSCToolDlg *dlg = (CLSCToolDlg *)GetParent();
				dlg->ShellStop();
				OnCancel();
			}
			else
			{
				OnCancel();
			}
		}
	}
	else
	{
		OnCancel();
	}
}

void CShellProgressMsg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	OnBnClickedButtonQuit();
	CDialog::OnClose();
}
