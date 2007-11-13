// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006// File: AppOptionsDlg.h// Classes: CAppOptionsDlg// Purpose: A dialog controlling global application options

#if !defined(AFX_APPOPTIONSDLG_H__6157A9DA_3029_4DAD_8505_210A425F98F6__INCLUDED_)
#define AFX_APPOPTIONSDLG_H__6157A9DA_3029_4DAD_8505_210A425F98F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// The Phantom Devices config dialog
#include "PhantomsDlg.h"

/// A dialog controlling global application options
class CAppOptionsDlg : public CDialog
{
// Construction
public:
	void setView(CView *view);
	CAppOptionsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAppOptionsDlg)
	enum { IDD = IDD_APPOPTIONS };
	CSliderCtrl	m_stacks;
	CButton	m_edit;
	CButton	m_snap;
	CButton	m_touch;
	CButton	m_frontCheck;
	CButton	m_sideCheck;
	CButton	m_planCheck;
	CSliderCtrl	m_editPointSnapDist;
	CSliderCtrl	m_rotationGuess;
	CSliderCtrl	m_editPointSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppOptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CView *m_view;

	// Generated message map functions
	//{{AFX_MSG(CAppOptionsDlg)
	afx_msg void OnConfigDevices();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	afx_msg void OnButtonSave();
	afx_msg void OnPlan();
	afx_msg void OnFront();
	afx_msg void OnSide();
	afx_msg void OnBeginner();
	afx_msg void OnIntermediate();
	afx_msg void OnExpert();
	afx_msg void OnTouch();
	afx_msg void OnSnapTo();
	afx_msg void OnEditPoint();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPOPTIONSDLG_H__6157A9DA_3029_4DAD_8505_210A425F98F6__INCLUDED_)
