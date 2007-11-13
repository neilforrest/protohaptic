// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Tab3.h
// Classes: CTab3
// Purpose: A tab in the shape properties dialog

#if !defined(AFX_TAB3_H__DC05327F_33D6_4968_AAFA_2CAE38889A74__INCLUDED_)
#define AFX_TAB3_H__DC05327F_33D6_4968_AAFA_2CAE38889A74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Shape.h"
#include "afxwin.h"
#include "ProtoHapticDoc.h"

/** A tab in the shape properties dialog */
class CTab3 : public CDialog
{
public:
	/** Sets the shape whose properties will be altered */
	void setShape(CShape* s);

	/** Set the document in which the specified shape resides */
	void SetDocument ( CProtoHapticDoc* doc );


	/** Standard constructor */
	CTab3(CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CTab3)
	enum { IDD = IDD_DIALOG3 };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTab3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Convert string to float
	bool StringToFloat ( char* str, float* flt );

	// Returns true if the shapes properties have changed since last call
	bool ShapeChanged ( );

	// Update edit boxes from shape properties
	void UpdateEditBoxes ();

	// Get Euler angles from rotation matrix
	void GetEulerFromMatrix ( float* matrix, float *x, float *y, float *z );

	// Shape properties at last call to ShapeChanged
	float lastPos[3];
	float lastSize[3];
	float lastRot[16];
	bool shapeSet; // A non-NULL shape was set

	/** The shape whose properties will be altered */
	CShape* m_shape;

	/** The document in which the specified shape resides */
	CProtoHapticDoc* m_doc;

	// Generated message map functions
	//{{AFX_MSG(CTab3)
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_posX;
public:
	CEdit m_posY;
public:
	CEdit m_posZ;
public:
	afx_msg void OnBnClickedApplyPosition();
public:
	afx_msg void OnBnClickedSizeApply();
public:
	afx_msg void OnBnClickedRotationApply();
public:
	CEdit m_sizeX;
public:
	CEdit m_sizeY;
public:
	CEdit m_sizeZ;
public:
	CEdit m_rotX;
public:
	CEdit m_rotY;
public:
	CEdit m_rotZ;
public:
	CButton m_applyPos;
public:
	CButton m_applySize;
public:
	CButton m_applyRot;
public:
	virtual BOOL OnInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TAB3_H__DC05327F_33D6_4968_AAFA_2CAE38889A74__INCLUDED_)
