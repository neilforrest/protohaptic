// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006
// File: DocumentOptionsDlg.h
// Classes: CDocumentOptionsDlg
// Purpose: A dialog controlling the document options

#if !defined(AFX_DOCUMENTOPTIONSDLG_H__032943F5_9A28_48B4_8CA2_3F26298348AD__INCLUDED_)
#define AFX_DOCUMENTOPTIONSDLG_H__032943F5_9A28_48B4_8CA2_3F26298348AD__INCLUDED_

#include "ProtoHapticDoc.h"	// Added by ClassView
#include "afxwin.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/** A dialog controlling the document options */
class CDocumentOptionsDlg : public CDialog
{
public:
	/** Create the dialog */
	CDocumentOptionsDlg(CWnd* pParent = NULL);   // standard constructor

	/** Set the document upon which the dialog acts */
	void setDocument(CProtoHapticDoc *d);

// Dialog Data
	//{{AFX_DATA(CDocumentOptionsDlg)
	enum { IDD = IDD_DOCOPTIONS };
	CButton	m_hideMemos;
	CSliderCtrl	m_gravitySlider;
	CSliderCtrl		m_simSpeedSlider;
	CSliderCtrl		m_airResistanceSlider;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDocumentOptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	/** The document on which the dialog acts */
	CProtoHapticDoc *m_document;

	// Generated message map functions
	//{{AFX_MSG(CDocumentOptionsDlg)
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHideMemos();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CButton m_enableSurfaceChange;
public:
	afx_msg void OnBnClickedCheck2();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOCUMENTOPTIONSDLG_H__032943F5_9A28_48B4_8CA2_3F26298348AD__INCLUDED_)
