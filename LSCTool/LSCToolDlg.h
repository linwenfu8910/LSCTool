// LSCToolDlg.h : 头文件
//

#pragma once
#include "mscomm1.h"
#include "afxwin.h"
#include "afx.h"
#include "Stack.h"

// Command Mode define
enum
{
	ecmdmSANYO,
	ecmdmPANAMIS,
	ecmdmPANABIN,
	ecmdmTEXT,
	ecmdmINVALID
};

typedef enum
{
	erSUCCESS,
	erERROR,
	erINVALID,
}eRESULT;


// Timer ID define
#define ID_SEND_REPEAT_TIME 100
#define ID_SEND_EEPROMDATA_TIME 101
#define ID_EEPROM_DAMAGE_TIME 102
#define ID_SEND_MODE_TIME 103

// Serial COM Send/Recieve Buffer 
#define SIZE_COM_BUFFER 1024

// Serial COM Send/Recieve Log 
#define LOG_SEND_STRING _T("S | ")
#define LOG_RECIEVE_STRING _T("R | ")

// Program INI FILE Name
#define INIT_FILE_NAME _T("\\LinSerialComSetup.ini")

// Shell Repeat start/end MAX Nest Call
#define REPEAT_VALUE_DEFAULT 10
#define REPEAT_LOOP_COUNT 10
#define CALL_STACK_COUNT 10
#define BREAK_TYPE_COUNT 100

// Eidt control diaglog, COM Log display Max Line 
#define EDIT_COMLOG_MAX_LINE 38

// SHELL Support Mode 
typedef enum
{
	esmEEPROMBIN2TSCMD,
	esmEEPROMBIN2G$WCMD,
	esmEEPROMWRITE,
	esmEEPROMREAD,
	esmEEPROMDAMAGE,
	esmTRCMDGENERATE,
	esmGRCMDGENERATE,
	esmSENDMODE,
	esmINVALID
}eSHELL_MODE;

typedef enum
{
	edmINIT,
	edmSENDSTARTCMD,
	edmSENDTSCMD,
	edmSENDTRCMD,
	edmCHECKSTARTCMD,
	edmCHECKTSCMD,
	edmCHECKTRCMD,
	edmCHECKDATA,
	edmCHECKREPEAT,
	edmCHECKEEROR,
	edmINVALID
}eDAMAGE_MODE;

// CLSCToolDlg 对话框
class CLSCToolDlg : public CDialog
{
// 构造
public:
	CLSCToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_LSCTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


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
					
	DECLARE_EVENTSINK_MAP()

	CMscomm1 m_MSComm;								// COM Widget object
	void OnCommMscomm1();							// COM widget communication hanlder

	// MFC Widget Variable define
	CString m_strComInfo;							// Use for display the Current COM information on the Dialog
	CString m_strSendString;						// Use for edit the Input send string
	CComboBox m_strComboInputLog;					// Use for display the Send string history 
	int m_nRepeatTime;								// Use for edit the Repeat time, number only
	CString m_strCOMLog;							// Use for display the Send/Recive String from Serial COM
	bool m_bShowSystemTime;							// Use for Show the system time by use SHELL Command, SHOWTIMELOG = TRUE
	bool m_bShowSendString;							// Use for Show the send string by use SHLEL Command, SHOWSENDLOG = TRUE
	bool m_bShowReceiveString;						// Use for Show the receive string by use SHELL Command, SHOWRECEIVELOG = TRUE
	bool m_bEditComLogDisplayComplete;				// Use for Show the COM Log complete or some messages need to V Scroll bottom to display all 
	bool m_bShowBottom;								// Use for Show the COM Log force Display Bottom

