// LSCToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LSCTool.h"
#include "LSCToolDlg.h"
#include "SerialComSetup.h"
#include "ShellProgressMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonGuide();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CAboutDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_GUIDE, &CAboutDlg::OnBnClickedButtonGuide)
END_MESSAGE_MAP()

void CAboutDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	WCHAR pFileName[MAX_PATH]; 
	int nPos = GetCurrentDirectory( MAX_PATH, pFileName); 
	CString strProgramFilePath(pFileName);
	strProgramFilePath += _T("\\串口语法教程.txt");
	ShellExecute(this->m_hWnd, _T("open"),_T("notepad.exe"), strProgramFilePath, _T(""), SW_SHOW );
}

void CAboutDlg::OnBnClickedButtonGuide()
{
	// TODO: 在此添加控件通知处理程序代码
	WCHAR pFileName[MAX_PATH]; 
	int nPos = GetCurrentDirectory( MAX_PATH, pFileName); 
	CString strProgramFilePath(pFileName);
	CString strValidFilePath = _T("");
	strProgramFilePath += _T("\\LSC工具使用说明书.xlsx");
	strValidFilePath = _T("\"") + strProgramFilePath + _T("\"");
	ShellExecute(this->m_hWnd, _T("open"),_T("EXCEL.exe"), strValidFilePath, _T(""), SW_SHOW );
}


// CLSCToolDlg 对话框


CShellProgressMsg m_ReadMsgDialog;

CLSCToolDlg::CLSCToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLSCToolDlg::IDD, pParent)
	, m_nComNumber(0)
	, m_strSendString(_T(""))
	, m_strCOMLog(_T(""))
	, m_nCmdMode(0)
	, m_nSendLogCount(1)
	, m_strInitFilePath(_T(""))
	, m_strSendLastString(_T(""))
	, m_nRepeatTime(0)
	, m_bShowSystemTime(false)
	, m_bShowSendString(false)
	, m_bComSyncStatus(false)
	, m_nShellMode(0)
	, m_nShellMaxLine(0)
	, m_strShellPathName(_T(""))
	, m_nShellSendTime(0)
	, m_nShellSendMode(0)
	, m_bShellSendStart(false)
	, m_strSaveLogPath(_T(""))
	, m_bShellSendModeLog(false)
	, m_strCurrentReciveString(_T(""))
	, m_strCOMReciveString(_T(""))
	, m_bShellSendWaitString(false)
	, m_bShellSendAlready(false)
	, m_bShowReceiveString(true)
	, m_bEditComLogDisplayComplete(true)
	, m_bShowBottom(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//memset(m_nShellSendCount, 0x00, sizeof(m_nShellSendCount));
	m_eDamageState = edmINIT; 
}

void CLSCToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCOMM1, m_MSComm);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_strSendString);
	DDX_Text(pDX, IDC_EDIT_COMLOG, m_strCOMLog);
	DDX_Text(pDX, IDC_EDIT_REPEATTIME, m_nRepeatTime);
	DDX_Control(pDX, IDC_COMBO_INPUTLOG, m_strComboInputLog);
	DDX_Radio(pDX, IDC_RADIO_CMD_SANYO, m_nCmdMode);
}

BEGIN_MESSAGE_MAP(CLSCToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_COMSETUP, &CLSCToolDlg::OnBnClickedButtonComsetup)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CLSCToolDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, &CLSCToolDlg::OnBnClickedButtonDisconnect)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CLSCToolDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_CLEARSCREEN, &CLSCToolDlg::OnBnClickedButtonClearscreen)
	ON_BN_CLICKED(IDC_BUTTON_CLEARSENDLOG, &CLSCToolDlg::OnBnClickedButtonClearsendlog)
	ON_BN_CLICKED(IDC_BUTTON_SENDLOG, &CLSCToolDlg::OnBnClickedButtonSendlog)
	ON_BN_CLICKED(IDOK, &CLSCToolDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_INPUTSHELL, &CLSCToolDlg::OnBnClickedButtonInputshell)
	ON_CBN_SELCHANGE(IDC_COMBO_INPUTLOG, &CLSCToolDlg::OnCbnSelchangeComboInputlog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_SENDREPEAT, &CLSCToolDlg::OnBnClickedCheckSendrepeat)
	ON_BN_CLICKED(IDC_CHECK_SHOWTIME, &CLSCToolDlg::OnBnClickedCheckShowtime)
	ON_BN_CLICKED(IDC_CHECK_SHOWSENDLOG, &CLSCToolDlg::OnBnClickedCheckShowsendlog)
	ON_BN_CLICKED(IDC_RADIO_CMD_SANYO, &CLSCToolDlg::OnBnClickedRadioCmdSanyo)
	ON_BN_CLICKED(IDC_RADIO_CMD_MIS, &CLSCToolDlg::OnBnClickedRadioCmdMis)
	ON_BN_CLICKED(IDC_RADIO_CMD_BIN, &CLSCToolDlg::OnBnClickedRadioCmdBin)
	ON_BN_CLICKED(IDC_RADIO_CMD_TEXT, &CLSCToolDlg::OnBnClickedRadioCmdText)
ON_BN_CLICKED(IDC_BUTTON_QUIT, &CLSCToolDlg::OnBnClickedButtonQuit)
ON_BN_CLICKED(IDC_BUTTON_OPENLOG, &CLSCToolDlg::OnBnClickedButtonOpenlog)
ON_EN_VSCROLL(IDC_EDIT_COMLOG, &CLSCToolDlg::OnEnVscrollEditComlog)
END_MESSAGE_MAP()


// CLSCToolDlg 消息处理程序

BOOL CLSCToolDlg::OnInitDialog()
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
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	// Init the Diaglog position to center
	CRect sRectDesk,sRectDiaglog;
	sRectDesk.left = 0;
	sRectDesk.right = GetSystemMetrics(SM_CXSCREEN);
	sRectDesk.bottom = GetSystemMetrics(SM_CYSCREEN);
	sRectDesk.top = 0;
	GetWindowRect(&sRectDiaglog);
	SetWindowPos(NULL,
				sRectDesk.Width()/2 - sRectDiaglog.Width()/2, 
				sRectDesk.Height()/2 - sRectDiaglog.Height()/2,
				0,
				0,
				SWP_NOOWNERZORDER|SWP_NOSIZE|SWP_NOZORDER); 

	// Default the Button Property
	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(FALSE);

	// Need to check Curren program's INI File
	CheckProgramInitFile();
	
	// Show the Com Info static text
	ShowCurrentComInfo();

	if (m_MSComm.get_PortOpen())  
    {  
        m_MSComm.put_PortOpen(FALSE);  
    }
	bool bConnect = ComConnect();
	if(bConnect)
	{
		GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(TRUE);
	} 
	else  
	{  
		GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(FALSE);
		CString strMessage = _T("");
		strMessage.AppendFormat(_T("COM%d无法设置,请确认！"), m_nComNumber);
		MessageBox(strMessage); 
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CLSCToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLSCToolDlg::OnPaint()
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
HCURSOR CLSCToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BEGIN_EVENTSINK_MAP(CLSCToolDlg, CDialog)
	ON_EVENT(CLSCToolDlg, IDC_MSCOMM1, 1, CLSCToolDlg::OnCommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()

void CLSCToolDlg::OnCommMscomm1()
{
	// TODO: 在此处添加消息处理程序代码
	VARIANT variant_inp;  
    COleSafeArray safearray_inp;  
    DWORD nLen; 
	LONG nIndex = 0;
	int nCount = 0;
	wchar_t cRxData[20*SIZE_COM_BUFFER] = {0};// temp buffer 10Kbyte
	static wchar_t cRxEndLine[3] = {0};
	static int nRxEndLineCount = 0;
	CString strMessage = _T("");
	int nSystemTimeLenth = 0;
	int nRecieveStringLenth = 0;
	bool bEndLine = false;
      
    switch (m_MSComm.get_CommEvent())  
    {  
		case 1:    //发送数据  
			break;  
  
		case 2:    //接收数据  
			variant_inp = m_MSComm.get_Input(); //read data  
			safearray_inp = variant_inp;  
			nLen = safearray_inp.GetOneDimSize();//得到有效数据长度  
			
			// Current String get the end line
			if(m_strCOMLog.GetLength() > 1 && m_strCOMLog[m_strCOMLog.GetLength() -1 ] == '\n')
			{
				// Check to show Send String
				if(m_bShowSendString)
				{
					nRecieveStringLenth = GetStringToBuffer(LOG_RECIEVE_STRING, &cRxData[nCount]);
					nCount = nCount + nRecieveStringLenth;
				}
				// Check to show System Time
				if(m_bShowSystemTime)
				{
					nSystemTimeLenth = GetSystemTime(&cRxData[nCount]);
					nCount += nSystemTimeLenth;
				}
			}
	          
			for (nIndex = 0; nIndex < (LONG)nLen; nIndex++)  
			{  
				// Check to show System Time
				if(bEndLine)
				{
					bEndLine = false;
					if(m_bShowSendString)
					{
						nRecieveStringLenth = GetStringToBuffer(LOG_RECIEVE_STRING, &cRxData[nCount]);
						nCount = nCount + nRecieveStringLenth; // Delete "+1", 20141013, bug fixed
					}
					if(m_bShowSystemTime)
					{
						nSystemTimeLenth = GetSystemTime(&cRxData[nCount]);
						nCount = nCount + nSystemTimeLenth; // Delete "+1", 20141013, bug fixed
					}
				}

				// Get Data from Serial COM
				if(ecmdmPANABIN == m_nCmdMode )
				{
					// Convert to the BIN mode display
					WCHAR wTempData = 0;
					safearray_inp.GetElement(&nIndex, &wTempData);
					if(wTempData != '\r' && wTempData != '\n')
					{
						CString strHexString = _T("");
						strHexString.AppendFormat(_T("%02X"), wTempData);
						cRxData[nCount] = strHexString[0];
						nCount++;
						cRxData[nCount] = strHexString[1];
						nCount++;
						cRxData[nCount] = ' ';
						if(wTempData == 0x03)// Use for End line char 
						{
							nCount++;
							cRxData[nCount] = '\r';
						}
					}
					else
					{
						cRxData[nCount] = wTempData;
					}
				}
				else if(ecmdmPANAMIS == m_nCmdMode )//Delete "|| ecmdmSANYO == m_nCmdMode", 20141013, bug fixed
				{
					safearray_inp.GetElement(&nIndex, &cRxData[nCount]);
					if(cRxData[nCount] == 0x03)
					{
						nCount++;
						cRxData[nCount] = '\r';
					}
				}
				else
				{
					safearray_inp.GetElement(&nIndex, &cRxData[nCount]);
				}

				// "\r   --> \r\n"
				// "\r\n --> \r\n"
				// "\n   --> \r\n"
				// "\n\n --> \r\n\r\n"
				// "\r\r --> \r\n\r\n"
				if(cRxData[nCount] == '\r' || cRxData[nCount] == '\n')
				{
					cRxEndLine[nRxEndLineCount] = cRxData[nCount];
					nRxEndLineCount++;
				}
				else if(nRxEndLineCount > 0)
				{
					cRxEndLine[nRxEndLineCount] = cRxData[nCount];
					nRxEndLineCount++;
				}

				if(nRxEndLineCount == 1)
				{
					bEndLine = true;
					cRxData[nCount] = '\r';
					nCount++;
					cRxData[nCount] = '\n';
				}
				else if(nRxEndLineCount == 2)
				{
					if( (cRxEndLine[0] == '\n' && cRxEndLine[1] == '\n')
						|| (cRxEndLine[0] == '\r' && cRxEndLine[1] == '\r')
					)
					{
						bEndLine = true;
						cRxData[nCount] = '\r';
						nCount++;
						cRxData[nCount] = '\n';
					}
					else if(cRxEndLine[0] == '\r' && cRxEndLine[1] == '\n')
					{
						cRxData[nCount] = 0;
						nCount--;
						// 0D 0A Comes, need to clear the ShowLog and SystemTime
						// Because the OA can't be execute at this time.
						// when OD comes, it's already been changed to 0D 0A.
						if(m_bShowSendString)
						{
							for(int nTemp = 0; nTemp < nRecieveStringLenth; nTemp++)
							{
								cRxData[nCount] = 0;
								nCount--;
							}
							bEndLine = true;
						}
						if(m_bShowSystemTime)
						{
							for(int nTemp = 0; nTemp < nSystemTimeLenth; nTemp++)
							{
								cRxData[nCount] = 0;
								nCount--;
							}
							bEndLine = true;
						}
					}
					nRxEndLineCount = 0;
					cRxEndLine[0] = 0;
					cRxEndLine[1] = 0;
				}
				else
				{
					nRxEndLineCount = 0;
				}

				if(cRxData[nCount] == 0x00)
				{
					cRxData[nCount] = '.';
				}
				nCount++;

				if(nCount >= sizeof(cRxData)/sizeof(cRxData[0]))
				{
					MessageBox(_T("The Memory Error, The Serial COM comes too much data during 1ms\n"));
				}
			}

			strMessage=cRxData;
			if(m_bShowReceiveString)
			{
				m_strCOMLog += strMessage;
			}
			else if(m_ReadMsgDialog.GetQuitStatus() == FALSE
				&& m_ReadMsgDialog.GetContinueStatus() == FALSE
				&& m_bShellSendAlready)
			{
				CString strWarn = m_ReadMsgDialog.GetWarnMsg();
				strWarn += strMessage;
				m_ReadMsgDialog.SetWarnMsg(strWarn);
				m_ReadMsgDialog.ShowWarnMsg();
			}
			
			m_strCOMReciveString += strMessage;  // Use for Memmory All recived string.

			if(bEndLine && (m_bShellSendAlready || m_bShellSendWaitString))
			{
				// Store the last recive string, last line 
				int nFirstPos = m_strCOMReciveString.ReverseFind('\r');
				if(nFirstPos != -1 && nFirstPos > 1)
				{
					m_strCurrentReciveString = m_strCOMReciveString.Mid(0, nFirstPos);
					int nSecondPos = m_strCurrentReciveString.ReverseFind('\n');
					if(nSecondPos == -1)
					{
						nSecondPos = 0;
					}
					m_strCurrentReciveString = m_strCOMReciveString.Mid(nSecondPos, nFirstPos);
					m_strCurrentReciveString.Trim(_T("\r\n"));
				//	m_strCOMLog += m_strCurrentReciveString;
				}

				if(m_bShellSendAlready)
				{
					m_bShellSendAlready = FALSE;
				}
				else if(m_bShellSendWaitString)
				{
					m_bShellSendWaitString = FALSE;
					SetTimer(ID_SEND_MODE_TIME, 1, NULL);
				}
			}
			

			if(m_bComSyncStatus)
			{
				BOOL bCmdCheckStatus = ShellCommunicationCheck();
				if(bCmdCheckStatus)
				{
					m_bComSyncStatus = FALSE;
					switch(m_nShellMode)
					{
						case esmEEPROMWRITE:
							KillTimer(ID_SEND_EEPROMDATA_TIME);
							m_ReadMsgDialog.UpdateProgress();
							ShellEepromWrite();
							break;

						case esmEEPROMREAD:
							KillTimer(ID_SEND_EEPROMDATA_TIME);
							m_ReadMsgDialog.UpdateProgress();
							ShellEepromRead();
							break;

						case esmEEPROMDAMAGE:
							KillTimer(ID_EEPROM_DAMAGE_TIME);
							ShellEepromDamage();
							break;

						default:
							break;
					}
				}
			}

			SetComLogDisplayBottom();
		// strMessage.AppendFormat(_T("Recieve data %s"), rxdata);
		//	MessageBox(strMessage);
			break;

		default:
			break;
	} 
}

//============================================================
// MFC Widget Message Function define
//============================================================
void CLSCToolDlg::OnBnClickedButtonComsetup()
{
	int nIndex = 0;
	CSerialComSetup SerialComSetupDialog;

	// Set COM Number
	SerialComSetupDialog.si_fnSerialComNumberSet(m_nComNumber);

	// Set COM Info
	SerialComSetupDialog.si_fnSerialComSetupSet(m_strComInfo);
	if(SerialComSetupDialog.DoModal() == IDOK)
	{
		SerialComSetupDialog.si_fnSerialComSetupGet(&m_strComInfo);
		m_nComNumber = SerialComSetupDialog.si_fnSerialComNumberGet();		

		// Stop the repeat timer while change the COM Port 
		KillTimer(ID_SEND_REPEAT_TIME);
		
		// Pop up warning message to ask user, need to change the connect right now or not.
		if(IDOK == MessageBox(_T("是否直接应用当前设置"), _T("是否要应用当前设置"), MB_OKCANCEL ))
		{
			if (m_MSComm.get_PortOpen())  
			{  
				m_MSComm.put_PortOpen(FALSE);  
			}
			OnBnClickedButtonConnect();
		}
		else
		{
			// Make the Serial COM off
			if (m_MSComm.get_PortOpen())  
			{  
				m_MSComm.put_PortOpen(FALSE);  
			}
			
			// Show the Connect Button and grayout the Dis-connect button
			GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(FALSE);
		}

		// Show the Com Info static text
		ShowCurrentComInfo();

		// Over write to the ini file.
		WriteComSetupInfoToInitFile();
	}
}

void CLSCToolDlg::OnBnClickedButtonConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	bool bConnect = ComConnect();
	if(bConnect)
	{
		GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(TRUE);
	} 
	else  
	{  
		GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(FALSE);
		CString strMessage = _T("");
		strMessage.AppendFormat(_T("COM%d无法设置,请确认！"), m_nComNumber);
		MessageBox(strMessage);
	}
}

void CLSCToolDlg::OnBnClickedButtonDisconnect()
{
	// TODO: 在此添加控件通知处理程序代码

	//Repeat Timer Clear
	CButton *pCheckButton = (CButton *)GetDlgItem(IDC_CHECK_SENDREPEAT);
	pCheckButton->SetCheck(0);
	KillTimer(ID_SEND_REPEAT_TIME);
	if (m_MSComm.get_PortOpen())  
    {  
        m_MSComm.put_PortOpen(FALSE);  
    }
	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(FALSE);
	UpdateData(false);
}

void CLSCToolDlg::OnBnClickedButtonSend()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_strSendString != _T(""))
	{
		// Send the string to COM
		eRESULT eResult = ComSendString(m_strSendString);
		if(eResult == erSUCCESS)
		{
			// Check to Show the Send String
			if(m_bShowSendString)
			{
				if(m_bShowSystemTime)
				{
					CString strSysteTime = _T("");
					strSysteTime = GetSystemTime();
					m_strCOMLog = m_strCOMLog + LOG_SEND_STRING + strSysteTime + m_strSendString + _T("\r\n");
				}
				else
				{
					m_strCOMLog = m_strCOMLog + LOG_SEND_STRING + m_strSendString + _T("\r\n");
				}

				// Set the VBar to the Bottom
				SetComLogDisplayBottom();
			}
		
			// Check Repeat Timer
			CButton *pCheckButton = (CButton *)GetDlgItem(IDC_CHECK_SENDREPEAT);
			if(pCheckButton->GetCheck())
			{
				// Get the Repeat time and then set the Timer
				if(m_nRepeatTime == 0)
				{
					KillTimer(ID_SEND_REPEAT_TIME);
				}
				else
				{
					SetTimer(ID_SEND_REPEAT_TIME, m_nRepeatTime, NULL);	
				}
			}
			else
			{
				KillTimer(ID_SEND_REPEAT_TIME);
			}

			// Save the Log to Combo log list
			int nIndex = m_strComboInputLog.FindStringExact(0, m_strSendString);
			if(nIndex == CB_ERR)
			{
				// Can't find the same
				m_strComboInputLog.AddString(m_strSendString);

				// Saving send log to the INI File
				WriteSendLogToInitFile(m_strSendString);
				m_nSendLogCount++;
			}

			// Saving last string to the INI File
			if(m_strSendString != m_strSendLastString)
			{
				m_strSendLastString = m_strSendString;
				WriteLastStringToInitFile(m_strSendString);
			}

			// Saving Last Command mode
			if(m_nRepeatTimeLast != m_nRepeatTime)
			{
				m_nRepeatTimeLast = m_nRepeatTime;
				WriteRepeatTimeToInitFile();
			}

			// Saving Last Repeat time
			if(m_nCmdModeLast != m_nCmdMode)
			{
				m_nCmdModeLast = m_nCmdModeLast;
				WriteCmdModeToInitFile();
			}
		}
		else
		{
			MessageBox(_T("请确保输入字符串是16进制字符并且以空格间隔\r\n 例如：02 00 FE 03 0A \r\n"),_T("输入非法字符"), MB_ICONEXCLAMATION|MB_OK );
		}
	}
}

