// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: MainFrm.h
// Classes: CMainFrame
// Purpose: Defines the main window

#if !defined(AFX_MAINFRM_H__9B334E46_BD9C_4EA5_B334_33020C6678D4__INCLUDED_)
#define AFX_MAINFRM_H__9B334E46_BD9C_4EA5_B334_33020C6678D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/** Represents the MFC main window */
class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	/** Status bar */
	CStatusBar  m_wndStatusBar;
	
	/** Main application tool bar */
	CToolBar    m_wndToolBar;
	
	/** Drawing toolbar */
	CToolBar    m_wndDrawTools;
	
	/** Viewing toolbar */
	CToolBar    m_wndViewTools;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__9B334E46_BD9C_4EA5_B334_33020C6678D4__INCLUDED_)
