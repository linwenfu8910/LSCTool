// ShellProgressMsg.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "LSCTool.h"
#include "ShellProgressMsg.h"
#include "LSCToolDlg.h"


// CShellProgressMsg �Ի���

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

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

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

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	ShowWarnMsg();

	// Init the Progress max range
	m_proLoad.SetRange(0, m_nProgressRange);
	m_proLoad.SetPos( 0 );
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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


// CShellProgressMsg ��Ϣ�������
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
	m_proLoad.OffsetPos( 1 );//PROGRESS_INCREASE�궨������ÿ�ε�����  
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
	// True means current shell was stop, display text "�����ű�"
	// False means current shell was run, dipslay text "��ͣ�ű�"
	// See the function OnBnClickedButtonStop for detailed.
	return m_bContinueStatus;
}

void CShellProgressMsg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_bQuitStatus == FALSE)
	{
		if( IDOK == MessageBox(_T("ȷ��Ҫ��ֹ�ű���"), _T("��ֹ�ű�"), MB_OKCANCEL))
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_bQuitStatus == FALSE)
	{
		CLSCToolDlg *dlg = (CLSCToolDlg *)GetParent();
		
		if(m_bContinueStatus)
		{
			GetDlgItem(IDC_BUTTON_STOP)->SetWindowText(_T("��ͣ�ű�"));
			m_bContinueStatus = FALSE;
			dlg->ShellSendModeContinue();
		}
		else
		{
			GetDlgItem(IDC_BUTTON_STOP)->SetWindowText(_T("�����ű�"));
			m_bContinueStatus = TRUE;
			dlg->ShellSendModeStop();
		}
	}
	else
	{
		MessageBox(_T("�ű��Ѿ�ִ�н�������ȷ��"), _T("��ֹ�ű�"), MB_OK);
	}
}

void CShellProgressMsg::OnBnClickedButtonQuit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_bQuitStatus == FALSE)
	{
		if( IDOK == MessageBox(_T("ȷ��Ҫ��ֹ�ű���"), _T("��ֹ�ű�"), MB_OKCANCEL))
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	OnBnClickedButtonQuit();
	CDialog::OnClose();
}
