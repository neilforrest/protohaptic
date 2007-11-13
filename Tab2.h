// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Tab2.h
// Classes: CTab2
// Purpose: A tab in the shape properties dialog

#if !defined(AFX_TAB2_H__06531ECC_B283_4810_A072_C72F9D9241DD__INCLUDED_)
#define AFX_TAB2_H__06531ECC_B283_4810_A072_C72F9D9241DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Shape.h"
#include "LogicalPropertiesDlg.h"
#include "afxwin.h"

class CTab2 : public CDialog
{
public:
	/** Sets the shape whose properties will be altered */
	void setShape(CShape* s);

	/** Standard constructor */
	CTab2(CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CTab2)
	enum { IDD = IDD_DIALOG2 };
	CSliderCtrl	m_logicalSlider;
	CSliderCtrl	m_dynamic_friction;
	CSliderCtrl	m_static_friction;
	CSliderCtrl	m_dampening;
	CSliderCtrl	m_stiffness;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTab2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	// Update the edit boxes from the sliders
	void UpdateEditBoxes ( );

	// Convert string to float
	bool StringToFloat ( char* str, float* flt );

	/** The shape whose properties will be altered */
	CShape* m_shape;

	/** The logical properties dialog */
	CLogicalPropertiesDlg dlg;

	// Generated message map functions
	//{{AFX_MSG(CTab2)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	afx_msg void OnFormula();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_stiffness1;
public:
	CEdit m_damping1;
public:
	CEdit m_static1;
public:
	CEdit m_dynamic1;
public:
	afx_msg void OnEnChangeEditStiffness1();
public:
	afx_msg void OnEnChangeEditDamping1();
public:
	afx_msg void OnEnChangeEditStatic1();
public:
	afx_msg void OnEnChangeEditDynamic1();
public:
	CButton m_controlMembers;
public:
	afx_msg void OnBnClickedControlMembers();
public:
	afx_msg void OnBnClickedCheck2();
public:
	CButton m_customRenderCheck;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TAB2_H__06531ECC_B283_4810_A072_C72F9D9241DD__INCLUDED_)
