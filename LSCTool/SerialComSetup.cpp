// SerialComSetup.cpp : 实现文件
//

#include "stdafx.h"
#include "LSCTool.h"
#include "SerialComSetup.h"
#include <atlbase.h>
#include <atlcom.h>


// Static Baund rate define
static const WCHAR * sm_cBaundRate[]=
{
	_T("115200"),
	_T("57600"),
	_T("56000"),
	_T("38400"),
	_T("28800"),
	_T("19200"),
	_T("9600"),
	_T("4800"),
	_T("2400"),
	_T("1200"),
};

static const WCHAR * sm_cComList[]=
{
	_T("COM1"),
	_T("COM2"),
	_T("COM3"),
	_T("COM4"),
	_T("COM5"),
	_T("COM6"),
	_T("COM7"),
	_T("COM8"),
	_T("COM9"),
	_T("COM10"),
	_T("COM11"),
	_T("COM12"),
	_T("COM13"),
	_T("COM14"),
	_T("COM15"),
	_T("COM16"),
};


// CSerialComSetup 对话框

IMPLEMENT_DYNAMIC(CSerialComSetup, CDialog)

CSerialComSetup::CSerialComSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CSerialComSetup::IDD, pParent)
	, m_RadioBaundRate(0)
	, m_RadioDataBit(0)
	, m_RadioCheckStatus(0)
	, m_RadioStopBit(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CSerialComSetup::~CSerialComSetup()
{

}

void CSerialComSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_BAUND0, m_RadioBaundRate);
	DDX_Radio(pDX, IDC_RADIO_DATABIT1, m_RadioDataBit);
	DDX_Radio(pDX, IDC_RADIO_CHECK_NONE, m_RadioCheckStatus);
	DDX_Radio(pDX, IDC_RADIO_STOPBIT1, m_RadioStopBit);
	DDX_Control(pDX, IDC_COMBO_COMLIST, m_ComboComList);
}


BEGIN_MESSAGE_MAP(CSerialComSetup, CDialog)
	ON_BN_CLICKED(IDOK, &CSerialComSetup::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_COMLIST, &CSerialComSetup::OnBnClickedButtonComlist)
END_MESSAGE_MAP()


BOOL CSerialComSetup::OnInitDialog()
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

	// TODO: 在此添加额外的初始化代码
	si_fnUpdateComSetupDlg();
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// CSerialComSetup 消息处理程序

void CSerialComSetup::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	
	sm_stComSetupInfo.nBaudRate = m_RadioBaundRate;
	sm_stComSetupInfo.nCheckStatus = m_RadioCheckStatus;
	sm_stComSetupInfo.nDataBit = m_RadioDataBit;
	sm_stComSetupInfo.nStopBit = m_RadioStopBit;


	// Get COM Number
	int nComNumber = 0;
	int nIndex = 0;
	CString strComText;
	int nComboLine = m_ComboComList.GetCurSel();
	m_ComboComList.GetLBText( nComboLine, strComText);
	for(nIndex = 0; nIndex < sizeof(sm_cComList)/sizeof(sm_cComList[0]); nIndex++)
	{
		if(strComText.Compare(sm_cComList[nIndex]) == 0)
		{
			nComNumber = nIndex + 1;
			break;
		}
	}

	sm_stComSetupInfo.nCom = nComNumber;
//	CString strMessage = _T("");
//	strMessage.AppendFormat(_T("COM ID is COM %d"), nComNumber);
//	MessageBox(strMessage);
//	strMessage.AppendFormat(_T("BaudRate = %d, CheckStatus = %d, DataBit = %d, StopBit = %d"), m_RadioBaundRate, m_RadioCheckStatus, m_RadioDataBit, m_RadioStopBit);
//	MessageBox(strMessage); 

	OnOK();
}

