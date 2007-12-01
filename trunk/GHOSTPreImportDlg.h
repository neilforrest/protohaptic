#pragma once
#include "afxwin.h"


// CGHOSTPreImportDlg dialog

class CGHOSTPreImportDlg : public CDialog
{
	DECLARE_DYNAMIC(CGHOSTPreImportDlg)

public:
	CGHOSTPreImportDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGHOSTPreImportDlg();

	CString GetRootSepName ( );
	float GetMaxStiffness ( );
	float GetMaxDamping ( );

// Dialog Data
	enum { IDD = IDD_GHOST_PREIMPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString m_rootSepName;
	float maxStiffness;
	float maxDamping;

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_rootName;
public:
	virtual BOOL OnInitDialog();
public:
	int m_useSeparator;
public:
	CButton m_useSeparatorCheck;
public:
	afx_msg void OnEnChangeEdit1();
public:
	afx_msg void OnEnChangeEdit3();
public:
	CEdit m_maxStiff;
public:
	CEdit m_maxDamp;
protected:
	virtual void OnOK();
public:
	CComboBox m_device;
public:
	afx_msg void OnCbnSelchangeDevice();
};