void CLSCToolDlg::OnBnClickedButtonClearscreen()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	m_strCOMLog = _T("");
	m_strCOMReciveString = _T("");
	m_bEditComLogDisplayComplete = TRUE;
	SetDlgItemText(IDC_STATIC_COMLOG, _T(""));
	UpdateData(false);

}


void CLSCToolDlg::OnBnClickedButtonClearsendlog()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strComboInputLog.ResetContent();
	m_strComboInputLog.AddString(_T("  ↓↓↓    发送记录    ↓↓↓"));
	m_strComboInputLog.SetCurSel(0);
	UpdateData(false);

	// Delete the INI Send string log
	ClearSendLogToInitFile();

}
void CLSCToolDlg::OnBnClickedButtonOpenlog()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_strSaveLogPath != _T(""))
	{
		ShellExecute(this->m_hWnd, _T("open"),_T("notepad.exe"), m_strSaveLogPath, _T(""), SW_SHOW );
	}
	else
	{
		MessageBox(_T("找不到保存记录，请确认是否已经保存？"),_T("未保存记录"), MB_ICONEXCLAMATION|MB_OK );
	}

}

void CLSCToolDlg::OnBnClickedButtonSendlog()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR szFilter[] = _T("文本文件(*.txt)|*.txt|所有文件(*.*)|*.*||");
	CFileDialog fileDlg(FALSE, _T("txt"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString strFilePath;
	if (IDOK == fileDlg.DoModal()) 
	{
		strFilePath = fileDlg.GetPathName();
		
		// Save the Log
		WCHAR wUnicodeHead = 0xfeff;
		CFile SaveFile(strFilePath, CFile::modeCreate|CFile::modeWrite);
		SaveFile.Write(&wUnicodeHead, 2);
		SaveFile.Write(m_strCOMLog, m_strCOMLog.GetLength()*sizeof(wchar_t));
		SaveFile.Flush();
		SaveFile.Close();

		m_strSaveLogPath = strFilePath;
	}
}
#include <locale.h>
void CLSCToolDlg::OnBnClickedButtonInputshell()
{
	// TODO: 在此添加控件通知处理程序代码
	// Support Shell
	WCHAR szFilters[]= _T("Shell Files (*.txt)|*.txt|All Files (*.*)|*.*||");
	CFileDialog fileDlg(TRUE, _T("txt"), _T("*.txt"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilters, NULL);
	CString strFilePathName = _T("");
	if( fileDlg.DoModal ()==IDOK )
	{
		strFilePathName = fileDlg.GetPathName();
		m_strShellPathName = strFilePathName;

		// Open the Shell
		//m_stdioReadFile.Close();
		setlocale( LC_CTYPE, "chs" );
		m_stdioReadFile.Open(strFilePathName, CFile::modeRead);
		m_nShellMaxLine = ShellGetMaxLine();
		eRESULT eResult = ShellFileCheck();
		if(erSUCCESS == eResult)
		{
			switch(m_nShellMode)
			{
				case esmEEPROMBIN2TSCMD:
					if(IDOK == MessageBox(_T("脚本加载成功! \r\n请选择一个EEPROM BIN文件 \r\n继续请点击确定\r\n"),_T("脚本加载成功"), MB_OKCANCEL ))
					{
						ShellBinCovertTSCmd();
					}
					m_stdioReadFile.Close();
					break;

				case esmEEPROMBIN2G$WCMD:
					if(IDOK == MessageBox(_T("脚本加载成功! \r\n请选择一个EEPROM BIN文件 \r\n继续请点击确定\r\n"),_T("脚本加载成功"), MB_OKCANCEL ))
					{
						ShellBinCovertGWCmd();
					}
					m_stdioReadFile.Close();
					break;

				case esmEEPROMWRITE:
					if(IDOK == MessageBox(_T("脚本加载成功! \r\n请确认是否要进行EEPROM WRITE操作 \r\n继续请点击确定\r\n"),_T("脚本加载成功"), MB_OKCANCEL ))
					{
						ShellEepromWrite();
						m_ReadMsgDialog.SetWarnMsg(_T("正在执行脚本中，如果需要停止，请点击退出!"));
						m_ReadMsgDialog.SetProgressRange(m_nShellMaxLine);
						m_ReadMsgDialog.SetQuitStatus(FALSE);
						m_ReadMsgDialog.DoModal();
					}
					else
					{
						m_stdioReadFile.Close();
					}
					break;

				case esmEEPROMREAD:
					if(IDOK == MessageBox(_T("脚本加载成功! \r\n请确认是否要进行EEPROM READ操作 \r\n继续请点击确定\r\n"),_T("脚本加载成功"), MB_OKCANCEL ))
					{
						ShellEepromRead();
						m_ReadMsgDialog.SetWarnMsg(_T("正在执行脚本中，如果需要停止，请点击退出!"));
						m_ReadMsgDialog.SetProgressRange(m_nShellMaxLine);
						m_ReadMsgDialog.SetQuitStatus(FALSE);
						m_ReadMsgDialog.DoModal();
					}
					else
					{
						m_stdioReadFile.Close();
					}
					break;

				case esmEEPROMDAMAGE:
					if(IDOK == MessageBox(_T("脚本加载成功! \r\n请确认是否要进行EEPROM破坏操作\r\n请注意该破坏操作有可能损坏EEPROM器件\r\n继续请点击确定\r\n"),_T("脚本加载成功"), MB_OKCANCEL ))
					{
						m_eDamageState = edmINIT;
						ShellEepromDamage();
						m_ReadMsgDialog.SetWarnMsg(_T("正在疯狂的破坏中，如果需要停止，请点击退出!"));
						m_ReadMsgDialog.SetProgressRange(m_nShellMaxLine);
						m_ReadMsgDialog.SetQuitStatus(FALSE);
						m_ReadMsgDialog.DoModal();
					}
					else
					{
						m_stdioReadFile.Close();
					}
					//m_stdioReadFile.Close();
					break;

				case esmTRCMDGENERATE:
					if(IDOK == MessageBox(_T("脚本加载成功! \r\n请确认是否要生成TR指令操作 \r\n继续请点击确定\r\n"),_T("脚本加载成功"), MB_OKCANCEL ))
					{
						ShellTRCommandGenerate();
					}
					m_stdioReadFile.Close();
					break;

				case esmGRCMDGENERATE:
					if(IDOK == MessageBox(_T("脚本加载成功! \r\n请确认是否要生成G$R指令操作 \r\n继续请点击确定\r\n"),_T("脚本加载成功"), MB_OKCANCEL ))
					{
						ShellGRCommandGenerate();
					}
					m_stdioReadFile.Close();
					break;


				case esmSENDMODE:
					if(IDOK == MessageBox(_T("脚本加载成功! \r\n请确认是否要执行脚本操作 \r\n继续请点击确定\r\n"),_T("脚本加载成功"), MB_OKCANCEL ))
					{
						m_bShellSendStart = TRUE;
						CLSCStack<unsigned int> stLscStackBreak;
						CLSCStack<int> stLscStackRepeatCount;
						CLSCStack<int> stLscStackShellRepeatCount;
						CLSCStack<ULONGLONG> stLscStackRepeatPos;
						CLSCStack<ULONGLONG> stLscStackCallPos;
						m_pstLscStackBreak = &stLscStackBreak;
						m_pstLscStackRepeatCount = &stLscStackRepeatCount;
						m_pstLscStackRepeatPos = &stLscStackRepeatPos;
						m_pstLscStackShellRepeatCount = &stLscStackShellRepeatCount;
						m_pstLscStackCallPos = &stLscStackCallPos;
						m_pstLscStackBreak->CreateStack(BREAK_TYPE_COUNT);
						m_pstLscStackRepeatCount->CreateStack(REPEAT_LOOP_COUNT);
						m_pstLscStackShellRepeatCount->CreateStack(REPEAT_LOOP_COUNT);
						m_pstLscStackRepeatPos->CreateStack(REPEAT_LOOP_COUNT);
						m_pstLscStackCallPos->CreateStack(CALL_STACK_COUNT);
						ShellSendMode();
						m_ReadMsgDialog.SetStopQuitStatus(TRUE);
						m_ReadMsgDialog.SetWarnMsg(_T("正在执行脚本，如果需要暂停或者退出，请点击指定按钮!"));
						m_ReadMsgDialog.SetProgressRange(m_nShellMaxLine);
						m_ReadMsgDialog.SetQuitStatus(FALSE);
						m_ReadMsgDialog.DoModal();
						m_pstLscStackBreak->DeleteStack();
						m_pstLscStackRepeatCount->DeleteStack();
						m_pstLscStackShellRepeatCount->DeleteStack();
						m_pstLscStackRepeatPos->DeleteStack();
						m_pstLscStackCallPos->DeleteStack();
					}
					else
					{
						m_stdioReadFile.Close();
					}
					break;

				default:
					break;
			}
		}
		else
		{
			m_stdioReadFile.Close();
			MessageBox(_T("脚本加载失败! \r\n请确认脚本是否有误 \r\n"),_T("脚本加载失败"), MB_OK );
		}
	}
}

void CLSCToolDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	eRESULT eResult = erSUCCESS;
	switch(nIDEvent)
	{
		case ID_SEND_REPEAT_TIME:
			eResult = ComSendString(m_strSendString);
			if(erSUCCESS == eResult)
			{
				if(m_bShowSendString)
				{
					if(m_bShowSystemTime)
					{
						CString strSysteTime = _T("");
						strSysteTime = GetSystemTime();
						m_strCOMLog = m_strCOMLog + LOG_SEND_STRING + strSysteTime + m_strSendString + _T("\r\n");
					}
					else
					{
						m_strCOMLog = m_strCOMLog + LOG_SEND_STRING + m_strSendString + _T("\r\n");
					}
					SetComLogDisplayBottom();
				}
			}
			break;
		
		case ID_SEND_EEPROMDATA_TIME:
			ShellStop();
			m_ReadMsgDialog.SetQuitStatus(TRUE);
			m_ReadMsgDialog.SetWarnMsg(_T("通信异常! \r\n请确认串口连接 \r\n"));
			m_ReadMsgDialog.ShowWarnMsg();
			break;

		case ID_EEPROM_DAMAGE_TIME:
			m_eDamageState = edmCHECKEEROR;
			ShellEepromDamage();
			break;


		case ID_SEND_MODE_TIME:
			KillTimer(ID_SEND_MODE_TIME);
			ShellSendMode();
			break;


		default:
			break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CLSCToolDlg::OnBnClickedCheckSendrepeat()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton *pCheckButton = (CButton *)GetDlgItem(IDC_CHECK_SENDREPEAT);
	if(pCheckButton->GetCheck())
	{
		// Can't Edit the repeat time
		GetDlgItem(IDC_EDIT_REPEATTIME)->EnableWindow(FALSE);
	}
	else
	{
		KillTimer(ID_SEND_REPEAT_TIME);
		GetDlgItem(IDC_EDIT_REPEATTIME)->EnableWindow(TRUE);
	}
}

void CLSCToolDlg::OnBnClickedCheckShowtime()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton *pCheckButton = (CButton *)GetDlgItem(IDC_CHECK_SHOWTIME);
	if(pCheckButton->GetCheck())
	{
		m_bShowSystemTime = true;
	}
	else
	{
		m_bShowSystemTime = false;
	}
}


void CLSCToolDlg::OnBnClickedCheckShowsendlog()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton *pCheckButton = (CButton *)GetDlgItem(IDC_CHECK_SHOWSENDLOG);
	if(pCheckButton->GetCheck())
	{
		m_bShowSendString = true;
	}
	else
	{
		m_bShowSendString = false;
	}
}


void CLSCToolDlg::OnCbnSelchangeComboInputlog()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_strComboInputLog.GetCurSel();
	if(nIndex != 0)
	{
		CString strSelectText;
		m_strComboInputLog.GetLBText( nIndex, strSelectText);
		m_strSendString = strSelectText;

		// The Vbar get to the bottom;
		CEdit* pedit = (CEdit*)GetDlgItem(IDC_EDIT_INPUT);
		pedit->SetWindowTextW(m_strSendString);
	}
}

void CLSCToolDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	// Enter Key. same to Send Button
	OnBnClickedButtonSend();
//	OnOK();
}