	// MFC Widget Message Function define
	afx_msg void OnBnClickedButtonComsetup();		// Serial COM Setup Button "串口设置"
	afx_msg void OnBnClickedButtonConnect();		// Serial COM Connect Button "连接"
	afx_msg void OnBnClickedButtonDisconnect();		// Serial COM Dis-connect Button "断开"
	afx_msg void OnBnClickedButtonSend();			// Serial COM Send Button "连接"
	afx_msg void OnBnClickedButtonClearscreen();	// Clear Screen Button "清除屏幕"
	afx_msg void OnBnClickedButtonClearsendlog();	// Clear Send Log Button "清除发送记录"
	afx_msg void OnBnClickedButtonOpenlog();		// Select COM Open Log Button "打开保存记录"
	afx_msg void OnBnClickedButtonSendlog();		// Serial COM Save Log Button "保存记录"
	afx_msg void OnBnClickedButtonInputshell();		// Serial COM load shell Button "加载脚本"
	afx_msg void OnTimer(UINT_PTR nIDEvent);		// Serial COM Timer Handler
	afx_msg void OnBnClickedCheckSendrepeat();		// Serial COM Check box Click Hander "重复时间"
	afx_msg void OnCbnSelchangeComboInputlog();		// Serial COM Send String Combo box "发送记录"
	afx_msg void OnBnClickedOk();					// Serial COM Press Enter from keyboard, default button
	afx_msg void OnBnClickedButtonQuit();			// Serial COM Quit button "退出"
	void ShowCurrentComInfo(void);					// Serial COM Static display item to show current COM status
	afx_msg void OnBnClickedCheckShowtime();		// Show time log Check Control "显示时间"
	afx_msg void OnBnClickedCheckShowsendlog();		// Show send log Check Control "显示发送记录"
	afx_msg void OnBnClickedRadioCmdSanyo();		// Select the SANYO Command mode
	afx_msg void OnBnClickedRadioCmdMis();			// Select the PANA MIS Command mode
	afx_msg void OnBnClickedRadioCmdBin();			// Select the PANA BIN Command mode
	afx_msg void OnBnClickedRadioCmdText();			// Select the TEXT Command mode
	afx_msg void OnEnVscrollEditComlog();			// V Scroll COM LOG "COM LOG 垂直滚动处理"

	// INI File Variable define
	CString m_strInitFilePath;						// Use for Memory the Init file Path

	// INI File Function define
	void CheckProgramInitFile(void);				// Check Current Program's INI file, If INI FIle wasn't found, then create new one. 
	void WriteSendLogToInitFile(CString strSendLog);// Write the Send Log to the INI file
	void WriteLastStringToInitFile(CString strSendLog);	// Wiret the Last string to the INI file
	void WriteRepeatTimeToInitFile(void);			// Write the Repeat time to INI file
	void WriteCmdModeToInitFile(void);				// Write the Command mode to INI file
	void WriteComSetupInfoToInitFile(void);			// Write the COM Setup Information to the INI file “9600，n,8,1”
	void ClearSendLogToInitFile(void);				// Clear the Send String Log of INI file

	// Internal Variable define
	int m_nComNumber;								// Use for Memory the COM number from INI File
	int m_nCmdMode;									// Use for Memory the Command Mode from INI File
	int m_nCmdModeLast;								// Use for Back up the Command Mode 
	int m_nSendLogCount;							// Use for Memory the Send String histroy total count
	CString m_strSendLastString;					// Use for Memory the Last String to be send
	int m_nRepeatTimeLast;							// Use for Back up the Repeat time
	CString m_strSaveLogPath;						// Use for Back up the Save Log path
	CString m_strCurrentReciveString;				// Use for Back up the Last Recive string
	CString m_strCOMReciveString;					// Use for Back up the Recive string from COM

