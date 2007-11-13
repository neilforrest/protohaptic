// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006
// File: MemoDlg.h
// Classes: CMemoDlg
// Purpose: A dialog to create a new memo point

#if !defined(AFX_MEMODLG1_H__823DD800_16B6_46E4_B194_1D7F76A22F7C__INCLUDED_)
#define AFX_MEMODLG1_H__823DD800_16B6_46E4_B194_1D7F76A22F7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/** A dialog to create a new memo point */
class CMemoDlg : public CDialog
{
// Construction
public:
	/** Get the name for the memo entered by the user */
	CString getName();
	
	/** Default constructor */
	CMemoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMemoDlg)
	enum { IDD = IDD_MEMO };
	CString	m_name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMemoDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEMODLG1_H__823DD800_16B6_46E4_B194_1D7F76A22F7C__INCLUDED_)