void CSerialComSetup::OnBnClickedButtonComlist()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strNameList[20];
	CString strPortList[20];
	int nCount = 0;
	nCount = GetSystemPorts(strNameList, strPortList);
	
	if(nCount != 0)
	{
		int nIndex = 0;
		m_ComboComList.ResetContent();
		for(nIndex = 0; nIndex < nCount; nIndex++)
		{
			m_ComboComList.AddString(strPortList[nIndex]);
		}
		m_ComboComList.SetCurSel(0);
	}

}

void CSerialComSetup::si_fnSerialComSetupSet(CString strInputComInfo)
{
	char nBaundRateIndex = 0;
	int nIndex = 0;
	bool bFind = true;
//	char cBaundRate[7] = {0};
	WCHAR cCheckStatus;
	WCHAR cDataBit;
	WCHAR cStopBit;

	if(strInputComInfo!= _T(""))
	{
		// "9600, n, 8, 1"
		// Get the Bound rate.
		WCHAR cBaudRateBuffer[20] = {0};
		CString strBaudRate = _T("");
		int nBaudRateLength = 0;
		strInputComInfo.Remove(' ');
		strInputComInfo.Remove('\t');
		nBaudRateLength = strInputComInfo.Find(_T(","));
		if(nBaudRateLength == -1)
		{
			// Not found, the input string is error.
			return ;
		}
		
		for(nIndex = 0; nIndex < nBaudRateLength; nIndex++)
		{
			cBaudRateBuffer[nIndex] = strInputComInfo[nIndex];
		}
		strBaudRate = cBaudRateBuffer;
	
		for(nBaundRateIndex =0; nBaundRateIndex< sizeof(sm_cBaundRate)/sizeof(sm_cBaundRate[0]); nBaundRateIndex++)
		{
			if(strBaudRate == sm_cBaundRate[nBaundRateIndex])
			{
				bFind = true;
				break;
			}
		}
		if(bFind)
		{
			sm_stComSetupInfo.nBaudRate = nBaundRateIndex;
		}
		else
		{
			// use the default value 115200
			sm_stComSetupInfo.nBaudRate = 0; 
		}

		int nLength = strInputComInfo.GetLength();
		if((strInputComInfo.GetLength() - nIndex - 1) == 5)
		{
			// Get the Check Status
			cCheckStatus = strInputComInfo[nIndex+1];
			switch(cCheckStatus)
			{
				case 'n':
				case 'N':
					sm_stComSetupInfo.nCheckStatus = 0;
					break;

				case 'o':
				case 'O':
					sm_stComSetupInfo.nCheckStatus = 1;
					break;

				case 'e':
				case 'E':
					sm_stComSetupInfo.nCheckStatus = 2;
					break;

				default:
					sm_stComSetupInfo.nCheckStatus = 0;
					break;
			}

			// Get the Data bit
			cDataBit = strInputComInfo[nIndex+3];
			switch(cDataBit)
			{
				case '7':
					sm_stComSetupInfo.nDataBit = 0;
					break;

				case '8':
					sm_stComSetupInfo.nDataBit = 1;
					break;
				
				default:
					sm_stComSetupInfo.nDataBit = 1;
					break;
			}

			// Get the Stop bit
			cStopBit = strInputComInfo[nIndex+5];
			switch(cStopBit)
			{
				case '1':
					sm_stComSetupInfo.nStopBit = 0;
					break;

				case '2':
					sm_stComSetupInfo.nStopBit = 1;
					break;
				
				default:
					sm_stComSetupInfo.nStopBit = 0;
					break;
			}
		}
		else
		{
			sm_stComSetupInfo.nCheckStatus = 0;
			sm_stComSetupInfo.nDataBit = 1;
			sm_stComSetupInfo.nStopBit = 0;
		}
		sm_bComSetupInfo = true;
	}
}