	// Internal Function define
	CString GetWorkDir(void);						// Get current Program's absolutly path
	int GetSystemTime(WCHAR * pwcBuffer);			// Get current system time from BIOS, save to the buffer
	CString GetSystemTime(void);					// Get current system tiem from BIOS, return to the cstring 
	int GetStringToBuffer(CString strInput, WCHAR * pwBuffer); // Save current string to the Wide Char buffer
	int GetStringToBuffer(CString strInput, char * pcBuffer);	// Save current string to the Char buffer
	bool ComConnect(void);							// Connect the Serail COM, and return the connect status
	eRESULT ComSendString(CString strSend);			// Send String by use the Serial COM
	bool ComInfoCheck(CString strInput);				// COM information string check
	int GetSystemCom(CString * strPortNameList);	// Read current available COM Port name from register 
	void SetComLogDisplayBottom(void);				// Set the log display bottom on the screen.
	CString DeleteStringTabSpaceLeft(CString strInput);		// Delete the TAB SPACE on the left 
	CString DeleteStringTabSpaceRight(CString strInput);	// Delete the TAB SPACE on the right

private:
	CLSCStack<unsigned int> * m_pstLscStackBreak;
	CLSCStack<int> * m_pstLscStackRepeatCount;
	CLSCStack<int> * m_pstLscStackShellRepeatCount;
	CLSCStack<ULONGLONG> * m_pstLscStackRepeatPos;
	CLSCStack<ULONGLONG> * m_pstLscStackCallPos;
	void ShellSendMode_Time(CString strRead);
	void ShellSendMode_Mode(CString strRead);
	void ShellSendMode_Wait(CString strRead);
	void ShellSendMode_WaitReceive(CString strRead);
	void ShellSendMode_IfMulti(CString strRead);
	void ShellSendMode_IfReceive(CString strRead);
	void ShellSendMode_ElseReceive(CString strRead);
	void ShellSendMode_SwitchReceive(CString strRead);
	void ShellSendMode_Log(CString strRead);
	void ShellSendMode_SendLog(CString strRead);
	void ShellSendMode_ReceiveLog(CString strRead);
	void ShellSendMode_TimeLog(CString strRead);
	void ShellSendMode_Print(CString strRead);
	void ShellSendMode_RepeatStart(CString strRead);
	void ShellSendMode_RepeatCount(CString strRead);
	void ShellSendMode_RepeatEnd(CString strRead);
	void ShellSendMode_Break(CString strRead);
	void ShellSendMode_Call(CString strRead);
	void ShellSendMode_CallStart(CString strRead);
	void ShellSendMode_CallEnd(CString strRead);
	void ShellSendMode_Jump(CString strRead);
	void ShellSendMode_JumpLabel(CString strRead);
	void ShellSendMode_SetCom(CString strRead);
	void ShellSendMode_ShowBottom(CString strRead);
	void ShellSendMode_SendString(CString strRead);

public:	
	// Support Shell variable define 
	bool m_bComSyncStatus;							// Use for Memory current Shell Communication sync status
	CStdioFile m_stdioReadFile;						// Use for Memory current Shell file pointer
	int m_nShellMode;								// Use for Memory current Shell Mode
	int m_nShellMaxLine;							// Use for Get current Shell file's MAX Line
	CString m_strShellPathName;						// Use for Memory current Shell path name
	eDAMAGE_MODE m_eDamageState;					// Use for EEPROM Damage mode shell's process state.

	// Support Shell function define
	eRESULT ShellFileCheck(void);					// Shell file check, to justify current file is a shell or not
	BOOL ShellCommunicationCheck(void);				// Figure out that current communication can be continue or not, (Send <--> RECEIVE handshake)
	void ShellBinCovertTSCmd(void);					// Shell Mode : BIN File convert to the TS Command (Need projector support TS NVRAMD DATA XXXX : XXXXXXX)
	void ShellBinCovertGWCmd(void);					// Shell Mode : BIN File conver to the G$W Command (Need projector support G$W:IIA0XXXXXXX)
	void ShellEepromWrite(void);					// Shell Mode : read the string from file and then write the string into the EEPROM
	void ShellEepromRead(void);						// Shell Mode : read the string from file and then read the string from EEPROM
	void ShellEepromDamage(void);					// Shell Mode : EEPROM Damage test, Write a string to EEPROM, and then read the string from EEPROM, then check these two string.
	void ShellTRCommandGenerate(void);				// Shell Mode : Generate the TR Command 
	void ShellGRCommandGenerate(void);				// Shell Mode : Generate the G$R Command 
	void ShellStop(void);							// Stop current shell, and close the shell file
	int ShellGetMaxLine(void);						// Get current shell file's max line
	void ShellSendMode(void);						// Shell Mode : Send mode, read the string from input file, and then send the string, and don't check the reply from projector
	CString ShellSendModeGetString(void);			// Shell Mode : Send mode, get a string from input file
	int m_nShellSendTime;							// Shell Mode : Send mode, the read a string from input file, and the wait some time 
	int m_nShellSendMode;							// Shell Mode : Send mode, use for change current command mode
//	int m_nShellSendCount[REPEAT_LOOP_COUNT];		// Shell Mode : Send mode, use for send repeat
	bool m_bShellSendModeLog;						// Shell Mode : Send mode, use for memory the Log status of print warning message
	bool m_bShellSendStart;							// Shell Mode : Send mode, Use for Initial the Send Mode Shell default value
	bool m_bShellSendWaitString;					// Shell Mode : Send mode, Use for Make Shell stop until Recieve a string from COM
	bool m_bShellSendAlready;						// Shell Mode : Send mode, Use for figureout current string has been send by COM

	void ShellSendModeStop(void);					// Shell Mode : Send mode, use for stop current shell temply, but not quit.
	void ShellSendModeContinue(void);				// Shell Mode : Send mode, use for continue current shell
};
