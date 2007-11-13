// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006
// File: PhantomsDlg.h
// Classes: CPhantomsDlg
// Purpose: A dialog allowing the configuration of haptic devices

#if !defined(AFX_PHANTOMSDLG_H__2464A69B_EE6F_456C_A828_AAE99C737E74__INCLUDED_)
#define AFX_PHANTOMSDLG_H__2464A69B_EE6F_456C_A828_AAE99C737E74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ProtoHaptic.h"
#include "ProtoHapticDoc.h"
#include "ProtoHapticView.h"

/** A dialog allowing the configuration of haptic devices */
class CPhantomsDlg : public CDialog
{
// Construction
public:
	/** Update the dialogs controls with data from the application */
	void UpdateControls();
	
	/** Set the view for this dialog to act upon */
	void setView(CView *view);
	
	/** Default constructor */
	CPhantomsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPhantomsDlg)
	enum { IDD = IDD_PHANTOMS };
	CButton	m_active;
	CButton	m_passive;
	CEdit	m_secondary;
	CEdit	m_primary;
	CButton	m_enable;
	CButton	m_default;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhantomsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	/** The view the dialog changes */
	CProtoHapticView* m_view;

	// Generated message map functions
	//{{AFX_MSG(CPhantomsDlg)
	afx_msg void OnPassive();
	afx_msg void OnActive();
	afx_msg void OnApply();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheck1();
	afx_msg void OnCheck3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHANTOMSDLG_H__2464A69B_EE6F_456C_A828_AAE99C737E74__INCLUDED_)
