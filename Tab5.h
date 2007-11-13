#pragma once

#include "Shape.h"
#include "afxwin.h"
#include "ProtoHapticDoc.h"

// CTab5 dialog

class CTab5 : public CDialog
{
	DECLARE_DYNAMIC(CTab5)

public:
	CTab5(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTab5();

	// Set the shape under control
	void setShape ( CShape* s );

	// Set the document in which the specified shape resides
	void SetDocument ( CProtoHapticDoc* d );

// Dialog Data
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Shape under control
	CShape* m_shape;

	// The document in which the current shape resides
	CProtoHapticDoc* m_doc;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedApply();
public:
	afx_msg void OnBnClickedAutoName();
public:
	CEdit m_nameEdit;
public:
	CButton m_applyButton;
public:
	CButton m_autoButton;
public:
	virtual BOOL OnInitDialog();
};
