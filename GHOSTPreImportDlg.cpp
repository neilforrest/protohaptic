// GHOSTPreImportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "GHOSTPreImportDlg.h"


// CGHOSTPreImportDlg dialog

IMPLEMENT_DYNAMIC(CGHOSTPreImportDlg, CDialog)

CGHOSTPreImportDlg::CGHOSTPreImportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGHOSTPreImportDlg::IDD, pParent)
	, m_useSeparator(0)
{

}

CGHOSTPreImportDlg::~CGHOSTPreImportDlg()
{
}

void CGHOSTPreImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_rootName);
	DDX_Control(pDX, IDC_RADIO3, m_useSeparatorCheck);
	DDX_Radio ( pDX, IDC_RADIO3, m_useSeparator );
}


BEGIN_MESSAGE_MAP(CGHOSTPreImportDlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT1, &CGHOSTPreImportDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CGHOSTPreImportDlg message handlers

BOOL CGHOSTPreImportDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	// Select default option
	m_useSeparatorCheck.SetCheck ( true );
	//MessageBox ( "", "", MB_OK );

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CGHOSTPreImportDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	char str[256];
	m_rootName.GetWindowText ( str, 256 );

	m_rootSepName= CString ( str );
}

CString CGHOSTPreImportDlg::GetRootSepName ( )
{
	return m_rootSepName;
}