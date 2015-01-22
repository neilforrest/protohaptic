// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006
// File: CodeView.h
// Classes: CCodeView
// Purpose: A form view that displays the scene as a sequence of OpenGL

#if !defined(AFX_CODEVIEW_H__BA642215_F2BD_4DF1_B028_3D3D742EBA92__INCLUDED_)
#define AFX_CODEVIEW_H__BA642215_F2BD_4DF1_B028_3D3D742EBA92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ShapeToAbstractModelCode.h"
#include "ShapeToX3DCode.h"

/// A form view that displays the scene as a sequence of OpenGL, glu and glut calls
class CCodeView : public CFormView
{
protected:
	DECLARE_DYNCREATE(CCodeView)

// Operations
public:
	/// Set the document for this view
	void SetDocument(CDocument *d);

	/// Create a new CCodeView
	CCodeView();

	/// Destroy the CCodeView
	virtual ~CCodeView();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCodeView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	// Get a translation so that the document is centered on the origin
	void GetToOriginVector ( float* vect );

	/// A pointer to the active document
	CDocument* m_doc;

	/// A pointer to the combo box listing types of code output
    CComboBox *m_comboCodeTypes;

	/// The currently selected code output type
	int codeType;

	/// AbstractModel translator
	CShapeToAbstractModelCode* amTranslator;

  /// X3D translator
  CShapeToX3DCode* x3dTranslator;

	/// Font to use for the edit box
	CFont* editFont;

	// Scale model before output
	float scaleFactor;

	// Move model to origin before output?
	bool toOrigin;

   //{{AFX_DATA(CCodeView)
   enum { IDD = IDD_CODEVIEW };
   //}}AFX_DATA

	//{{AFX_MSG(CCodeView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSaveCode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeCombo1();
public:
	afx_msg void OnBnClickedButtonOptions();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CODEVIEW_H__BA642215_F2BD_4DF1_B028_3D3D742EBA92__INCLUDED_)
