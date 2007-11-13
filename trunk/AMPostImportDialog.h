#pragma once
#include "afxwin.h"

#include "CodeToDocument.h"
#include "ProtoHapticView.h"

// CAMPostImportDialog dialog

class CAMPostImportDialog : public CDialog
{
	DECLARE_DYNAMIC(CAMPostImportDialog)

public:
	CAMPostImportDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAMPostImportDialog();

	// Set ptr to translator
	void SetTranslator ( CCodeToDocument* t );

	// Set ptr to document
	void SetDocument ( CProtoHapticDoc* d );

	// Set ptr to view
	void SetView ( CProtoHapticView* v );

// Dialog Data
	enum { IDD = IDD_AMPOSTIMPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Ptr to translator, to save variables of
	CCodeToDocument* translator;

	// PTr to docuemnt, to change scale factor
	CProtoHapticDoc* document;

	// Ptr to view, to re-center view
	CProtoHapticView* view;

	// The last scale factor that was applied
	float lastScale;

	DECLARE_MESSAGE_MAP()
public:
	CEdit scale1;
public:
	CEdit scale2;
public:
	afx_msg void OnEnChangeEdit1();
public:
	afx_msg void OnEnChangeEdit2();
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedButtonScale();
};
