// GHOSTPreImportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "GHOSTPreImportDlg.h"
#include "Parse.h"


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

float CGHOSTPreImportDlg::GetMaxStiffness ( )
{
	return maxStiffness;
}

float CGHOSTPreImportDlg::GetMaxDamping ( )
{
	return maxDamping;
}

void CGHOSTPreImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_rootName);
	DDX_Control(pDX, IDC_RADIO3, m_useSeparatorCheck);
	DDX_Radio ( pDX, IDC_RADIO3, m_useSeparator );
	DDX_Control(pDX, IDC_EDIT2, m_maxStiff);
	DDX_Control(pDX, IDC_EDIT3, m_maxDamp);
	DDX_Control(pDX, IDC_COMBO1, m_device);
}


BEGIN_MESSAGE_MAP(CGHOSTPreImportDlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT1, &CGHOSTPreImportDlg::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT3, &CGHOSTPreImportDlg::OnEnChangeEdit3)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CGHOSTPreImportDlg::OnCbnSelchangeDevice)
END_MESSAGE_MAP()


// CGHOSTPreImportDlg message handlers

BOOL CGHOSTPreImportDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	// Select default option
	m_useSeparatorCheck.SetCheck ( true );

	m_maxStiff.SetWindowText ( "0.5" );
	m_maxDamp.SetWindowText ( "0.003" ); // Defaults for omni

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
void CGHOSTPreImportDlg::OnEnChangeEdit3()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CGHOSTPreImportDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	char s[256];
	m_maxStiff.GetWindowText ( s, 256 );
	
	if ( !StringToFloat ( s, &maxStiffness ) )
	{
		MessageBox ( "Max stiffness must be a number", "Error", MB_OK );
		return;
	}

	m_maxDamp.GetWindowText ( s, 256 );

	if ( !StringToFloat ( s, &maxDamping ) )
	{
		MessageBox ( "Max damping must be a number", "Error", MB_OK );
		return;
	}

	if ( maxStiffness <= 0 || maxDamping <= 0 )
	{
		MessageBox ( "Numbers must be > 0", "Error", MB_OK );
		return;
	}

	CDialog::OnOK();
}

void CGHOSTPreImportDlg::OnCbnSelchangeDevice()
{
	// TODO: Add your control notification handler code here
	int index= m_device.GetCurSel ();

	if ( index == 0 ) // Omni
	{
		m_maxDamp.SetWindowText ( "0.003" );
		m_maxStiff.SetWindowText ( "0.5" );
	}
	else if ( index == 1 ) // Premium
	{
		m_maxDamp.SetWindowText ( "0.002" );
		m_maxStiff.SetWindowText ( "0.6" );
	}
}
