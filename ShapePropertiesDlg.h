// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: ShapePropertiesDlg.h
// Classes: CShapePropertiesDlg
// Purpose: Represents the shape properties dialog box

#if !defined(AFX_SHAPEPROPERTIESDLG_H__D548294D_C36E_4523_A28B_F6E8DC56AEFD__INCLUDED_)
#define AFX_SHAPEPROPERTIESDLG_H__D548294D_C36E_4523_A28B_F6E8DC56AEFD__INCLUDED_

#include "ShapeTabCtrl.h"
#include "Shape.h"	// Added by ClassView
#include "ProtoHapticDoc.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/** Represents the shape properties dialog box */
class CShapePropertiesDlg : public CDialog
{
// Construction
public:
	/** Set the shape whose properties will be altered */
	void setShape(CShape *shape);

	/** Set the document in which the specified shape resides */
	void SetDocument ( CProtoHapticDoc* doc );

	/** Set the dialog visible or hidden */
	void setVisable(BOOL visable);

	/** Returns true iff the dialog is visable */
	BOOL isVisable();

	/** Standard constructor */
	CShapePropertiesDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CShapePropertiesDlg)
	enum { IDD = IDD_SHAPE };
	ShapeTabCtrl	m_tabCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShapePropertiesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	/** The shape whose properties will be altered */
	CShape* m_shape;

	/** The document in which m_shape resides */
	CProtoHapticDoc *m_doc;

	/** True iff the dialog is visable */
	BOOL m_visable;

	// Generated message map functions
	//{{AFX_MSG(CShapePropertiesDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCloseShapes();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnCancelMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHAPEPROPERTIESDLG_H__D548294D_C36E_4523_A28B_F6E8DC56AEFD__INCLUDED_)
