#pragma once
#include "afxwin.h"


// CSerialComSetup �Ի���

typedef struct
{
	int nCom;
	int nBaudRate;
	int nDataBit;
	int nStopBit;
	int nCheckStatus;
}stCOM_SETUP_INFO;


class CSerialComSetup : public CDialog
{
	DECLARE_DYNAMIC(CSerialComSetup)

public:
	CSerialComSetup(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSerialComSetup();
	
// �Ի�������
	enum { IDD = IDD_COMSETUP };

private:
	BOOL sm_bComSetupInfo;
	stCOM_SETUP_INFO sm_stComSetupInfo;

protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	
	// MFC Widget Variable define
	CComboBox m_ComboComList;								// Use for Display the available COM list
	int m_RadioBaundRate;									// Use for Memory the Baund Rate selection
	int m_RadioDataBit;										// Use for Memory the Data Bit selection
	int m_RadioCheckStatus;									// Use for Memory the Data Check Status selection 
	int m_RadioStopBit;										// Use for Memory the Stop Bit selection

	// MFC Widget Message Function define
	afx_msg void OnBnClickedOk();							// Serial COM confirm Button "ȷ��"
	afx_msg void OnBnClickedButtonComlist();				// Serial COM get from regsiter "��ȡ�����б�"

	// Internal Function define
	void si_fnSerialComSetupSet(CString string);			// Set the Serial COM by use the string like "9600, n, 8, 1"
	void si_fnSerialComSetupGet(CString * pcstring);		// Get the Serial COM string like "9600, n, 8, 1"
	void si_fnUpdateComSetupDlg(void);						// Update the Serial COM dialog
	int GetSystemPorts(CString * pNameList, CString * pPortList);	// Read current available COM Port name from register
	int si_fnSerialComNumberGet(void);						// Get Current COM Setting Number 
	void si_fnSerialComNumberSet(int nComNumber);			// Set Current COM Setting Number
	CString si_fnSerialComSetupBaundRateGet(void);			// Get Current COM baund rate
};