void CLSCToolDlg::ShowCurrentComInfo(void)
{
	CString strStaticTextComInfo = _T("");
	WCHAR cBaudRateBuffer[20] = {0};
	CString strBaudRate = _T("波特率：");
	int nBaudRateLength = 0;
	int nIndex = 0;

	nBaudRateLength = m_strComInfo.Find(_T(","));
	if(nBaudRateLength != -1)
	{
		for(nIndex = 0; nIndex < nBaudRateLength; nIndex++)
		{
			cBaudRateBuffer[nIndex] = m_strComInfo[nIndex];
		}
		strBaudRate = strBaudRate + cBaudRateBuffer + _T(" bps");
	}

	strStaticTextComInfo.AppendFormat(_T("当前使用：COM%d   "), m_nComNumber);
	strStaticTextComInfo += strBaudRate;
	SetDlgItemText(IDC_STATIC_COMINFO, strStaticTextComInfo);
}

void CLSCToolDlg::OnBnClickedRadioCmdSanyo()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
}

void CLSCToolDlg::OnBnClickedRadioCmdMis()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
}

void CLSCToolDlg::OnBnClickedRadioCmdBin()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
}

void CLSCToolDlg::OnBnClickedRadioCmdText()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
}

void CLSCToolDlg::OnBnClickedButtonQuit()
{
	// TODO: 在此添加控件通知处理程序代码
	if(IDOK == MessageBox(_T("确定退出程序吗？"),_T("退出程序"), MB_ICONHAND|MB_OKCANCEL ))
	{
		OnCancel();
	}
}

void CLSCToolDlg::OnEnVscrollEditComlog()
{
	// TODO: 在此添加控件通知处理程序代码
	//const int EDIT_MAX_LINE = 38;
	static int sm_nVert = 0;
	int nMinVScroll = 0;
	int nMaxVScroll = 0;
	CEdit* pedit = (CEdit*)GetDlgItem(IDC_EDIT_COMLOG);
	int nMaxLen = pedit->GetLineCount();

	sm_nVert = pedit->GetScrollPos(SB_VERT);
	pedit->GetScrollRange(SB_VERT, &nMinVScroll, &nMaxVScroll);

	// Scroll to the Bottom need to update data again
	if((sm_nVert + EDIT_COMLOG_MAX_LINE) >= nMaxVScroll && m_bEditComLogDisplayComplete == FALSE)
	{
		m_bEditComLogDisplayComplete = TRUE;
		pedit->ShowWindow(SW_HIDE);
		pedit->SetWindowTextW(m_strCOMLog);
		pedit->LineScroll(sm_nVert);
		//m_bEditComLogDisplayComplete = TRUE;
		pedit->ShowWindow(SW_SHOW);
		//GetDlgItem(IDC_STATIC_COMLOG)->ShowWindow(SW_HIDE);
		SetDlgItemText(IDC_STATIC_COMLOG, _T(""));
	}
}


//============================================================
// INI File Function define
//============================================================
void CLSCToolDlg::CheckProgramInitFile(void)
{
//	CStdioFile InitFile;
//	CFileException fileException;
	CString strInitFilePath = GetWorkDir();
	strInitFilePath += INIT_FILE_NAME;
	
	// Find the INI File
	CString strSectionName = _T("SerialComSetup");
	CFileFind finder;
	BOOL ifFind = finder.FindFile(strInitFilePath); 
	m_strInitFilePath = strInitFilePath;  // Save the Init File Path
	if( !ifFind )
	{
		m_nCmdMode = 0;
		m_nCmdModeLast = 0;
		m_nRepeatTime = 500;
		m_nRepeatTimeLast = 500;
		m_nComNumber = 1;
		m_strComInfo = _T("19200, n, 8, 1");
		WritePrivateProfileString(strSectionName, _T("ComNo"), _T("1"), strInitFilePath);
		WritePrivateProfileString(strSectionName, _T("ComInfo"), m_strComInfo, strInitFilePath);
		WritePrivateProfileString(strSectionName, _T("ComMode"), _T("0"), strInitFilePath);
		WritePrivateProfileString(strSectionName, _T("RepeatTime"), _T("500"), strInitFilePath);
		
		// Send String Log
		WritePrivateProfileString(_T("SendString"), _T("LastString"), _T(""), strInitFilePath);
		
		// Default input log value
		m_strComboInputLog.AddString(_T("  ↓↓↓    发送记录    ↓↓↓"));
		m_strComboInputLog.SetCurSel(0);

		UpdateData(false);
	}
	else
	{ 
		WCHAR wTempBuffer[128] = {0};
		m_nComNumber = GetPrivateProfileInt(strSectionName, _T("ComNo"), 1, strInitFilePath);
		GetPrivateProfileString(strSectionName, _T("ComInfo"), _T("19200, n, 8, 1"), (LPWSTR)wTempBuffer, 128, strInitFilePath);
		m_strComInfo = wTempBuffer;

		// Get Command mode 
		m_nCmdMode = GetPrivateProfileInt(strSectionName, _T("ComMode"), 0, strInitFilePath);
		m_nCmdModeLast = m_nCmdMode;
		// Get Repeat time
		m_nRepeatTime = GetPrivateProfileInt(strSectionName, _T("RepeatTime"), 500, strInitFilePath);
		m_nRepeatTimeLast = m_nRepeatTime;

		// Get last string log
		GetPrivateProfileString(_T("SendString"), _T("LastString"), NULL, (LPWSTR)wTempBuffer, 128, strInitFilePath);
		m_strSendString = wTempBuffer; // Last string update to the edit dialog
		m_strSendLastString = m_strSendString;
		UpdateData(false);
		
		// Cycle to get the Send String Log.
		m_strComboInputLog.AddString(_T("  ↓↓↓    发送记录    ↓↓↓"));
		m_strComboInputLog.SetCurSel(0);
		int nCount = 0;
		bool bContinue = true;
		int nLogStringLength = 0;
		CString strKeyString = _T("");
		CString strLogString = _T("");
		do
		{
			nCount++;
			strKeyString = _T("");
			strKeyString.AppendFormat(_T("LIST%d"), nCount);
			nLogStringLength = GetPrivateProfileString(_T("SendString"), strKeyString, NULL, (LPWSTR)wTempBuffer, 128, strInitFilePath);

			if(nLogStringLength > 0)
			{
				int nFindResult = 0;
				strLogString = wTempBuffer;
				nFindResult = m_strComboInputLog.FindStringExact(0, strLogString);
				if(nFindResult == CB_ERR)
				{
					// Can't find the same
					m_strComboInputLog.AddString(strLogString);
				}
			}
			else
			{
				// Save Current available Log count
				m_nSendLogCount = nCount;
				bContinue = false;
			}
		}while(bContinue);
			
	}
}

void CLSCToolDlg::WriteSendLogToInitFile(CString strSendLog)
{
	if(m_strInitFilePath != _T(""))
	{
		CString strKeyString = _T("");
		strKeyString.AppendFormat(_T("LIST%d"), m_nSendLogCount);
		WritePrivateProfileString(_T("SendString"), strKeyString, strSendLog, m_strInitFilePath);
	}
}


void CLSCToolDlg::WriteLastStringToInitFile(CString strSendLog)
{
	if(m_strInitFilePath != _T(""))
	{
		WritePrivateProfileString(_T("SendString"), _T("LastString"), strSendLog, m_strInitFilePath);
	}
}

void CLSCToolDlg::WriteRepeatTimeToInitFile(void)
{
	if(m_strInitFilePath != _T(""))
	{
		CString strSectionName = _T("SerialComSetup");
		CString strRepeatTime = _T("");
		strRepeatTime.AppendFormat(_T("%d"), m_nRepeatTime);
		WritePrivateProfileString(strSectionName, _T("RepeatTime"), strRepeatTime, m_strInitFilePath);
	}
}

void CLSCToolDlg::WriteCmdModeToInitFile(void)
{
	if(m_strInitFilePath != _T(""))
	{
		CString strSectionName = _T("SerialComSetup");
		CString strCmdMode = _T("");
		strCmdMode.AppendFormat(_T("%d"), m_nCmdMode);
		WritePrivateProfileString(strSectionName, _T("ComMode"), strCmdMode, m_strInitFilePath);
	}
}

void CLSCToolDlg::WriteComSetupInfoToInitFile(void)
{
	if(m_strInitFilePath != _T(""))
	{
		CString strSectionName = _T("SerialComSetup");
		CString strComNumber = _T("");
		strComNumber.AppendFormat(_T("%d"), m_nComNumber);
		WritePrivateProfileString(strSectionName, _T("ComNo"), strComNumber, m_strInitFilePath);
		WritePrivateProfileString(strSectionName, _T("ComInfo"), m_strComInfo, m_strInitFilePath);
	}
}

void CLSCToolDlg::ClearSendLogToInitFile(void)
{
	if(m_strInitFilePath != _T(""))
	{
		int nIndex = 0;
		for(nIndex = 0; nIndex < m_nSendLogCount; nIndex++)
		{
			CString strKeyString = _T("");
			strKeyString.AppendFormat(_T("LIST%d"), nIndex);
			WritePrivateProfileString(_T("SendString"), strKeyString, NULL, m_strInitFilePath);
		}
		m_nSendLogCount = 1;
		m_strSendLastString = _T("");
	}
}


//============================================================
// Internal Function define
//============================================================
CString CLSCToolDlg::GetWorkDir(void)
{
	WCHAR pFileName[MAX_PATH]; 
	int nPos = GetCurrentDirectory( MAX_PATH, pFileName); 
 
	CString csFullPath(pFileName);  
	if( nPos < 0 ) 
		return CString(""); 
	else 
		return csFullPath; 
}

CString CLSCToolDlg::GetSystemTime(void)
{
	SYSTEMTIME stSystemTime;
	CString strTime = _T("");
	GetLocalTime(&stSystemTime); 
	strTime.AppendFormat(_T("%02d:%02d:%02d | "), stSystemTime.wHour, stSystemTime.wMinute, stSystemTime.wSecond);
	return strTime;
}


int CLSCToolDlg::GetSystemTime(WCHAR * pwcBuffer)
{
	int nIndex = 0;
	int nLength = 0;
	CString strTime = _T("");
	strTime = GetSystemTime();
	nLength = strTime.GetLength();

	for(nIndex = 0; nIndex < nLength; nIndex++)
	{
		pwcBuffer[nIndex] = strTime[nIndex];
	}
	return nLength;
}

int CLSCToolDlg::GetStringToBuffer(CString strInput, WCHAR * pwBuffer)
{
	int nIndex = 0;
	int nLength = strInput.GetLength();
	for(nIndex = 0; nIndex < nLength; nIndex++)
	{
		pwBuffer[nIndex] = strInput[nIndex];
	}
	return nLength;
}


int CLSCToolDlg::GetStringToBuffer(CString strInput, char * pcBuffer)
{
	int nIndex = 0;
	int nLength = strInput.GetLength();
	for(nIndex = 0; nIndex < nLength; nIndex++)
	{
		pcBuffer[nIndex] = (char)strInput[nIndex];
	}
	return nLength;
}

CString CLSCToolDlg::DeleteStringTabSpaceLeft(CString strInput)
{
	CString strOutput = _T("");
	strOutput = strInput.TrimLeft(_T("\t "));
	return strOutput;
}

CString CLSCToolDlg::DeleteStringTabSpaceRight(CString strInput)
{
	CString strOutput = _T("");
	strOutput = strInput.TrimRight(_T("\t "));
	return strOutput;
}

bool CLSCToolDlg::ComConnect(void)
{
	bool bConnect = false;
	if(m_nComNumber < 16)
	{
		bool bValid = false;
		int nIndex = 0;
		int nValidComCount = 0;
		CString strCurrentCom = _T("");
		CString strValidPortComList[20];
		nValidComCount = GetSystemCom(strValidPortComList);
		strCurrentCom.AppendFormat(_T("COM%d"), m_nComNumber);
		for(nIndex = 0; nIndex < nValidComCount && nIndex < 20; nIndex++)
		{
			if(strCurrentCom == strValidPortComList[nIndex])
			{
				bValid = true;
				break;
			}
		}

		if(bValid)
		{
			m_MSComm.put_CommPort(m_nComNumber);
			m_MSComm.put_InBufferSize(SIZE_COM_BUFFER);  
			m_MSComm.put_OutBufferSize(SIZE_COM_BUFFER);    
			m_MSComm.put_InputMode(1);						//二进制模式  
			m_MSComm.put_Settings(m_strComInfo);  
			m_MSComm.put_RThreshold(1);						//响应函数中接收数据,没个数据产生一次事件
			m_MSComm.put_InputLen(0);						//一次读取全部数据
		
			if (!m_MSComm.get_PortOpen())  
			{  
				m_MSComm.put_PortOpen(TRUE);  
				bConnect = true;
			}  
			else  
			{  
				bConnect = false; 
			}
		}
		else
		{
			bConnect = false; 
		}
	}
	return bConnect;
}


eRESULT CLSCToolDlg::ComSendString(CString strSend)
{
	eRESULT eResult = erSUCCESS;
	CString strSendString = _T("");
	if(strSend != _T(""))
	{
		switch(m_nCmdMode)
		{
			case ecmdmSANYO:
				strSendString = strSend + _T("\r");
				m_MSComm.put_Output(COleVariant(strSendString));
				break;

			case ecmdmPANAMIS:
				{
					WCHAR wStart = 0x02;
					WCHAR wEnd = 0x03;
					strSendString = wStart +  strSend + wEnd;
					m_MSComm.put_Output(COleVariant(strSendString));
				}
				break;

			case ecmdmPANABIN:
				{
					int nIndex = 0;
					CByteArray cbDataBuffer;
					int nTempData = 0;
					unsigned char cTempData = 0;
					char cTempDataBuffer[3] = {0};
					bool bStringStatus = false;
					
					// Check the input string
					for(nIndex = 0; nIndex < strSend.GetLength(); nIndex++)
					{
						if(0 == ((nIndex + 1) % 3))
						{
							if(strSend[nIndex] == ' ')
							{
								bStringStatus = true;
							}
							else
							{
								bStringStatus = false;
								break;
							}
 						}
						else if( (strSend[nIndex] >= '0' && strSend[nIndex] <= '9')
							|| (strSend[nIndex] >= 'A' && strSend[nIndex] <= 'F')
							|| (strSend[nIndex] >= 'a' && strSend[nIndex] <= 'f'))
						{
							bStringStatus = true;
						}
						else
						{
							bStringStatus = false;
							break;
						}

					}

					if(bStringStatus)
					{
						for(nIndex = 0; nIndex < strSend.GetLength(); nIndex += 3)
						{
							cTempDataBuffer[0] = (unsigned char)strSend[nIndex];
							cTempDataBuffer[1] = (unsigned char)strSend[nIndex+1];
							sscanf(cTempDataBuffer,"%x",&nTempData) ;
							cTempData = (unsigned char)nTempData;
							cbDataBuffer.Add(cTempData);
						}
						m_MSComm.put_Output(COleVariant(cbDataBuffer));
					}
					else
					{
						eResult = erERROR;
					}
				}
				break;

			case ecmdmTEXT:
				strSendString = strSend;
				m_MSComm.put_Output(COleVariant(strSendString));
				break;

			default:
				strSendString = strSend;
				m_MSComm.put_Output(COleVariant(strSendString));
				break;
		}
	}
	return eResult;
}

bool CLSCToolDlg::ComInfoCheck(CString strInput)
{
	bool bValid = true;
	if(strInput != _T(""))
	{
		bool bEndString = false;
		int nIndex = 0;
		int nLength = 0;

		strInput.Remove(' ');
		strInput.Remove('\t');
		nLength = strInput.GetLength();


		// Check the COM Info String : Baudrate
		for(nIndex = 0; nIndex < nLength; nIndex++)
		{
			if(strInput[nIndex] == ',')
			{
				bEndString = true;
				break;
			}
			else if(strInput[nIndex] < '0' || strInput[nIndex] > '9')
			{
				bValid = false;
				break;
			}
		}

		if((nLength - nIndex - 1) == 5)
		{
			wchar_t cCheckStatus = strInput[nIndex + 1];
			wchar_t cDataBit = strInput[nIndex + 3];
			wchar_t cStopBit = strInput[nIndex + 5];
			
			if(cCheckStatus == 'n'
				|| cCheckStatus == 'o'
				|| cCheckStatus == 'e'
				|| cCheckStatus == 'N'
				|| cCheckStatus == 'O'
				|| cCheckStatus == 'E'
				|| cDataBit == '8'
				|| cDataBit == '7'
				|| cStopBit == '1'
				|| cStopBit == '2'
				)
			{
				bValid = true;	
			}
			else
			{
				bValid = false;
			}
		}
		else
		{
			bValid = false;
		}
	}
	else
	{
		bValid = false;
	}
	return bValid;
}

