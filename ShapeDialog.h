#if !defined(AFX_SHAPEDIALOG_H__6A6A359B_76E9_45FC_811A_5B68672EEC92__INCLUDED_)
#define AFX_SHAPEDIALOG_H__6A6A359B_76E9_45FC_811A_5B68672EEC92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShapeDialog.h : header file
//
#include "Shape.h"
/////////////////////////////////////////////////////////////////////////////
// CShapeDialog dialog

class CShapeDialog : public CDialog
{
// Construction
public:
	BOOL m_visable;
	void setVisable(BOOL visable);
	BOOL isVisable();
	void SetShape(CShape* shape);
	CShapeDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CShapeDialog)
	enum { IDD = IDD_DIALOG1 };
	CSliderCtrl	m_dynamic_friction;
	CSliderCtrl	m_static_friction;
	CSliderCtrl	m_dampening;
	CSliderCtrl	m_stiffness;
	CSliderCtrl	m_sliderBlue;
	CSliderCtrl	m_sliderGreen;
	CSliderCtrl	m_sliderRed;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShapeDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CShape* m_shape;
	// Generated message map functions
	//{{AFX_MSG(CShapeDialog)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//}}AFX_MSG

	LRESULT OnHScrollSlider(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHAPEDIALOG_H__6A6A359B_76E9_45FC_811A_5B68672EEC92__INCLUDED_)
