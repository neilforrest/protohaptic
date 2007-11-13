// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Tab4.h
// Classes: CTab4
// Purpose: A tab in the shape properties dialog

#if !defined(AFX_TAB4_H__9A990B24_32D0_4E12_AF7D_1CF37A225772__INCLUDED_)
#define AFX_TAB4_H__9A990B24_32D0_4E12_AF7D_1CF37A225772__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Shape.h"

/** A tab in the shape properties dialog */
class CTab4 : public CDialog
{
public:
	/** Draws the dialog and controls */ 
	void OnPaint();

	/** Sets the shape whose properties will be altered */
	void setShape(CShape* s);

	/** Standard constructor */
	CTab4(CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CTab4)
	enum { IDD = IDD_DIALOG4 };
	CEdit	m_mass;
	CButton	m_proxyDynamic;
	CButton	m_collisionDynamic;
	CSliderCtrl	m_frictionSlider;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTab4)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	/** The shape whose properties will be altered */
	CShape* m_shape;

	// Generated message map functions
	//{{AFX_MSG(CTab4)
	afx_msg void OnChangeEdit();
	afx_msg void OnProxyDynamic();
	afx_msg void OnCollisionDynamic();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TAB4_H__9A990B24_32D0_4E12_AF7D_1CF37A225772__INCLUDED_)
