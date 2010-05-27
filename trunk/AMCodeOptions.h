#pragma once
#include "afxwin.h"


// CAMCodeOptions dialog 

class CAMCodeOptions : public CDialog
{
	DECLARE_DYNAMIC(CAMCodeOptions)

public:
	CAMCodeOptions(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAMCodeOptions();

	// Call after IDOK. Gets users specified model scaling factor
	float GetScaleFactor ();

	// Centre model at origin
	bool IsCentreAtOrigin ( );

	// Set centre model at origin
	void SetCentreAtOrigin ( bool b );

// Dialog Data
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Model scaling factor for output
	float scale;

	// Centre model at origin
	bool origin;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	CEdit scale1Edit;
public:
	CEdit scale2Edit;
public:
	CButton m_originCheck;
public:
	afx_msg void OnBnClickedOrigin();
public:
	virtual BOOL OnInitDialog();
};
