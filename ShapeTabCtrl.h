// ProtoHaptic
// ===========
// Author: Unknown

// File: ShapeTabCtrl.h
// Classes: ShapeTabCtrl
// Purpose: Manages the tabs in the shape properties dialog

#if !defined(AFX_SHAPETABCTRL_H__4C4C4E66_A557_49BF_92D9_F61827866DBE__INCLUDED_)
#define AFX_SHAPETABCTRL_H__4C4C4E66_A557_49BF_92D9_F61827866DBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/** Manages the tabs in the shape properties dialog */
class ShapeTabCtrl : public CTabCtrl
{

public:
	/** Construction */
	ShapeTabCtrl();

public:
	/** Return the dialog in tab pane i */
	CDialog* getPane(int i);

	/** Destruction */
	virtual ~ShapeTabCtrl();

	/** Function to Create the dialog boxes during startup */
	void InitDialogs();

	/** Function to activate the tab dialog boxes */
	void ActivateTabDialogs();

protected:
	/** Array to hold the list of dialog boxes/tab pages for CTabCtrl */
	int m_DialogID[5];

	/** Number of tabs */
	int m_nPageCount;

	/** CDialog Array Variable to hold the dialogs */
	CDialog *m_Dialog[5];

	//{{AFX_MSG(ShapeTabCtrl)
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHAPETABCTRL_H__4C4C4E66_A557_49BF_92D9_F61827866DBE__INCLUDED_)
