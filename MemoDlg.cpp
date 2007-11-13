// MemoDlg1.cpp : implementation file
//

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "MemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMemoDlg dialog


CMemoDlg::CMemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMemoDlg)
	m_name = _T("");
	//}}AFX_DATA_INIT
}


void CMemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMemoDlg)
	DDX_Text(pDX, IDC_EDIT1, m_name);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMemoDlg, CDialog)
	//{{AFX_MSG_MAP(CMemoDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMemoDlg message handlers

CString CMemoDlg::getName()
{
	return m_name;
}
