#pragma once

#include "afxwin.h"
#include "resource.h"
#include "CodeToDocument.h"

// CVarValueDlg dialog

class CVarValueDlg : public CDialog
{
	DECLARE_DYNAMIC(CVarValueDlg)

public:
	CVarValueDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVarValueDlg();

	// Set parameters
	void SetParameters ( CString* ident, int lineNumber,
						 CString* value,
						 CCodeToDocument* tran,
						 bool databaseOpen);

	// Has user opened a database
	bool IsOpenedDatabase ( );

// Dialog Data
	enum { IDD = IDD_VARVALUE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Ptr to value to change
	CString* varValue;

	// Name of identifier
	CString varIdent;

	// Line number
	int varLine;

	// Ptr to translator (for loading variable database)
	CCodeToDocument* translator;

	// Database already opened
	bool dataOpened;

	// User has opened database this time
	bool userOpenedData;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLoad();
public:
	CEdit editIdentifier;
public:
	CEdit editLine;
public:
	CEdit editValue;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnEnChangeEdit3();
};
