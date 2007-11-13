// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006
// File: GotoMemo.h
// Classes: CGotoMemo
// Purpose: A dialog which allows the user to select a memo point

#if !defined(AFX_GOTOMEMO_H__AF002E1B_9A63_427E_816F_7F62E022A24E__INCLUDED_)
#define AFX_GOTOMEMO_H__AF002E1B_9A63_427E_816F_7F62E022A24E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/** A dialog which allows the user to select a memo point to goto */
class CGotoMemo : public CDialog
{
// Construction
public:
	/** Set the CProtoHapticView to act upon */
	void setView(CView *view);
	
	/** Empty the list of memo points */
	void clear();
	
	/** Add an identifier to the list of memo points */
	void addName(CString s);
	
	/** Default constructor */
	CGotoMemo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGotoMemo)
	enum { IDD = IDD_GOTO_MEMO };
	CListBox	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGotoMemo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	/** The view to act upon */
	CView* m_view;
	
	/** The number of memo points listed */
	int m_count;

	// Generated message map functions
	//{{AFX_MSG(CGotoMemo)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GOTOMEMO_H__AF002E1B_9A63_427E_816F_7F62E022A24E__INCLUDED_)
