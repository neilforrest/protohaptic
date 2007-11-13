// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006
// File: LogicalPropertiesDlg.h
// Classes: CLogicalPropertiesDlg
// Purpose: A dialog which allows the user to set a shapes logical property formulae

#if !defined(AFX_LOGICALPROPERTIESDLG_H__31EF8DCC_B6A3_4BA3_9399_B47920FA7013__INCLUDED_)
#define AFX_LOGICALPROPERTIESDLG_H__31EF8DCC_B6A3_4BA3_9399_B47920FA7013__INCLUDED_

#include "Shape.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/** A dialog which allows the user to set a shapes logical property formulae */
class CLogicalPropertiesDlg : public CDialog
{
// Construction
public:
	/** Sets the shape to act upon */
	void setShape(CShape *s);
	
	/** Default constructor */
	CLogicalPropertiesDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLogicalPropertiesDlg)
	enum { IDD = IDD_FORMULAS };
	CEdit	m_static;
	CEdit	m_dynamic;
	CEdit	m_dampening;
	CEdit	m_stiffness;
	CButton	m_enable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogicalPropertiesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	/** The shape to act upon */
	CShape *m_shape;

	// Generated message map functions
	//{{AFX_MSG(CLogicalPropertiesDlg)
	afx_msg void OnPaint();
	afx_msg void OnEnable();
	afx_msg void OnApply();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGICALPROPERTIESDLG_H__31EF8DCC_B6A3_4BA3_9399_B47920FA7013__INCLUDED_)
