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

// Dialog Data
	enum { IDD = IDD_GHOST_PREIMPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString m_rootSepName;


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
};