int CLSCToolDlg::GetSystemCom(CString * strPortNameList)
{     
    int nCount = 0;   
	HKEY hKey = NULL;

	long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("Hardware\\DeviceMap\\SerialComm"),0,KEY_READ,&hKey); //打开注册表
	if( lRet != ERROR_SUCCESS )
	{
		MessageBox(_T("无法找到串口！"));
		return 0;
	}

	WCHAR cValueName[_MAX_PATH];    //键值名称
	DWORD dwValueType;				//键值类型
	WCHAR cValueData[_MAX_PATH];    //键值数值
	do
	{
		DWORD nValueNameSize = _MAX_PATH; //重置键值名称长度
		DWORD nValueDataSize = _MAX_PATH; //重置键值数值长度
		lRet = RegEnumValue(hKey, nCount, (LPWSTR)cValueName, &nValueNameSize, 0, &dwValueType, (LPBYTE)cValueData, &nValueDataSize); //枚举键值
		
		if(lRet == ERROR_SUCCESS)
		{  
			if(strPortNameList)   
				strPortNameList[nCount] = cValueData; 
			nCount++;
		}
	}while(lRet == ERROR_SUCCESS);   //读取完毕后退出循环
    return nCount;   
} 

void CLSCToolDlg::SetComLogDisplayBottom(void)
{
#if 0
	// The Vbar get to the bottom
	int nScrolLine = 0;
	int nMaxLine = 0;
	//UpdateData(FALSE);
	CEdit* pedit = (CEdit*)GetDlgItem(IDC_EDIT_COMLOG);

	pedit->ShowWindow(false);
//	UpdateData(FALSE);
	pedit->SetWindowTextW(m_strCOMLog);
//	pedit->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_BOTTOM, 0), 0);
	//nMaxLine = pedit->GetLineCount();
	//nScrolLine = pedit->GetScrollPos(SB_VERT);
	//if(nScrolLine == nMaxLine || nScrolLine == 0)
	//{
	pedit->LineScroll(pedit->GetLineCount());
	pedit->ShowWindow(true);
	//}
#else
	int nVert = 0;
	int nMinVScroll = 0;
	int nMaxVScroll = 0;
	CEdit* pedit = (CEdit*)GetDlgItem(IDC_EDIT_COMLOG);

	nVert = pedit->GetScrollPos(SB_VERT);
	pedit->GetScrollRange(SB_VERT, &nMinVScroll, &nMaxVScroll);

	if((nVert + EDIT_COMLOG_MAX_LINE) >= nMaxVScroll || m_bShowBottom)
	{
		m_bEditComLogDisplayComplete = TRUE;
		pedit->ShowWindow(SW_HIDE);
		pedit->SetWindowTextW(m_strCOMLog);
		pedit->LineScroll(pedit->GetLineCount());	
		pedit->ShowWindow(SW_SHOW);
		//m_bEditComLogDisplayComplete = TRUE;
		SetDlgItemText(IDC_STATIC_COMLOG, _T(""));
	}
	else
	{
		if(m_bEditComLogDisplayComplete)
		{
			SetDlgItemText(IDC_STATIC_COMLOG, _T("还有数据没有显示，下拉到底部刷新"));
		}
		m_bEditComLogDisplayComplete = FALSE;
	}
#endif 
}