void CSerialComSetup::si_fnSerialComSetupGet(CString * pcstring)
{
	CString strOutputComInfo = _T("");
	int nBaudRate = sm_stComSetupInfo.nBaudRate;
	int nDataBit = sm_stComSetupInfo.nDataBit;
	int nStopBit = sm_stComSetupInfo.nStopBit;
	int nCheckStatus = sm_stComSetupInfo.nCheckStatus;
	const char * sm_cDataBit[]=
	{
		"7",
		"8",
	};
	const char * sm_cStopBit[]=
	{
		"1",
		"2",
	};
	const char *sm_cCheckStatus[]=
	{
		"n",
		"o",
		"e",
	};

	if( nBaudRate < sizeof(sm_cBaundRate)/sizeof(sm_cBaundRate[0])
		&& nDataBit < sizeof(sm_cDataBit)/sizeof(sm_cDataBit[0])
		&& nStopBit < sizeof(sm_cStopBit)/sizeof(sm_cStopBit[0])
		&& nCheckStatus < sizeof(sm_cCheckStatus)/sizeof(sm_cCheckStatus[0])
		)
	{
		strOutputComInfo.AppendFormat(_T("%s, %s, %s, %s"), sm_cBaundRate[nBaudRate], sm_cCheckStatus[nCheckStatus], sm_cDataBit[nDataBit], sm_cStopBit[nStopBit]);
	}
	else
	{
		// use default value
		strOutputComInfo.AppendFormat(_T("115200, n, 8, 1"));
	}
	*pcstring = strOutputComInfo;
}


void CSerialComSetup::si_fnUpdateComSetupDlg(void)
{
	m_RadioBaundRate = sm_stComSetupInfo.nBaudRate;
	m_RadioCheckStatus = sm_stComSetupInfo.nCheckStatus;
	m_RadioDataBit = sm_stComSetupInfo.nDataBit;
	m_RadioStopBit = sm_stComSetupInfo.nStopBit;
	UpdateData(false);


	int nIndex = 0;
	for(nIndex = 0; nIndex < sizeof(sm_cComList)/sizeof(sm_cComList[0]); nIndex++)
	{
		m_ComboComList.AddString((LPCTSTR)sm_cComList[nIndex]);
	}
	if(sm_stComSetupInfo.nCom < sizeof(sm_cComList)/sizeof(sm_cComList[0]) && sm_stComSetupInfo.nCom >0)
	{
		m_ComboComList.SetCurSel(sm_stComSetupInfo.nCom - 1);
	}
	else
	{
		m_ComboComList.SetCurSel(0);
	}
//	UpdateData(true);
}

int CSerialComSetup::GetSystemPorts(CString * pNameList, CString * pPortList)
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
	DWORD dwValueType;      //键值类型
	WCHAR cValueData[_MAX_PATH];    //键值数值
	do
	{
		DWORD nValueNameSize = _MAX_PATH; //重置键值名称长度
		DWORD nValueDataSize = _MAX_PATH; //重置键值数值长度
		lRet = RegEnumValue(hKey, nCount, (LPWSTR)cValueName, &nValueNameSize, 0, &dwValueType, (LPBYTE)cValueData, &nValueDataSize); //枚举键值
		
		if(lRet == ERROR_SUCCESS)
		{
			if(pNameList)   
				pNameList[nCount] = cValueName;   
               
			if(pPortList)   
				pPortList[nCount] = cValueData; 
			nCount++;
		}
	}while(lRet == ERROR_SUCCESS);   //读取完毕后退出循环
    return nCount;   
}   

int CSerialComSetup::si_fnSerialComNumberGet(void)
{
	int nComNumber = sm_stComSetupInfo.nCom;
	return nComNumber;
}

void CSerialComSetup::si_fnSerialComNumberSet(int nComNumber)
{
	sm_stComSetupInfo.nCom = nComNumber;
}

CString CSerialComSetup::si_fnSerialComSetupBaundRateGet(void)
{
	int nIndex = 0;
	CString strBaudRate = _T("");
	if(sm_stComSetupInfo.nBaudRate < sizeof(sm_cBaundRate)/sizeof(sm_cBaundRate[0]))
	{
		strBaudRate = sm_cBaundRate[sm_stComSetupInfo.nBaudRate];
	}
	return strBaudRate;
}
