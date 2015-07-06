#pragma once
#include "afxcmn.h"


// CShellProgressMsg 对话框

class CShellProgressMsg : public CDialog
{
	DECLARE_DYNAMIC(CShellProgressMsg)

public:
	CShellProgressMsg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShellProgressMsg();

// 对话框数据
	enum { IDD = IDD_SHELL_PROGRESS };

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CString m_strWarnMsg;					// Use for display the warning message on the dialog
	bool m_bQuitStatus;						// Use for figure out current shell has already been end or not
	bool m_bStopQuitStatus;					// Use for figure out current shell need Stop/Quit two button on the dialog or not
	bool m_bContinueStatus;					// Use for figure out current shell need continue button on the dialog or not
	CProgressCtrl m_proLoad;				// Use for progress acitve bar control
	int m_nProgressRange;					// Use for memory the progress range 

	void SetWarnMsg(CString strMsg);		// Setting the Warn message 
	CString GetWarnMsg(void);				// Getting the Warn message
	void ShowWarnMsg(void);					// Show the Warn message on the dialog
	void SetProgressRange(int nRange);		// Set the range of progess bar
	int UpdateProgress(void);				// Update the progress bar by INC
	void UpdateProgressFull(void);			// Update the progress bar to dispay full
	void SetQuitStatus(bool bStatus);		// Set quit status, current shell has already been end or not 
	bool GetQuitStatus(void);				// Get quit status, current shell has already been end or not 
	void SetStopQuitStatus(bool bStatus);	// Set current Stop quit status, current shell need Stop/Quit or not
	bool GetContinueStatus(void);			// Get continue status, current shell has already been stop or not
	void UpdateProgress(int nCurrentPos);	// Update the progress bar by given position

	afx_msg void OnBnClickedCancel();		// Quite button  "退出"
	afx_msg void OnBnClickedButtonStop();	// Stop button "暂停/继续脚本"
	afx_msg void OnBnClickedButtonQuit();	// Quite button "退出脚本"

	afx_msg void OnClose();
};