void CLSCToolDlg::ShellBinCovertTSCmd(void)
{
	WCHAR szFilters[]= _T("bin Files (*.bin)|*.bin|All Files (*.*)|*.*||");
	CFileDialog fileDlg(TRUE, _T("bin"), _T("*.bin"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilters, NULL);
	CString strFilePathName = _T("");
	CString strWriteFilePath = GetWorkDir();
	if( fileDlg.DoModal ()==IDOK )
	{
		strFilePathName = fileDlg.GetPathName();
		strWriteFilePath = strWriteFilePath + _T("\\") + fileDlg.GetFileTitle() + _T("ConvertTSCmdShell.txt");
		CFile WriteFile(strWriteFilePath, CFile::modeCreate|CFile::modeWrite);
		CFile ReadFile(strFilePathName, CFile::modeRead | CFile::typeBinary);
		int nIndex = 0;
		ULONGLONG nFileLength = ReadFile.GetLength();
		char cTempBuffer[64] = {0};
		char cTempAddrBuffer[10] = {0};
		char cTempConvertBuffer[10] = {0};
		char cTempTSCmdBuffer[30] = {0};
		int nTS_DATA_MAX = 64;

		WriteFile.Write("SHELLMODE = EEPROM WRITE\r\n", 26);
		WriteFile.Write("CMDMODE = SANYO\r\n", 17);
		WriteFile.Write("TS NVMODEON\r\n", 13);
		for(nIndex = 0; nIndex < nFileLength; nIndex += nTS_DATA_MAX)
		{
			ReadFile.Read(&cTempBuffer[0], nTS_DATA_MAX);	
			sprintf( &cTempAddrBuffer[0], "%04X", nIndex);
			sprintf(cTempTSCmdBuffer, "TS NVDATA %s %02d : ", cTempAddrBuffer, nTS_DATA_MAX);
			WriteFile.Write(cTempTSCmdBuffer, 20);
			for(int nDataCount = 0; nDataCount < nTS_DATA_MAX; nDataCount++)
			{
				sprintf( &cTempConvertBuffer[0], "%02X", cTempBuffer[nDataCount]);
				WriteFile.Write(cTempConvertBuffer, 2);
				if(nDataCount < nTS_DATA_MAX - 1)
				{
					WriteFile.Write(" ", 1);
				}
			}
			WriteFile.Write("\r\n", 2);
		}
		int nLastData = nFileLength % nTS_DATA_MAX;
		if( nLastData != 0)
		{
			sprintf( &cTempAddrBuffer[0], "%04X", nFileLength - nLastData);
			sprintf(cTempTSCmdBuffer, "TS NVDATA %s %02d : ", cTempAddrBuffer, nLastData);
			WriteFile.Write(cTempTSCmdBuffer, 20);
			ReadFile.Read(&cTempBuffer[0], nLastData);
			for(nIndex = 0; nIndex < nLastData; nIndex++)
			{
				sprintf( &cTempConvertBuffer[0], "%02X", cTempBuffer[nIndex]);
				WriteFile.Write(cTempConvertBuffer, 2);
				if(nIndex < nLastData - 1)
				{
					WriteFile.Write(" ", 1);
				}
			}
			WriteFile.Write("\r\n", 2);
		}
		ReadFile.Close();
		WriteFile.Close();

		// Warning message
		CString strWarnMsg = _T("已经转换完成 \r\n路径为：");
		strWarnMsg += strWriteFilePath;
		MessageBox(strWarnMsg,_T("BIN文件转换完成"), MB_OK );
	}
}

void CLSCToolDlg::ShellBinCovertGWCmd(void)
{
	int nDeviceAddr = -1;
	char cDeviceAddr[32] = {0};
	char cDeviceID[3] = {0};
	CString strInputString = _T("");
	m_stdioReadFile.ReadString(strInputString);
	if( -1 != strInputString.Find(_T("DEVICEADDRESS = ")))
	{
		for(int nAddrCount = 0; nAddrCount < strInputString.GetLength(); nAddrCount++)
		{
			cDeviceAddr[nAddrCount] = (char)strInputString[nAddrCount];
		}
		sscanf(cDeviceAddr, "DEVICEADDRESS = %x", &nDeviceAddr);
	}
	m_stdioReadFile.ReadString(strInputString);
	if( -1 != strInputString.Find(_T("DEVICEID = ")))
	{
		for(int nAddrCount = 0; nAddrCount < strInputString.GetLength(); nAddrCount++)
		{
			cDeviceAddr[nAddrCount] = (char)strInputString[nAddrCount];
		}
		sscanf(cDeviceAddr, "DEVICEID = %s", &cDeviceID);
	}

	if(nDeviceAddr >= 0 && nDeviceAddr <= 0xFF)
	{
		WCHAR szFilters[]= _T("bin Files (*.bin)|*.bin|All Files (*.*)|*.*||");
		CFileDialog fileDlg(TRUE, _T("bin"), _T("*.bin"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilters, NULL);
		CString strFilePathName = _T("");
		CString strWriteFilePath = GetWorkDir();
		if( fileDlg.DoModal ()==IDOK )
		{
			strFilePathName = fileDlg.GetPathName();
			strWriteFilePath = strWriteFilePath + _T("\\") + fileDlg.GetFileTitle() + _T("ConvertG$WCmdShell.txt");
			CFile WriteFile(strWriteFilePath, CFile::modeCreate|CFile::modeWrite);
			CFile ReadFile(strFilePathName, CFile::modeRead | CFile::typeBinary);
			int nIndex = 0;
			ULONGLONG nFileLength = ReadFile.GetLength();
			char cTempBuffer[64] = {0};
			char cTempAddrBuffer[10] = {0};
			char cTempConvertBuffer[10] = {0};
			char cTempGWCmdBuffer[30] = {0};
			int nTS_DATA_MAX = 64;
			unsigned char cCheckSum = 0;
			int nDataCount = 0;

			WriteFile.Write("SHELLMODE = EEPROM WRITE\r\n", 26);
			WriteFile.Write("CMDMODE = MIS\r\n", 15);

			for(nIndex = 0; nIndex < nFileLength; nIndex += nTS_DATA_MAX)
			{
				ReadFile.Read(&cTempBuffer[0], nTS_DATA_MAX);	
				sprintf( &cTempAddrBuffer[0], "%04X", nIndex);
				sprintf(cTempGWCmdBuffer, "G$W:%c%c%02X%02X%s", cDeviceID[0], cDeviceID[1], nTS_DATA_MAX, nDeviceAddr, cTempAddrBuffer);
				WriteFile.Write(cTempGWCmdBuffer, 14);
				cCheckSum = 0;
				for(nDataCount = 0; nDataCount < 10; nDataCount++)
				{
					cCheckSum += cTempGWCmdBuffer[nDataCount + 4];
				}
				for(nDataCount = 0; nDataCount < nTS_DATA_MAX; nDataCount++)
				{
					sprintf( &cTempConvertBuffer[0], "%02X", cTempBuffer[nDataCount]);
					WriteFile.Write(cTempConvertBuffer, 2);
					cCheckSum += cTempConvertBuffer[0];
					cCheckSum += cTempConvertBuffer[1];
				}
				sprintf( &cTempConvertBuffer[0], "%02X", cCheckSum);
				WriteFile.Write(cTempConvertBuffer, 2);
				WriteFile.Write("\r\n", 2);
			}
	#if	1
			int nLastData = nFileLength % nTS_DATA_MAX;
			if( nLastData != 0)
			{
				sprintf( &cTempAddrBuffer[0], "%04X", nFileLength - nLastData);
				sprintf(cTempGWCmdBuffer, "G$W:%C%C%02X%02X%s", cDeviceID[0], cDeviceID[1], nLastData, nDeviceAddr, cTempAddrBuffer);
				WriteFile.Write(cTempGWCmdBuffer, 14);
				cCheckSum = 0;
				for(nDataCount = 0; nDataCount < 10; nDataCount++)
				{
					cCheckSum += cTempGWCmdBuffer[nDataCount + 4];
				}
				ReadFile.Read(&cTempBuffer[0], nLastData);
				for(nIndex = 0; nIndex < nLastData; nIndex++)
				{
					sprintf( &cTempConvertBuffer[0], "%02X", cTempBuffer[nIndex]);
					WriteFile.Write(cTempConvertBuffer, 2);
					cCheckSum += cTempConvertBuffer[0];
					cCheckSum += cTempConvertBuffer[1];
				}
				sprintf( &cTempConvertBuffer[0], "%02X", cCheckSum);
				WriteFile.Write(cTempConvertBuffer, 2);
				WriteFile.Write("\r\n", 2);
			}
	#endif 
			ReadFile.Close();
			WriteFile.Close();

			// Warning message
			CString strWarnMsg = _T("已经转换完成 \r\n路径为：");
			strWarnMsg += strWriteFilePath;
			MessageBox(strWarnMsg,_T("BIN文件转换完成"), MB_OK );
		}
	}
	else
	{
		MessageBox(_T("脚本错误"), _T("BIN文件转换失败"), MB_OK );
	}
}

void CLSCToolDlg::ShellEepromWrite(void)
{
	CString strData = _T("");
	int nIndex = 0;
	char cTempBuffer[64] = {0};
	char cTempAddrBuffer[10] = {0};
	char cTempConvertBuffer[10] = {0};
	BOOL bEndFile = m_stdioReadFile.ReadString(strData);
	m_bComSyncStatus = true;

	// Escape the # line
	while(1)
	{
		if(strData == _T("CMDMODE = SANYO"))
		{
			m_nCmdMode = 0;
			UpdateData(FALSE);
		}
		else if(strData == _T("CMDMODE = MIS"))
		{
			m_nCmdMode = 1;
			UpdateData(FALSE);
		}
		else if(strData == _T("CMDMODE = BIN"))
		{
			m_nCmdMode = 2;
			UpdateData(FALSE);
		}
		else if(strData == _T("CMDMODE = TEXT"))
		{
			m_nCmdMode = 3;
			UpdateData(FALSE);
		}
		else if(strData[0] != '#')
		{
			break;
		}
		bEndFile = m_stdioReadFile.ReadString(strData);
		if(bEndFile == false)
		{
			break;
		}
	}

	if(bEndFile == false)
	{
	//	m_bComSyncStatus = false;
	//	KillTimer(ID_SEND_EEPROMDATA_TIME);
	//	m_stdioReadFile.Close();
	//	MessageBox(_T("已经写入完毕"),_T("EEPROM写入"), MB_OK );
		ShellStop();
		m_ReadMsgDialog.SetQuitStatus(TRUE);
		m_ReadMsgDialog.UpdateProgressFull();
		m_ReadMsgDialog.SetWarnMsg(_T("已经写入完毕"));
		m_ReadMsgDialog.ShowWarnMsg();
	}
	else if(strData != _T(""))
	{
		ComSendString(strData);
		if(m_bShowSendString)
		{
			if(m_bShowSystemTime)
			{
				CString strSysteTime = _T("");
				strSysteTime = GetSystemTime();
				m_strCOMLog = m_strCOMLog + LOG_SEND_STRING + strSysteTime + strData + _T("\r\n");
			}
			else
			{
				m_strCOMLog = m_strCOMLog + LOG_SEND_STRING + strData + _T("\r\n");
			}
		}
			
		UpdateData(false);
		SetTimer(ID_SEND_EEPROMDATA_TIME, 2000, NULL);
	}
}

void CLSCToolDlg::ShellEepromRead(void)
{
	CString strData = _T("");
	int nIndex = 0;
	char cTempBuffer[64] = {0};
	char cTempAddrBuffer[10] = {0};
	char cTempConvertBuffer[10] = {0};
	BOOL bEndFile = m_stdioReadFile.ReadString(strData);
	m_bComSyncStatus = true;

	// Escape the # line
	while(1)
	{
		if(strData == _T("CMDMODE = SANYO"))
		{
			m_nCmdMode = 0;
			UpdateData(FALSE);
		}
		else if(strData == _T("CMDMODE = MIS"))
		{
			m_nCmdMode = 1;
			UpdateData(FALSE);
		}
		else if(strData == _T("CMDMODE = BIN"))
		{
			m_nCmdMode = 2;
			UpdateData(FALSE);
		}
		else if(strData == _T("CMDMODE = TEXT"))
		{
			m_nCmdMode = 3;
			UpdateData(FALSE);
		}
		else if(strData[0] != '#')
		{
			break;
		}
		
		bEndFile = m_stdioReadFile.ReadString(strData);
		if(bEndFile == false)
		{
			break;
		}
	}

	if(bEndFile == false)
	{
		ShellStop();
		m_ReadMsgDialog.SetQuitStatus(TRUE);
		m_ReadMsgDialog.UpdateProgressFull();
		m_ReadMsgDialog.SetWarnMsg(_T("已经读取完毕"));
		m_ReadMsgDialog.ShowWarnMsg();
	//	MessageBox(_T("已经读取完毕"),_T("EEPROM读取"), MB_OK );
	}
	else if(strData != _T(""))
	{
		ComSendString(strData);
		if(m_bShowSendString)
		{
			if(m_bShowSystemTime)
			{
				CString strSysteTime = _T("");
				strSysteTime = GetSystemTime();
				m_strCOMLog = m_strCOMLog + LOG_SEND_STRING + strSysteTime + strData + _T("\r\n");
			}
			else
			{
				m_strCOMLog = m_strCOMLog + LOG_SEND_STRING + strData + _T("\r\n");
			}
		}
			
		UpdateData(false);
		SetTimer(ID_SEND_EEPROMDATA_TIME, 4000, NULL);
	}
}

void CLSCToolDlg::ShellEepromDamage(void)
{
//	static eDAMAGE_MODE m_eDamageState = edmINIT;
	m_bComSyncStatus = TRUE;
	static int sm_nLogTime = -1;
	static int sm_nCycleTime = -1;
	static int sm_nCurrentTime = -1;
	static int sm_nStartAddr = -1;
	static int sm_nCurrentAddr = -1;
	static int sm_nEndAddr = -1;
	static unsigned char sm_cTestValue = 0x00;
	static unsigned char sm_cTestValueRead = 0x00;
	static int sm_nReadAddr = -1;
	static int sm_nReadCount = -1;
	static int sm_nErrCount = 0;
	static CString sm_strSaveFile = _T("");
	char cInputString[64] = {0};
	CString strInputString = _T("");
	CString strSendString = _T("");
	int nIndex = 0;

	switch(m_eDamageState)
	{
		case edmINIT:
			// Set CMD mode to SANYO Mode 
			m_nCmdMode = 0;
			UpdateData(FALSE);

			// Get Start Address
			m_stdioReadFile.ReadString(strInputString);
			memset(&cInputString[0], 0x00, sizeof(cInputString));
			GetStringToBuffer(strInputString, &cInputString[0]);
			sscanf(cInputString, "START = %04X", &sm_nStartAddr);

			// Get End Address
			m_stdioReadFile.ReadString(strInputString);
			memset(&cInputString[0], 0x00, sizeof(cInputString));
			GetStringToBuffer(strInputString, &cInputString[0]);
			sscanf(cInputString, "END = %04X", &sm_nEndAddr);

			// Get Cycle Time
			m_stdioReadFile.ReadString(strInputString);
			memset(&cInputString[0], 0x00, sizeof(cInputString));
			GetStringToBuffer(strInputString, &cInputString[0]);
			sscanf(cInputString, "TIME = %d", &sm_nCycleTime);

			// Get Log Time 
			m_stdioReadFile.ReadString(strInputString);
			memset(&cInputString[0], 0x00, sizeof(cInputString));
			GetStringToBuffer(strInputString, &cInputString[0]);
			sscanf(cInputString, "LOGTIME = %d", &sm_nLogTime);
			
			// Save File 
			sm_strSaveFile = _T("");
			m_stdioReadFile.ReadString(strInputString);
			memset(&cInputString[0], 0x00, sizeof(cInputString));
			GetStringToBuffer(strInputString, &cInputString[0]);
			for(nIndex = 11; nIndex < strInputString.GetLength(); nIndex++)
			{
				WCHAR wTempData = cInputString[nIndex];
				sm_strSaveFile += wTempData;
			}
			if(sm_strSaveFile != _T(""))
			{
				WCHAR wcUnicode = 0xfeff;
				CString strTime = GetSystemTime();
				CFile SaveFile(sm_strSaveFile, CFile::modeCreate|CFile::modeWrite);
				SaveFile.Write(&wcUnicode, 2);
				strTime += _T("EEPROM破坏实验开始\r\n");
				SaveFile.Write(strTime, 2*strTime.GetLength());
				SaveFile.Close();
			}

		//	m_stdioReadFile.Close();
			m_eDamageState = edmSENDSTARTCMD;
			sm_cTestValueRead = sm_cTestValue = 0;
			sm_nReadAddr = 0;
			sm_nReadCount = 0;
			sm_nCurrentTime = 0;
			sm_nErrCount = 0;
			
			if(sm_nLogTime < 10)
			{
				sm_nLogTime = 10;
			}

			// Initial the progress max time
			m_nShellMaxLine = sm_nCycleTime;

			// do not break here.
		case edmSENDSTARTCMD:
			if(sm_nStartAddr >= 0
				&& sm_nEndAddr > 0
				&& sm_nEndAddr > sm_nStartAddr)
			{
				m_eDamageState = edmCHECKSTARTCMD;
				ComSendString(_T("TS NVMODEON"));
				sm_nCurrentAddr = sm_nStartAddr;
				SetTimer(ID_EEPROM_DAMAGE_TIME, 4000, NULL);
			}
			else
			{
				MessageBox(_T("脚本错误"), _T("BIN文件转换失败"), MB_OK );
			}
			break;

		case edmSENDTSCMD:
			strSendString = _T("");
			if((sm_nCurrentAddr + 64) <= sm_nEndAddr)
			{
				CString strTemp = _T("");
				sm_nReadAddr = sm_nCurrentAddr;
				sm_nReadCount = 64;
				strSendString.AppendFormat(_T("TS NVDATA %04X %02d : "), sm_nCurrentAddr, sm_nReadCount);
				for(int nDataCount = 0; nDataCount < sm_nReadCount; nDataCount++)
				{
					strTemp = _T("");
					strTemp.AppendFormat(_T("%02X"), sm_cTestValue);
					strSendString += strTemp;
					if(nDataCount < sm_nReadCount -1)
					{
						strSendString += _T(" ");
					}
				}
				sm_nCurrentAddr += 64;
				sm_cTestValueRead = sm_cTestValue;
			}
			else if( (sm_nEndAddr - sm_nCurrentAddr) > 0)
			{
				CString strTemp = _T("");
				sm_nReadAddr = sm_nCurrentAddr;
				sm_nReadCount = sm_nEndAddr - sm_nCurrentAddr;
				strSendString.AppendFormat(_T("TS NVDATA %04X %02d : "), sm_nCurrentAddr, sm_nReadCount);
				for(int nDataCount = 0; nDataCount < sm_nReadCount; nDataCount++)
				{
					strTemp = _T("");
					strTemp.AppendFormat(_T("%02X"), sm_cTestValue);
					strSendString += strTemp;
					if(nDataCount < sm_nReadCount - 1)
					{
						strSendString += _T(" ");
					}
				}
				sm_nCurrentAddr = sm_nEndAddr;
				sm_cTestValueRead = sm_cTestValue;
			}
			
			// roll back 
			if(sm_nCurrentAddr >= sm_nEndAddr)
			{
				sm_nCurrentAddr = sm_nStartAddr;
				sm_nCurrentTime++;
				sm_cTestValue++;
				
				// Update the progress menu
				m_ReadMsgDialog.UpdateProgress();
			}
			
			
			if( (sm_nCurrentTime > 0) && (sm_nCurrentTime % sm_nLogTime) == 0)
			{
				// Each 1000 time 
				CString strLog = _T("");
				CFile SaveFile(sm_strSaveFile, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate);
				SaveFile.SeekToEnd();
				strLog.AppendFormat(_T("已经完成次数%d \r\n"), sm_nCurrentTime);
				SaveFile.Write(strLog, 2*strLog.GetLength());
				SaveFile.Close();

				m_eDamageState = edmCHECKTSCMD;
				ComSendString(strSendString);

				// Set the check timer and display warning message
				SetTimer(ID_EEPROM_DAMAGE_TIME, 4000, NULL);
				CString strWarnMsg = _T("正在疯狂的破坏中，如果需要停止，请点击退出!\r\n");
				strWarnMsg += strLog;
				strLog = _T("");
				strLog.AppendFormat(_T("异常次数为%d \r\n"), sm_nErrCount);
				strWarnMsg += strLog;
				m_ReadMsgDialog.SetWarnMsg(strWarnMsg);
				m_ReadMsgDialog.ShowWarnMsg();
			}
			else if(sm_nCurrentTime > sm_nCycleTime)
			{
				CString strLog = _T("");
				CFile SaveFile(sm_strSaveFile, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate);
				SaveFile.SeekToEnd();
				strLog.AppendFormat(_T("指定次数%d已经完成 \r\n"), sm_nCycleTime);
				SaveFile.Write(strLog, 2*strLog.GetLength());
				SaveFile.Close();
				m_eDamageState = edmINVALID;
				ShellStop();

				// Update the progress menu
				CString strWarnMsg = _T("");
				strWarnMsg.AppendFormat(_T("已经完成%d次测试\r\n"), sm_nCycleTime);
				strLog = _T("");
				strLog.AppendFormat(_T("异常次数为%d \r\n"), sm_nErrCount);
				strWarnMsg += strLog;
				m_ReadMsgDialog.SetQuitStatus(TRUE);
				m_ReadMsgDialog.SetWarnMsg(strWarnMsg);
				m_ReadMsgDialog.ShowWarnMsg();
			}
			else
			{
				m_eDamageState = edmCHECKTSCMD;
				ComSendString(strSendString);
				SetTimer(ID_EEPROM_DAMAGE_TIME, 4000, NULL);
			}
			break;

		case edmSENDTRCMD:
			strSendString.AppendFormat(_T("TR NVDATA %04X %02d"), sm_nReadAddr, sm_nReadCount);
			m_eDamageState = edmCHECKTRCMD;
			ComSendString(strSendString);
			SetTimer(ID_EEPROM_DAMAGE_TIME, 8000, NULL);
			break;

		case edmCHECKSTARTCMD:
			m_eDamageState = edmSENDTSCMD;
			break;

		case edmCHECKTSCMD:
			m_eDamageState = edmSENDTRCMD;
			break;

		case edmCHECKTRCMD:
			m_eDamageState = edmSENDTSCMD;
			// don't break here
		case edmCHECKDATA:
			{
				CString strTemp = _T("");
				CString strCheckString = _T("");
				strCheckString.AppendFormat(_T("TS NVDATA %04X %02d : "), sm_nReadAddr, sm_nReadCount);
				for(int nDataCount = 0; nDataCount < sm_nReadCount; nDataCount++)
				{
					strTemp = _T("");
					strTemp.AppendFormat(_T("%02X"), sm_cTestValueRead);
					strCheckString += strTemp;
					if(nDataCount < sm_nReadCount - 1)
					{
						strCheckString += _T(" ");
					}
				}

				if( m_strCOMLog.Find(strCheckString, 0) == -1 )
				{
					sm_nErrCount++;
					CString strLog = _T("");
					CFile SaveFile(sm_strSaveFile, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);
					SaveFile.SeekToEnd();
					strLog = _T("============= START EEPROM ERROR INFORMATION =============\r\n");
					SaveFile.Write(strLog, 2*strLog.GetLength());
					strLog = _T("");
					strLog.AppendFormat(_T("当前已经测试次数为：%d\r\n"), sm_nCurrentTime);
					SaveFile.Write(strLog, 2*strLog.GetLength());
					strLog = _T("");
					strLog.AppendFormat(_T("当前已经异常次数为：%d\r\n"), sm_nErrCount);
					SaveFile.Write(strLog, 2*strLog.GetLength());
					strLog = _T("当前检测字符串为：") + strCheckString + _T("\r\n");
					SaveFile.Write(strLog, 2*strLog.GetLength());
					strLog = _T("保存的记录为：\r\n");
					SaveFile.Write(strLog, 2*strLog.GetLength());
					SaveFile.Write(m_strCOMLog, 2*m_strCOMLog.GetLength());
					strLog = _T("============= END EEPROM ERROR  INFORMATION =============\r\n");
					SaveFile.Write(strLog, 2*strLog.GetLength());
					SaveFile.Close();
				}
				else
				{
					if(sm_cTestValueRead == 0xff)
					{
						m_strCOMLog = _T("");
						SetComLogDisplayBottom();
					}
				}
			}
			break;

		case edmCHECKEEROR:
			{
				ShellStop();
				m_ReadMsgDialog.SetQuitStatus(TRUE);
				m_ReadMsgDialog.SetWarnMsg(_T("通信异常! \r\n请确认串口连接 \r\n"));
				m_ReadMsgDialog.ShowWarnMsg();
				m_eDamageState = edmINVALID;

				CString strLog = _T("");
				CFile SaveFile(sm_strSaveFile, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);
				SaveFile.SeekToEnd();
				strLog = _T("============= START COM EEROR INFORMATION =============\r\n");
				SaveFile.Write(strLog, 2*strLog.GetLength());
				strLog = _T("");
				strLog.AppendFormat(_T("当前已经测试次数为：%d\r\n"), sm_nCurrentTime);
				SaveFile.Write(strLog, 2*strLog.GetLength());
				strLog = _T("");
				strLog.AppendFormat(_T("当前已经异常次数为：%d\r\n"), sm_nErrCount);
				SaveFile.Write(strLog, 2*strLog.GetLength());
				strLog = _T("保存的记录为：\r\n");
				SaveFile.Write(strLog, 2*strLog.GetLength());
				SaveFile.Write(m_strCOMLog, 2*m_strCOMLog.GetLength());
				strLog = _T("============= END COM EEROR INFORMATION =============\r\n");
				SaveFile.Write(strLog, 2*strLog.GetLength());
				SaveFile.Close();
			}
			break;

		case edmCHECKREPEAT:

			break;

		default:
			break;
	}


		// Warning message
	//	CString strWarnMsg = _T("已经转换完成 \r\n路径为：");
	//	strWarnMsg += strWriteFilePath;
	//	MessageBox(strWarnMsg,_T("BIN文件转换完成"), MB_OK );
}

eRESULT CLSCToolDlg::ShellFileCheck(void)
{
	//#define SHELL_EEPROM_BIN_TO_TSCMD 
	//#define SHELL_EEPROM_WRITE 
	//#define SHELL_EEPROM_READ _T("")
	//#define SHELL_EEPROM_DAMAGE _T("")
	const WCHAR * sm_cShellSupportMode[] = 
	{
		_T("SHELLMODE = EEPROM BIN TO TS CMD"),
		_T("SHELLMODE = EEPROM BIN TO G$W CMD"),
		_T("SHELLMODE = EEPROM WRITE"),
		_T("SHELLMODE = EEPROM READ"),
		_T("SHELLMODE = EEPROM DAMAGE"),
		_T("SHELLMODE = EEPROM TR CMD GENERATE"),
		_T("SHELLMODE = EEPROM G$R CMD GENERATE"),
		_T("SHELLMODE = SEND MODE"),
	};
	int nIndex = 0;
	eRESULT eResult = erSUCCESS;
	CString strDataLine = _T("");
	BOOL bShellModeStatus = FALSE;
	BOOL bValidString = TRUE;
	while(bValidString)
	{
		bValidString = m_stdioReadFile.ReadString(strDataLine);
		if(strDataLine[0] != '#')
		{
			for(nIndex = 0; nIndex < sizeof(sm_cShellSupportMode)/sizeof(sm_cShellSupportMode[0]); nIndex++)
			{
				if(strDataLine == sm_cShellSupportMode[nIndex])
				{
					m_nShellMode = nIndex;
					bShellModeStatus = TRUE; 
					break;
				}
			}
		}
		if(bShellModeStatus)
		{
			break;
		}
	}

	if(bShellModeStatus == FALSE || bValidString == FALSE)
	{
		eResult = erERROR;
	}
	return eResult;
}

BOOL CLSCToolDlg::ShellCommunicationCheck(void)
{
	BOOL bCmdCheckStatus = FALSE;
	int nLengthCOMLog = m_strCOMLog.GetLength();
	
	switch(m_nShellMode)
	{
		case esmEEPROMWRITE:
			if(ecmdmPANAMIS == m_nCmdMode)
			{
				if(nLengthCOMLog >= 8)
				{
					if(m_strCOMLog[nLengthCOMLog -1] == '\n'
						&& m_strCOMLog[nLengthCOMLog -2] == '\r'
						&& m_strCOMLog[nLengthCOMLog -3] == 0x03
						&& m_strCOMLog[nLengthCOMLog -4] == '1'
						&& m_strCOMLog[nLengthCOMLog -5] == '0'
						&& m_strCOMLog[nLengthCOMLog -6] == '4'
						&& m_strCOMLog[nLengthCOMLog -7] == 'R'
						&& m_strCOMLog[nLengthCOMLog -8] == 'E')
					{
						bCmdCheckStatus = FALSE;
					}
					else if(m_strCOMLog[nLengthCOMLog -1] == '\n'
						&& m_strCOMLog[nLengthCOMLog -2] == '\r'
						&& m_strCOMLog[nLengthCOMLog -3] == 0x03
						&& m_strCOMLog[nLengthCOMLog -4] == '2'
						&& m_strCOMLog[nLengthCOMLog -5] == '0'
						&& m_strCOMLog[nLengthCOMLog -6] == '4'
						&& m_strCOMLog[nLengthCOMLog -7] == 'R'
						&& m_strCOMLog[nLengthCOMLog -8] == 'E')
					{
						bCmdCheckStatus = FALSE;
					}
					else if(m_strCOMLog[nLengthCOMLog -1] == '\n'
						&& m_strCOMLog[nLengthCOMLog -2] == '\r')
					{
						bCmdCheckStatus = TRUE;
					}
				}
				else if(nLengthCOMLog >= 2)
				{
					if(m_strCOMLog[nLengthCOMLog -1] == '\n'
						&& m_strCOMLog[nLengthCOMLog -2] == '\r')
					{
						bCmdCheckStatus = TRUE;
					}
				}
			}
			else if(ecmdmSANYO == m_nCmdMode)
			{
				if(nLengthCOMLog >= 5)
				{
					if(m_strCOMLog[nLengthCOMLog -1] == '\n'
						&& m_strCOMLog[nLengthCOMLog -2] == '\r'
						&& m_strCOMLog[nLengthCOMLog -3] == '0'
						&& m_strCOMLog[nLengthCOMLog -4] == '0'
						&& m_strCOMLog[nLengthCOMLog -5] == '0')
					{
						bCmdCheckStatus = TRUE;
					}
				}
			}
			break;

		
		case esmEEPROMREAD:
			if(nLengthCOMLog >= 2)
			{
				if(m_strCOMLog[nLengthCOMLog -1] == '\n'
					&& m_strCOMLog[nLengthCOMLog -2] == '\r')
				{
					bCmdCheckStatus = TRUE;
				}
			}
			break;

		case esmEEPROMDAMAGE:
			if(m_eDamageState == edmCHECKTRCMD)
			{
				if(nLengthCOMLog >= 2)
				{
					if(m_strCOMLog[nLengthCOMLog -1] == '\n'
						&& m_strCOMLog[nLengthCOMLog -2] == '\r')
					{
						bCmdCheckStatus = TRUE;
					}
				}
			}
			else if(m_eDamageState == edmCHECKSTARTCMD || m_eDamageState == edmCHECKTSCMD)
			{
				if(nLengthCOMLog >= 5)
				{
					if(m_strCOMLog[nLengthCOMLog -1] == '\n'
						&& m_strCOMLog[nLengthCOMLog -2] == '\r'
						&& m_strCOMLog[nLengthCOMLog -3] == '0'
						&& m_strCOMLog[nLengthCOMLog -4] == '0'
						&& m_strCOMLog[nLengthCOMLog -5] == '0')
					{
						bCmdCheckStatus = TRUE;
					}
				}
			}
			if(bCmdCheckStatus)
			{
				ShellEepromDamage();
			}
			break;

		default:
			break;
	}
	return bCmdCheckStatus;
}

void CLSCToolDlg::ShellTRCommandGenerate(void)
{
	int nLineNumber = -1;
	int nStartAddr = -1;
	int nEndAddr = -1;
	char cDeviceID[3] = {0};
	int nDeviceAddr = -1;
	char cInputString[32] = {0};
	CString strInputString = _T("");

	// Get Start Address
	m_stdioReadFile.ReadString(strInputString);
	memset(&cInputString[0], 0x00, sizeof(cInputString));
	GetStringToBuffer(strInputString, &cInputString[0]);
	sscanf(cInputString, "START = %04X", &nStartAddr);

	// Get End Address
	m_stdioReadFile.ReadString(strInputString);
	memset(&cInputString[0], 0x00, sizeof(cInputString));
	GetStringToBuffer(strInputString, &cInputString[0]);
	sscanf(cInputString, "END = %04X", &nEndAddr);

	// Get Line Number 
	m_stdioReadFile.ReadString(strInputString);
	memset(&cInputString[0], 0x00, sizeof(cInputString));
	GetStringToBuffer(strInputString, &cInputString[0]);
	sscanf(cInputString, "LINENUMBER = %02X", &nLineNumber);
	
	if( nStartAddr >= 0
		&& nEndAddr > 0
		&& nEndAddr > nStartAddr)
	{
		CString strWriteFilePath = GetWorkDir();
		strWriteFilePath = strWriteFilePath + _T("\\") + _T("成功生成TR脚本.txt");
		CFile WriteFile(strWriteFilePath, CFile::modeCreate|CFile::modeWrite);
		int nIndex = 0;
		char cTempBuffer[64] = {0};
		char cTempAddrBuffer[10] = {0};
		char cTempConvertBuffer[10] = {0};
		char cTempTRCmdBuffer[30] = {0};
		int nTS_DATA_MAX = nLineNumber;
		int nDataCount = 0;

		WriteFile.Write("SHELLMODE = EEPROM READ\r\n", 25);
		WriteFile.Write("CMDMODE = SANYO\r\n", 17);
		WriteFile.Write("TS NVMODEON\r\n", 13);
		
		for(nIndex = nStartAddr; nIndex < nEndAddr; nIndex += nTS_DATA_MAX)
		{
			sprintf( &cTempAddrBuffer[0], "%04X", nIndex);
			sprintf(cTempTRCmdBuffer, "TR NVDATA %s %02d", cTempAddrBuffer, nTS_DATA_MAX);
			WriteFile.Write(cTempTRCmdBuffer, 17);
			WriteFile.Write("\r\n", 2);
		}

		int nLastData = nEndAddr % nTS_DATA_MAX;
		if( nLastData != 0)
		{
			sprintf( &cTempAddrBuffer[0], "%04X", nEndAddr - nLastData);
			sprintf(cTempTRCmdBuffer, "TR NVDATA %s %02d", cTempAddrBuffer, nLastData);
			WriteFile.Write(cTempTRCmdBuffer, 17);
			WriteFile.Write("\r\n", 2);
		}

		WriteFile.Close();

		// Warning message
		CString strWarnMsg = _T("已经转换完成 \r\n路径为：");
		strWarnMsg += strWriteFilePath;
		MessageBox(strWarnMsg,_T("BIN文件转换完成"), MB_OK );
	}
	else
	{
		MessageBox(_T("脚本错误"), _T("BIN文件转换失败"), MB_OK );
	}
}

void CLSCToolDlg::ShellGRCommandGenerate(void)
{
	int nLineNumber = -1;
	int nStartAddr = -1;
	int nEndAddr = -1;
	char cDeviceID[3] = {0};
	int nDeviceAddr = -1;
	char cInputString[32] = {0};
	CString strInputString = _T("");
	
	// Get Device Address
	m_stdioReadFile.ReadString(strInputString);
	memset(&cInputString[0], 0x00, sizeof(cInputString));
	GetStringToBuffer(strInputString, &cInputString[0]);
	sscanf(cInputString, "DEVICEADDRESS = %x", &nDeviceAddr);

	// Get DeviceID
	m_stdioReadFile.ReadString(strInputString);
	memset(&cInputString[0], 0x00, sizeof(cInputString));
	GetStringToBuffer(strInputString, &cInputString[0]);
	sscanf(cInputString, "DEVICEID = %s", &cDeviceID[0]);

	// Get Start Address
	m_stdioReadFile.ReadString(strInputString);
	memset(&cInputString[0], 0x00, sizeof(cInputString));
	GetStringToBuffer(strInputString, &cInputString[0]);
	sscanf(cInputString, "START = %04X", &nStartAddr);

	// Get End Address
	m_stdioReadFile.ReadString(strInputString);
	memset(&cInputString[0], 0x00, sizeof(cInputString));
	GetStringToBuffer(strInputString, &cInputString[0]);
	sscanf(cInputString, "END = %04X", &nEndAddr);

	// Get Line Number 
	m_stdioReadFile.ReadString(strInputString);
	memset(&cInputString[0], 0x00, sizeof(cInputString));
	GetStringToBuffer(strInputString, &cInputString[0]);
	sscanf(cInputString, "LINENUMBER = %02X", &nLineNumber);
	
	if(nDeviceAddr >= 0 && nDeviceAddr <= 0xFF 
		&& nStartAddr >= 0
		&& nEndAddr > 0
		&& nEndAddr > nStartAddr)
	{
		CString strWriteFilePath = GetWorkDir();
		strWriteFilePath = strWriteFilePath + _T("\\") + _T("成功生成G$R脚本.txt");
		CFile WriteFile(strWriteFilePath, CFile::modeCreate|CFile::modeWrite);
		int nIndex = 0;
		char cTempBuffer[64] = {0};
		char cTempAddrBuffer[10] = {0};
		char cTempConvertBuffer[10] = {0};
		char cTempGWCmdBuffer[30] = {0};
		int nTS_DATA_MAX = nLineNumber;
		unsigned char cCheckSum = 0;
		int nDataCount = 0;

		WriteFile.Write("SHELLMODE = EEPROM READ\r\n", 25);
		WriteFile.Write("CMDMODE = MIS\r\n", 15);

		for(nIndex = nStartAddr; nIndex < nEndAddr; nIndex += nTS_DATA_MAX)
		{
			sprintf( &cTempGWCmdBuffer[0], "G$R:%C%C%02X%02X", cDeviceID[0], cDeviceID[1], nTS_DATA_MAX, nDeviceAddr);
			sprintf( &cTempGWCmdBuffer[10], "%04X", nIndex);
			cCheckSum = 0;
			for(nDataCount=0; nDataCount< 10; nDataCount++)
			{
				cCheckSum += cTempGWCmdBuffer[nDataCount+4];
			}
			sprintf( &cTempGWCmdBuffer[14], "%02X", cCheckSum);
			WriteFile.Write(cTempGWCmdBuffer, 16);
			WriteFile.Write("\r\n", 2);
		}

		int nLastData = nEndAddr % nTS_DATA_MAX;
		if( nLastData != 0)
		{
			sprintf( &cTempAddrBuffer[0], "%04X", nEndAddr - nLastData);
			sprintf(cTempGWCmdBuffer, "G$R:%C%C%02X%02X%s", cDeviceID[0], cDeviceID[1], nLastData, nDeviceAddr, cTempAddrBuffer);
			WriteFile.Write(cTempGWCmdBuffer, 14);
			cCheckSum = 0;
			for(nDataCount = 0; nDataCount < 10; nDataCount++)
			{
				cCheckSum += cTempGWCmdBuffer[nDataCount + 4];
			}
			sprintf( &cTempConvertBuffer[0], "%02X", cCheckSum);
			WriteFile.Write(cTempConvertBuffer, 2);
			WriteFile.Write("\r\n", 2);
		}
 
		WriteFile.Close();

		// Warning message
		CString strWarnMsg = _T("已经转换完成 \r\n路径为：");
		strWarnMsg += strWriteFilePath;
		MessageBox(strWarnMsg,_T("BIN文件转换完成"), MB_OK );
	}
	else
	{
		MessageBox(_T("脚本错误"), _T("BIN文件转换失败"), MB_OK );
	}
}

void CLSCToolDlg::ShellStop(void)
{
	m_bComSyncStatus = false;
	m_bShellSendAlready = false;
	m_bShellSendWaitString = false;
	m_bShowReceiveString = true; // quit the Shell need to reset the status of Show Receive string Log 
	m_bShowBottom = false;
	KillTimer(ID_SEND_EEPROMDATA_TIME);
	KillTimer(ID_EEPROM_DAMAGE_TIME);
	KillTimer(ID_SEND_MODE_TIME);
	m_stdioReadFile.Close();
//	MessageBox(_T("已经读取完毕"),_T("EEPROM读取"), MB_OK );
}

int CLSCToolDlg::ShellGetMaxLine(void)
{
	int nLine = 0;
	BOOL bEndFile = FALSE;
	CString strRead = _T("");
	while(1)
	{
		nLine++;
		bEndFile = m_stdioReadFile.ReadString(strRead);
		if(bEndFile == FALSE)
		{
			break;
		}
	}
	m_stdioReadFile.Close();
	m_stdioReadFile.Open(m_strShellPathName, CFile::modeRead);
	return nLine;
}

enum
{
	essmTIME = 0x00,
	essmMODE,
	essmWAITRECEIVE,
	essmWAIT,
	essmIFMULT,
	essmPRINT,		// Priority need to set higher than IF/ESE/SWITCH/REPEAT command
	essmIFRECEIVE,
	essmELSERECIVE,
	essmIFRECEIVEEND,
	essmSWITCHRECEIVE,
	essmCASE,
	essmDEFAULT,
	essmSWITCHRECEIVEEND,
	essmLOG,
	essmSENDLOG,
	essmRECEIVELOG,
	essmTIMELOG,
	essmREPEATSTART,
	essmREPEATCOUNT,
	essmREPEATEND,
	essmBREAK,
	essmCALL,
	essmCALLSTART,
	essmCALLEND,
	essmJUMP,
	essmJUMPLABEL,
	essmSETCOM,
	essmSHOWBOTTOM,
	essmINVALID,
};
	
enum
{
	ebtSWITCH,
	ebtREPEAT,
	ebtINVALID
};

static const CString sm_strSupportCMDTable[]=
{
	_T("TIME = "),
	_T("CMDMODE = "),
	_T("WAIT = RECEIVE"),
	_T("WAIT = "),
	_T("MULTI-IF = "),
	_T("PRINT = "),
	_T("IF RECEIVE = "),
	_T("ELSE"),
	_T("ENDIF"),
	_T("SWITCH = RECEIVE"),
	_T("CASE = "),
	_T("DEFAULT"),
	_T("ENDSWITCH"),
	_T("SHOWCMDLOG = "),
	_T("SHOWSENDLOG = "),
	_T("SHOWRECEIVELOG = "),
	_T("SHOWTIMELOG = "),
	_T("REPEAT START"),
	_T("REPEAT COUNT = "),
	_T("REPEAT END"),
	_T("BREAK"),
	_T("CALL = "),
	_T("CALL START = "),
	_T("CALL END"),
	_T("JUMP = "),
	_T("JUMP LABEL = "),
	_T("SETCOM = "),
	_T("SHOWBOTTOM = "),
	_T("INVALID"),
};
static bool sm_bStopProgress = FALSE;
static bool sm_bIfStatus = FALSE;
static bool sm_bSwitchStatus = FALSE;
static bool sm_bIfMulti = TRUE;

void CLSCToolDlg::ShellSendMode_Time(CString strRead)
{
	// Get Send Time
	strRead = strRead.Mid(sm_strSupportCMDTable[essmTIME].GetLength());
	strRead = DeleteStringTabSpaceLeft(strRead);
	strRead = DeleteStringTabSpaceRight(strRead);
	m_nShellSendTime = _ttoi(strRead);
	SetTimer(ID_SEND_MODE_TIME, m_nShellSendTime, NULL);
				
	// Print the Warning message
	if(m_bShellSendModeLog)
	{
		CString strLogInfo = _T("");
		strLogInfo.AppendFormat(_T("设置时间间隔为%dms\r\n"), m_nShellSendTime);
		m_strCOMLog += strLogInfo;
		SetComLogDisplayBottom();
	}
}

void CLSCToolDlg::ShellSendMode_Mode(CString strRead)
{
	WCHAR * strCMDmode[]=
	{
		_T("SANYO"),
		_T("MIS"),
		_T("BIN"),
		_T("TEXT"),
	};
	strRead = strRead.Mid(sm_strSupportCMDTable[essmMODE].GetLength());
	strRead = DeleteStringTabSpaceLeft(strRead);
	strRead = DeleteStringTabSpaceRight(strRead);
	m_nShellSendMode = 3;

	int nIndex = 0;
	for(nIndex = 0; nIndex < sizeof(strCMDmode)/sizeof(strCMDmode[0]); nIndex++)
	{
		if(strRead == strCMDmode[nIndex])
		{
			m_nShellSendMode = nIndex;
			break;
		}
	}

	// Change the Mode
	//UpdateData(FALSE);
	m_nCmdMode = m_nShellSendMode;
	UpdateData(FALSE);
	SetTimer(ID_SEND_MODE_TIME, m_nShellSendTime, NULL);

	// Print the Warning message
	if(m_bShellSendModeLog)
	{
		m_strCOMLog = m_strCOMLog + _T("设置命令模式为") + strCMDmode[m_nShellSendMode] + _T("\r\n");
		SetComLogDisplayBottom();
	}
}

void CLSCToolDlg::ShellSendMode_Wait(CString strRead)
{
	int nWaitTime = 0;
	strRead = strRead.Mid(sm_strSupportCMDTable[essmWAIT].GetLength());
	strRead = DeleteStringTabSpaceLeft(strRead);
	strRead = DeleteStringTabSpaceRight(strRead);
	nWaitTime = _ttoi(strRead);
	SetTimer(ID_SEND_MODE_TIME, nWaitTime, NULL);

	// Print the Warning message
	if(m_bShellSendModeLog)
	{
		CString strLogInfo = _T("");
		strLogInfo.AppendFormat(_T("设置等待时间为%dms\r\n"), nWaitTime);
		m_strCOMLog += strLogInfo;
		SetComLogDisplayBottom();
	}	
}
void CLSCToolDlg::ShellSendMode_WaitReceive(CString strRead)
{
	if(m_bShellSendAlready == TRUE)
	{
		m_bShellSendAlready = FALSE;
		m_bShellSendWaitString = TRUE;
		// Print the Warning message
		if(m_bShellSendModeLog)
		{
			CString strLogInfo = _T("");
			strLogInfo.AppendFormat(_T("正在等待接收的到来\r\n"));
			m_strCOMLog += strLogInfo;
			SetComLogDisplayBottom();
		}
	}
	else
	{
		SetTimer(ID_SEND_MODE_TIME, 1, NULL);
		// Print the Warning message
		if(m_bShellSendModeLog)
		{
			CString strLogInfo = _T("");
			strLogInfo.AppendFormat(_T("已经接收到，不需要在等待\r\n"));
			m_strCOMLog += strLogInfo;
			SetComLogDisplayBottom();
		}
	}
}
void CLSCToolDlg::ShellSendMode_IfMulti(CString strRead)
{
	strRead = strRead.Mid(sm_strSupportCMDTable[essmIFMULT].GetLength());
	strRead = DeleteStringTabSpaceLeft(strRead);
	strRead = DeleteStringTabSpaceRight(strRead);
	if(strRead.Find(_T("TRUE")) != -1)
	{
		sm_bIfMulti = TRUE;
	}
	else
	{
		sm_bIfMulti = FALSE;
	}
	SetTimer(ID_SEND_MODE_TIME, m_nShellSendTime, NULL);
}

void CLSCToolDlg::ShellSendMode_IfReceive(CString strRead)
{
	int nStartPos = 0;
	int nEndPos = 0;
	CString strSubString = _T("");
	int nSubString = 0;
	int nIfPos = 0;
	int nCount = 0;
	int nIndex = 0;
	CString strDivide = _T("\t,，");  // Divide the string to sub-string

	strRead = strRead.Mid(sm_strSupportCMDTable[essmIFRECEIVE].GetLength());
	strRead = DeleteStringTabSpaceLeft(strRead);
	strRead = DeleteStringTabSpaceRight(strRead);


	// Delete the Send String Log Info 
	if(m_bShowSendString)
	{
		int nTempPos = m_strCurrentReciveString.Find(_T("|"));
		if( -1 !=  nTempPos)
		{
			m_strCurrentReciveString = m_strCurrentReciveString.Mid(nTempPos + 1);
			m_strCurrentReciveString = m_strCurrentReciveString.TrimLeft(_T(" "));
		}
	}
	// Delete the System Time Log info
	if(m_bShowSystemTime)
	{
		int nTempPos = m_strCurrentReciveString.Find(_T("|"));
		if( -1 !=  nTempPos)
		{
			m_strCurrentReciveString = m_strCurrentReciveString.Mid(nTempPos + 1);
			m_strCurrentReciveString = m_strCurrentReciveString.TrimLeft(_T(" "));
		}
	}

	while(1)
	{
		nCount++;
		strSubString = _T("");
						
		if(sm_bIfMulti)
		{
			for(nIndex = 0; nIndex < strDivide.GetLength(); nIndex++)
			{
				WCHAR wTemp = strDivide[nIndex];
				nEndPos = strRead.Find(wTemp);
				if(nEndPos != -1)
				{
					break;
				}
			}
		}
		else
		{
			// Don't need to use divide string
			nEndPos = -1;
		}

		if(nEndPos != -1)
		{
			strSubString = strRead.Mid(nStartPos, nEndPos);
			strRead = strRead.Mid(nEndPos, strRead.GetLength());
			strRead = strRead.TrimLeft(strDivide);
			strSubString = strSubString.Trim(strDivide);
			nIfPos = m_strCurrentReciveString.Find(strSubString);	
			nStartPos = nEndPos;

			if(nIfPos != -1)
			{
				break;
			}
		}
		else 
		{
			nIfPos = m_strCurrentReciveString.Find(strRead);
			break;
		}
						
		// Use for protect only
		if(nCount > 100)
		{
			break;
		}
	}// end while

	if(nIfPos == -1)
	{
		CString strFindEndIfString = _T("");
		while(1)
		{
			if(m_ReadMsgDialog.GetQuitStatus())
			{
				// End file
				break;
			}
			strFindEndIfString = ShellSendModeGetString();
			if(strFindEndIfString != _T(""))
			{
				if(strFindEndIfString.Find(_T("ENDIF")) != -1
					|| strFindEndIfString.Find(_T("ELSE")) != -1)
				{
					break;
				}
			}
		}
		sm_bIfStatus = FALSE;
	}
	else
	{
		sm_bIfStatus = TRUE;
	}
	if(m_ReadMsgDialog.GetQuitStatus() == FALSE)
	{
		SetTimer(ID_SEND_MODE_TIME, 1, NULL);
	}
}

void CLSCToolDlg::ShellSendMode_ElseReceive(CString strRead)
{
	if(sm_bIfStatus)
	{
		int nIfCount = 0;
		CString strFindEndIfString = _T("");
		while(1)
		{
			if(m_ReadMsgDialog.GetQuitStatus())
			{
				// End file
				break;
			}
			strFindEndIfString = ShellSendModeGetString();
			if(strFindEndIfString != _T(""))
			{
				if(strFindEndIfString.Find(_T("IF RECEIVE = ")) != -1)
				{
					nIfCount++; 
				}
				else if(strFindEndIfString.Find(_T("ENDIF")) != -1)
				{
					nIfCount--;
				}
			}
			if(nIfCount < 0)
			{
				break;
			}
		}
	}
	if(m_ReadMsgDialog.GetQuitStatus() == FALSE)
	{
		SetTimer(ID_SEND_MODE_TIME, 1, NULL);
	}
}
void CLSCToolDlg::ShellSendMode_SwitchReceive(CString strRead)
{
	int nStartPos = 0;
	int nEndPos = 0;
	CString strSubString = _T("");
	int nSubString = 0;
	int nIfPos = 0;
	int nCount = 0;
	int nIndex = 0;
	CString strDivide = _T("\t,，");  // Divide the string to sub-string

	// Add the switch data to the Stack
	m_pstLscStackBreak->DataPush(ebtSWITCH);

	// Delete the Send String Log Info 
	if(m_bShowSendString)
	{
		int nTempPos = m_strCurrentReciveString.Find(_T("|"));
		if( -1 !=  nTempPos)
		{
			m_strCurrentReciveString = m_strCurrentReciveString.Mid(nTempPos + 1);
			m_strCurrentReciveString = m_strCurrentReciveString.TrimLeft(_T(" "));
		}
	}
	// Delete the System Time Log info
	if(m_bShowSystemTime)
	{
		int nTempPos = m_strCurrentReciveString.Find(_T("|"));
		if( -1 !=  nTempPos)
		{
			m_strCurrentReciveString = m_strCurrentReciveString.Mid(nTempPos + 1);
			m_strCurrentReciveString = m_strCurrentReciveString.TrimLeft(_T(" "));
		}
	}
					
	// To find the CASE or DEFAULT line
	while(1)
	{
		BOOL bValidString = FALSE;
		if(m_ReadMsgDialog.GetQuitStatus())
		{
			// End file
			break;
		}
		strRead = ShellSendModeGetString();
		if(strRead != _T(""))
		{
			if(strRead.Find(_T("CASE =")) != -1)
			{
				bValidString = TRUE; 
				strRead = strRead.Mid(6);
				strRead = DeleteStringTabSpaceLeft(strRead);
				strRead = DeleteStringTabSpaceRight(strRead);
			}
			else if(strRead.Find(_T("DEFAULT")) != -1)
			{
				strRead = DeleteStringTabSpaceLeft(strRead);
				strRead = DeleteStringTabSpaceRight(strRead);
				if(strRead[0] == 'D')
				{
					bValidString = TRUE;
					break;
				}
			}
			else if(strRead.Find(_T("ENDSWITCH")) != -1)
			{
				// Delete data from Stack
				m_pstLscStackBreak->DataPop();
				break;
			}
		}
		// Check the Recieve string
		while(bValidString)
		{
			nCount++;
			strSubString = _T("");
			if(sm_bIfMulti)
			{
				for(nIndex = 0; nIndex < strDivide.GetLength(); nIndex++)
				{
					WCHAR wTemp = strDivide[nIndex];
					nEndPos = strRead.Find(wTemp);
					if(nEndPos != -1)
					{
						break;
					}
				}
			}
			else
			{
				// Don't need to use divide string
				nEndPos = -1;
			}

			if(nEndPos != -1)
			{
				strSubString = strRead.Mid(nStartPos, nEndPos);
				strRead = strRead.Mid(nEndPos, strRead.GetLength());
				strRead = strRead.TrimLeft(strDivide);
				strSubString = strSubString.Trim(strDivide);
				nIfPos = m_strCurrentReciveString.Find(strSubString);	
				nStartPos = nEndPos;

				if(nIfPos != -1)
				{
					break;
				}
			}
			else 
			{
				nIfPos = m_strCurrentReciveString.Find(strRead);
				break;
			}
							
			// Use for protect only
			if(nCount > 100)
			{
				break;
			}
		}// end while

		if(nIfPos != -1)
		{
			break;
		}
	}// end while
				
	if(m_ReadMsgDialog.GetQuitStatus() == FALSE)
	{
		SetTimer(ID_SEND_MODE_TIME, 1, NULL);
	}
}

void CLSCToolDlg::ShellSendMode_Log(CString strRead)
{
	// Get Log Information
	strRead = strRead.Mid(sm_strSupportCMDTable[essmLOG].GetLength());
	strRead = DeleteStringTabSpaceLeft(strRead);
	strRead = DeleteStringTabSpaceRight(strRead);
	if(strRead.Find(_T("TRUE")) != -1)
	{
		m_bShellSendModeLog = TRUE;
	}
	else
	{
		m_bShellSendModeLog = FALSE;
	}
	SetTimer(ID_SEND_MODE_TIME, m_nShellSendTime, NULL);
}

void CLSCToolDlg::ShellSendMode_SendLog(CString strRead)
{
	strRead = strRead.Mid(sm_strSupportCMDTable[essmSENDLOG].GetLength());
	strRead = DeleteStringTabSpaceLeft(strRead);
	strRead = DeleteStringTabSpaceRight(strRead);
	if(strRead.Find(_T("TRUE")) != -1)
	{
		m_bShowSendString = TRUE;
		CButton *pCheckButton = (CButton *)GetDlgItem(IDC_CHECK_SHOWSENDLOG);
		pCheckButton->SetCheck(m_bShowSendString);	
	}
	else
	{
		m_bShowSendString = FALSE;
		CButton *pCheckButton = (CButton *)GetDlgItem(IDC_CHECK_SHOWTIME);
		pCheckButton->SetCheck(m_bShowSendString);
	}
	SetTimer(ID_SEND_MODE_TIME, m_nShellSendTime, NULL);	
}
void CLSCToolDlg::ShellSendMode_ReceiveLog(CString strRead)
{
	strRead = strRead.Mid(sm_strSupportCMDTable[essmRECEIVELOG].GetLength());
	strRead = DeleteStringTabSpaceLeft(strRead);
	strRead = DeleteStringTabSpaceRight(strRead);
	if(strRead.Find(_T("FALSE")) != -1)
	{
		m_bShowReceiveString = false;
	}
	else
	{
		m_bShowReceiveString = true;
	}
	SetTimer(ID_SEND_MODE_TIME, m_nShellSendTime, NULL);	
}
void CLSCToolDlg::ShellSendMode_TimeLog(CString strRead)
{
	strRead = strRead.Mid(sm_strSupportCMDTable[essmTIMELOG].GetLength());
	strRead = DeleteStringTabSpaceLeft(strRead);
	strRead = DeleteStringTabSpaceRight(strRead);
	if(strRead.Find(_T("TRUE")) != -1)
	{
		m_bShowSystemTime = TRUE;
		CButton *pCheckButton = (CButton *)GetDlgItem(IDC_CHECK_SHOWTIME);
		pCheckButton->SetCheck(m_bShowSystemTime);	
	}
	else
	{
		m_bShowSystemTime = FALSE;
		CButton *pCheckButton = (CButton *)GetDlgItem(IDC_CHECK_SHOWTIME);
		pCheckButton->SetCheck(m_bShowSystemTime);
	}
	SetTimer(ID_SEND_MODE_TIME, m_nShellSendTime, NULL);
}
void CLSCToolDlg::ShellSendMode_Print(CString strRead)
{
	strRead = strRead.Mid(sm_strSupportCMDTable[essmPRINT].GetLength());
	strRead = DeleteStringTabSpaceLeft(strRead);
	strRead = DeleteStringTabSpaceRight(strRead);
				
	if(strRead.Find(_T("SYSTEMTIME")) != -1)
	{
		strRead = _T("| 当前时间为：") + GetSystemTime();
	}
	else if(m_bShowSystemTime)
	{
		strRead = GetSystemTime() + strRead;
	}
	if(strRead.Find(_T("COUNT")) != -1)
	{
		if(!m_pstLscStackRepeatCount->IsEmpty())
		{
			CString strCount = _T("");
			int nCount = m_pstLscStackRepeatCount->DataGet();
			strCount.AppendFormat(_T("%d"), nCount + 1);
			strRead.Replace(_T("COUNT"), strCount);
		}
	}
	m_strCOMLog += strRead + _T("\r\n");
	SetComLogDisplayBottom();
	SetTimer(ID_SEND_MODE_TIME, 1, NULL);
}
void CLSCToolDlg::ShellSendMode_RepeatStart(CString strRead)
{
	m_pstLscStackRepeatCount->DataPush(0);
	m_pstLscStackRepeatPos->DataPush(m_stdioReadFile.GetPosition());
					
	// Add the Break to Stack
	m_pstLscStackBreak->DataPush(ebtREPEAT);

	// Use for Protect, Over the MAX LOOP 
	if(m_pstLscStackRepeatCount->IsFull())
	{
		CString strLogInfo = _T("");
		strLogInfo.AppendFormat(_T("警告已经超出最大循环次数%d\r\n"),REPEAT_LOOP_COUNT);
		m_strCOMLog += strLogInfo;
		SetComLogDisplayBottom();
	}
	else if(m_bShellSendModeLog) // Print the Warning message
	{
		CString strLogInfo = _T("");
		strLogInfo.AppendFormat(_T("第%d层循环开始\r\n"),m_pstLscStackRepeatCount->NumberGet());
		m_strCOMLog += strLogInfo;
		SetComLogDisplayBottom();
	}

	SetTimer(ID_SEND_MODE_TIME, 1, NULL);
}
void CLSCToolDlg::ShellSendMode_RepeatCount(CString strRead)
{
	// Get Repeat Count
	int nCurrentRepeatIndex = 0;
	int nCurrentShellNum = 0;
	strRead = strRead.Mid(sm_strSupportCMDTable[essmREPEATCOUNT].GetLength());
	strRead = DeleteStringTabSpaceLeft(strRead);
	strRead = DeleteStringTabSpaceRight(strRead);
	nCurrentShellNum = _ttoi(strRead);
	nCurrentRepeatIndex = m_pstLscStackRepeatCount->NumberGet();
	// REPEAT COUTN run before the REPEAT START, so we need to +1 here. 1 means the first value
	m_pstLscStackShellRepeatCount->DataSet(nCurrentRepeatIndex + 1, nCurrentShellNum);
	SetTimer(ID_SEND_MODE_TIME, m_nShellSendTime, NULL);

	// Use for Protect, Over the MAX LOOP 
	if(nCurrentRepeatIndex > REPEAT_LOOP_COUNT)
	{
		CString strLogInfo = _T("");
		strLogInfo.AppendFormat(_T("警告已经超出最大循环次数%d\r\n"),REPEAT_LOOP_COUNT);
		m_strCOMLog += strLogInfo;
		SetComLogDisplayBottom();
	}
	else if(m_bShellSendModeLog) // print the Warning message
	{
		CString strLogInfo = _T("");
		strLogInfo.AppendFormat(_T("第%d层循环次数为%d次\r\n"), nCurrentRepeatIndex+1, nCurrentShellNum);
		m_strCOMLog += strLogInfo;
		SetComLogDisplayBottom();
	}
}
void CLSCToolDlg::ShellSendMode_RepeatEnd(CString strRead)
{
	if(!m_pstLscStackRepeatCount->IsEmpty() && !m_pstLscStackRepeatCount->IsFull())
	{
		CString strLogInfo = _T("");
		int nCurrentIndex = m_pstLscStackRepeatCount->NumberGet();
		int nCurrentCount = m_pstLscStackRepeatCount->DataPop();
		int nShellCount = m_pstLscStackShellRepeatCount->DataGet(nCurrentIndex);
		nCurrentCount++;
		m_pstLscStackRepeatCount->DataPush(nCurrentCount);
		if(nCurrentCount < nShellCount)
		{
			// print the Warning message
			if(m_bShellSendModeLog)
			{
				strLogInfo.AppendFormat(_T("第%d层循环第%d次\r\n"), nCurrentIndex, nCurrentCount+1); // From 1 start
				m_strCOMLog += strLogInfo;
				SetComLogDisplayBottom();
			}
			m_stdioReadFile.Seek(m_pstLscStackRepeatPos->DataGet(), 0);
		}
		else
		{
			// print the Warning message
			if(m_bShellSendModeLog)
			{
				strLogInfo.AppendFormat(_T("第%d层循环循环结束\r\n"), nCurrentIndex);
				m_strCOMLog += strLogInfo;
				SetComLogDisplayBottom();
			}

			// Delete the Repeat pos from Stack
			// Shell repeat count value return to Default value
			m_pstLscStackRepeatPos->DataPop();
			m_pstLscStackRepeatCount->DataPop();
			//m_pstLscStackShellRepeatCount->DataPop();
			m_pstLscStackShellRepeatCount->DataSet(nCurrentIndex, REPEAT_VALUE_DEFAULT);
			m_pstLscStackBreak->DataPop();
		}
		SetTimer(ID_SEND_MODE_TIME, 1, NULL);
	}
	else
	{
		SetTimer(ID_SEND_MODE_TIME, 1, NULL);
	}
}
void CLSCToolDlg::ShellSendMode_Break(CString strRead)
{
	if(!m_pstLscStackBreak->IsEmpty())
	{
		// Get Data from Stack and then delete it
		int nCurrentBreakType = m_pstLscStackBreak->DataPop();
		if(nCurrentBreakType == ebtSWITCH)
		{
			//	sm_bSwitchStatus = FALSE;
			CString strFindEndString = _T("");
			while(1)
			{
				if(m_ReadMsgDialog.GetQuitStatus())
				{
					// End file
					break;
				}
				strFindEndString = ShellSendModeGetString();
				if(strFindEndString != _T(""))
				{
					if(strFindEndString.Find(_T("ENDSWITCH")) != -1)
					{
						break;
					}
				}
			}
		}
		else if(nCurrentBreakType == ebtREPEAT && !m_pstLscStackRepeatCount->IsEmpty())
		{
			CString strFindEndString = _T("");
			int nRepeatStart = 0;
			while(1)
			{
				if(m_ReadMsgDialog.GetQuitStatus())
				{
					// End file
					break;
				}
				strFindEndString = ShellSendModeGetString();
				if(strFindEndString != _T(""))
				{
					if(strFindEndString.Find(_T("REPEAT END")) != -1)
					{
						if(nRepeatStart <= 0)
						{
							// print warning message
							if(m_bShellSendModeLog)
							{
								CString strLogInfo = _T("");
								strLogInfo.AppendFormat(_T("第%d层循环循环跳出结束\r\n"), m_pstLscStackRepeatCount->NumberGet());
								m_strCOMLog += strLogInfo;
								SetComLogDisplayBottom();
							}
							// Delete the data from stack
							m_pstLscStackShellRepeatCount->DataSet(m_pstLscStackRepeatCount->NumberGet(), REPEAT_VALUE_DEFAULT);
							m_pstLscStackRepeatPos->DataPop();
							m_pstLscStackRepeatCount->DataPop();
							//m_pstLscStackShellRepeatCount->DataPop();
							break;
						}
						else
						{
							nRepeatStart--;
						}
					}
					else if(strFindEndString.Find(_T("REPEAT START")) != -1)
					{
						nRepeatStart++;
					}
				}
			}// end while(1)
		}// end else if(nCurrentBreakType == ebtREPEAT && !m_pstLscStackRepeatCount->IsEmpty())
	} // end if(!m_pstLscStackBreak->IsEmpty())
					
	if(m_ReadMsgDialog.GetQuitStatus() == FALSE)
	{
		SetTimer(ID_SEND_MODE_TIME, 1, NULL);
	}
}

void CLSCToolDlg::ShellSendMode_Call(CString strRead)
{
	BOOL bValid = TRUE;
	CString strFind = _T("");
	strRead = strRead.Mid(sm_strSupportCMDTable[essmCALL].GetLength());
	strRead = DeleteStringTabSpaceLeft(strRead);
	strRead = DeleteStringTabSpaceRight(strRead);

	m_pstLscStackCallPos->DataPush(m_stdioReadFile.GetPosition());
	m_stdioReadFile.Seek(0, 0);
	strRead = _T("CALL START = ") + strRead;
	
	while(1)
	{
		if(m_ReadMsgDialog.GetQuitStatus())
		{
			// End file
			break;
		}
		strFind = ShellSendModeGetString();
		
		if(strFind == strRead)
		{
			break;
		}
	}

	if(m_ReadMsgDialog.GetQuitStatus() == FALSE)
	{
		SetTimer(ID_SEND_MODE_TIME, 1, NULL);
	}
}
void CLSCToolDlg::ShellSendMode_CallStart(CString strRead)
{
	if(m_pstLscStackCallPos->IsEmpty())
	{
		CString strFind = _T("");
		while(1)
		{
			if(m_ReadMsgDialog.GetQuitStatus())
			{
				// End file
				break;
			}
			strFind = ShellSendModeGetString();
			
			if(strFind.Find(_T("CALL END")) != -1)
			{
				break;
			}
		}
	}
	if(m_ReadMsgDialog.GetQuitStatus() == FALSE)
	{
		SetTimer(ID_SEND_MODE_TIME, 1, NULL);
	}

}
void CLSCToolDlg::ShellSendMode_CallEnd(CString strRead)
{
	if(!m_pstLscStackCallPos->IsEmpty())
	{
		m_stdioReadFile.Seek(m_pstLscStackCallPos->DataPop(), 0);
	}
	if(m_ReadMsgDialog.GetQuitStatus() == FALSE)
	{
		SetTimer(ID_SEND_MODE_TIME, 1, NULL);
	}
}

void CLSCToolDlg::ShellSendMode_Jump(CString strRead)
{
	BOOL bValid = TRUE;
	CString strFind = _T("");
	strRead = strRead.Mid(sm_strSupportCMDTable[essmJUMP].GetLength());
	strRead = DeleteStringTabSpaceLeft(strRead);
	strRead = DeleteStringTabSpaceRight(strRead);

	if(strRead == _T("END"))
	{
		// Search to the End file
		while(1)
		{
			if(m_ReadMsgDialog.GetQuitStatus())
			{
				// End file
				break;
			}
			strFind = ShellSendModeGetString();
		}
	}
	else
	{
		strRead = _T("JUMP LABEL = ") + strRead;
		m_stdioReadFile.Seek(0, 0);
		while(1)
		{
			if(m_ReadMsgDialog.GetQuitStatus())
			{
				// End file
				break;
			}
			strFind = ShellSendModeGetString();

			if(strFind.Find(strRead) != -1)
			{
				break;
			}
		}
	}

	if(m_ReadMsgDialog.GetQuitStatus() == FALSE)
	{
		SetTimer(ID_SEND_MODE_TIME, 1, NULL);
	}
}

void CLSCToolDlg::ShellSendMode_JumpLabel(CString strRead)
{
	if(m_ReadMsgDialog.GetQuitStatus() == FALSE)
	{
		SetTimer(ID_SEND_MODE_TIME, 1, NULL);
	}
}


void CLSCToolDlg::ShellSendMode_SetCom(CString strRead)
{
	int nEndPos = 0;
	BOOL bValid = TRUE;
	strRead = strRead.Mid(sm_strSupportCMDTable[essmSETCOM].GetLength());
	strRead = DeleteStringTabSpaceLeft(strRead);
	strRead = DeleteStringTabSpaceRight(strRead);
					
	nEndPos = strRead.Find(_T(","));
	if(nEndPos != -1)
	{
		CString strComNumber = _T("");
		int nComNumber = m_nComNumber;
		CString strComInfo = m_strComInfo;

		strComNumber = strRead.Mid(0, nEndPos);
		strComNumber = DeleteStringTabSpaceLeft(strComNumber);
		strComNumber = DeleteStringTabSpaceRight(strComNumber);
		nComNumber = _ttoi(strRead);
		strComInfo = strRead.Mid(nEndPos+1, strRead.GetLength());
		bValid = ComInfoCheck(strComInfo);
		if(bValid)
		{
			m_nComNumber = nComNumber;
			m_strComInfo = strComInfo;
			OnBnClickedButtonDisconnect();
			OnBnClickedButtonConnect();
			ShowCurrentComInfo();
		}

		if(m_bShellSendModeLog) // print the Warning message
		{
			CString strLogInfo = _T("");
			strLogInfo.AppendFormat(_T("切换COM = %d  "), nComNumber);
			strLogInfo = strLogInfo + _T( "数据 = ") + strComInfo + _T("\r\n");
			m_strCOMLog += strLogInfo;
			SetComLogDisplayBottom();
		}
	}
	SetTimer(ID_SEND_MODE_TIME, m_nShellSendTime, NULL);
}
void CLSCToolDlg::ShellSendMode_ShowBottom(CString strRead)
{
	strRead = strRead.Mid(sm_strSupportCMDTable[essmSHOWBOTTOM].GetLength());
	strRead = DeleteStringTabSpaceLeft(strRead);
	strRead = DeleteStringTabSpaceRight(strRead);
	if(strRead.Find(_T("TRUE")) != -1)
	{
		m_bShowBottom = TRUE;
	}
	else
	{
		m_bShowBottom = FALSE;
	}
	SetTimer(ID_SEND_MODE_TIME, m_nShellSendTime, NULL);
}
void CLSCToolDlg::ShellSendMode_SendString(CString strRead)
{
	m_strCurrentReciveString = _T("");
	m_bShellSendAlready = TRUE;
	ComSendString(strRead);
	if(m_bShowSendString)
	{
		if(m_bShowSystemTime)
		{
			CString strSysteTime = _T("");
			strSysteTime = GetSystemTime();
			m_strCOMLog = m_strCOMLog + LOG_SEND_STRING + strSysteTime + strRead + _T("\r\n");
		}
		else
		{
			m_strCOMLog = m_strCOMLog + LOG_SEND_STRING + strRead + _T("\r\n");
		}

		// Set the VBar to the Bottom
		SetComLogDisplayBottom();
	}
	SetTimer(ID_SEND_MODE_TIME, m_nShellSendTime, NULL);
}


void CLSCToolDlg::ShellSendMode(void)
{
	CString strRead = _T("");
	int nCmdType = 0;
	char cInputString[64] = {0};
	char cSendMode[10] = {0};
	strRead = ShellSendModeGetString();
	if(strRead != "")
	{
		for(nCmdType = 0; nCmdType < sizeof(sm_strSupportCMDTable)/sizeof(sm_strSupportCMDTable[0]); nCmdType++)
		{
			if(strRead.Find(sm_strSupportCMDTable[nCmdType]) != -1)
			{
				break;
			}
		}

		// Initial the first Value
		if(m_bShellSendStart)
		{
			m_bShellSendModeLog = FALSE;
			m_bShellSendStart = FALSE;
			m_nShellSendTime = 500;
			m_nShellSendMode = m_nCmdMode;
			m_bShellSendWaitString = FALSE;
			m_bShellSendAlready = FALSE;
			
			// Repeat count default value intial 
			m_pstLscStackShellRepeatCount->InitStack(REPEAT_VALUE_DEFAULT);
			m_pstLscStackRepeatCount->InitStack(0);
			sm_bIfStatus = FALSE;
			sm_bIfMulti = TRUE;

		}
		else
		{
			CString strMsg = _T("正在执行脚本，如果需要暂停或者退出，请点击指定按钮!\r\n 当前正在执行脚本：\r\n");
			strMsg = strMsg + strRead + _T("\r\n");
			m_ReadMsgDialog.SetWarnMsg(strMsg);
			m_ReadMsgDialog.ShowWarnMsg();
		}

		switch(nCmdType)
		{
			case essmTIME:
				ShellSendMode_Time(strRead);
				break;

			case essmMODE:		
				ShellSendMode_Mode(strRead);
				break;

			case essmWAIT:
				ShellSendMode_Wait(strRead);
				break;

			case essmWAITRECEIVE:
				ShellSendMode_WaitReceive(strRead);
				break;

			case essmIFMULT:
				ShellSendMode_IfMulti(strRead);
				break;


			case essmIFRECEIVE:
				ShellSendMode_IfReceive(strRead);
				break;
			
			case essmELSERECIVE:
				ShellSendMode_ElseReceive(strRead);
				break;

			case essmIFRECEIVEEND:
				SetTimer(ID_SEND_MODE_TIME, 1, NULL);
				break;

			case essmSWITCHRECEIVE:
				ShellSendMode_SwitchReceive(strRead);
				break;

			case essmCASE:
			case essmDEFAULT:
			case essmSWITCHRECEIVEEND:
				SetTimer(ID_SEND_MODE_TIME, 1, NULL);
				break;

			case essmLOG:
				ShellSendMode_Log(strRead);
				break;

			case essmSENDLOG:
				ShellSendMode_SendLog(strRead);
				break;

			case essmRECEIVELOG:
				ShellSendMode_ReceiveLog(strRead);
				break;

			case essmTIMELOG:
				ShellSendMode_TimeLog(strRead);
				break;

			case essmPRINT:
				ShellSendMode_Print(strRead);
				break;


			case essmREPEATSTART:
				ShellSendMode_RepeatStart(strRead);
				break;

			case essmREPEATCOUNT:
				ShellSendMode_RepeatCount(strRead);
				break;

			case essmREPEATEND:
				ShellSendMode_RepeatEnd(strRead);
				break;

			case essmBREAK:
				ShellSendMode_Break(strRead);
				break;

			case essmCALL:
				ShellSendMode_Call(strRead);
				break;

			case essmCALLSTART:
				ShellSendMode_CallStart(strRead);
				break;

			case essmCALLEND:
				ShellSendMode_CallEnd(strRead);
				break;
				
			case essmJUMP:
				ShellSendMode_Jump(strRead);
				break;

			case essmJUMPLABEL:
				ShellSendMode_JumpLabel(strRead);
				break;

			case essmSETCOM:
				ShellSendMode_SetCom(strRead);
				break;

			case essmSHOWBOTTOM:
				ShellSendMode_ShowBottom(strRead);
				break;

			default:
				ShellSendMode_SendString(strRead);
				break;
		}
	}
}

CString CLSCToolDlg::ShellSendModeGetString(void)
{
	CString strRead = _T("");
	BOOL bValidString = TRUE;
	while(bValidString)
	{
		strRead = _T("");
		bValidString = m_stdioReadFile.ReadString(strRead);

		// Delete the TAB and space by the left 
		//strRead.TrimLeft(_T("\t "));
		strRead = DeleteStringTabSpaceLeft(strRead);
		
		if(strRead[0] != '#' && strRead != _T(""))
		{
			break;
		}
	}
	if(bValidString == FALSE)
	{
		KillTimer(ID_SEND_MODE_TIME);
		m_stdioReadFile.Close();
		
		m_ReadMsgDialog.SetStopQuitStatus(FALSE);
		m_ReadMsgDialog.SetQuitStatus(TRUE);
		m_ReadMsgDialog.SetWarnMsg(_T("脚本执行完毕!\r\n请点击退出\r\n"));
		m_ReadMsgDialog.ShowWarnMsg();

		// Quit the Shell Need to Reset to show the Receive String
		m_bShowReceiveString = true;
	}

	return strRead;
}

void CLSCToolDlg::ShellSendModeStop(void)
{
	KillTimer(ID_SEND_MODE_TIME);
}

void CLSCToolDlg::ShellSendModeContinue(void)
{
	SetTimer(ID_SEND_MODE_TIME, 10, NULL);
}