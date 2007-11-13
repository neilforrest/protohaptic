// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Tab1.h
// Classes: CTab1
// Purpose: A tab in the shape properties dialog

#if !defined(AFX_TAB1_H__05311E10_C06C_4EC7_9FFC_9ADD0E9E97F7__INCLUDED_)
#define AFX_TAB1_H__05311E10_C06C_4EC7_9FFC_9ADD0E9E97F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Shape.h"


/** A tab in the shape properties dialog */
class CTab1 : public CDialog
{

public:
	/** Sets the shape whose properties will be altered */
	void setShape(CShape* s);

    /** standard constructor */
	CTab1(CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CTab1)
	enum { IDD = IDD_DIALOG1 };
	CSliderCtrl	m_sliderBlue;
	CSliderCtrl	m_sliderGreen;
	CSliderCtrl	m_sliderRed;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTab1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	/** The shape whose properties will be altered */
	CShape* m_shape;

	// Generated message map functions
	//{{AFX_MSG(CTab1)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TAB1_H__05311E10_C06C_4EC7_9FFC_9ADD0E9E97F7__INCLUDED_)
