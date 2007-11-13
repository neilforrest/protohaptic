// Tab5.cpp : implementation file
//

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "Tab5.h"


// CTab5 dialog

IMPLEMENT_DYNAMIC(CTab5, CDialog)

CTab5::CTab5(CWnd* pParent /*=NULL*/)
	: CDialog(CTab5::IDD, pParent)
{
	m_shape= NULL;
	m_doc= NULL;
}

CTab5::~CTab5()
{
}

// Set the document in which the specified shape resides
void CTab5::SetDocument ( CProtoHapticDoc* d )
{
	m_doc= d;
}

// Set the shape under control
void CTab5::setShape ( CShape* s )
{
	if ( m_shape != s )
	{
		if ( s != NULL )
		{
			m_nameEdit.SetWindowText ( s->GetName ( ).GetBuffer ( ) );
			m_nameEdit.EnableWindow ( TRUE );
			m_applyButton.EnableWindow ( TRUE );
			m_autoButton.EnableWindow ( TRUE );
		}
		else
		{
			m_nameEdit.SetWindowText ( "" );
			m_nameEdit.EnableWindow ( FALSE );
			m_applyButton.EnableWindow ( FALSE );
			m_autoButton.EnableWindow ( FALSE );
		}
	}

	m_shape= s;
}

void CTab5::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_nameEdit);
	DDX_Control(pDX, IDC_BUTTON1, m_applyButton);
	DDX_Control(pDX, IDC_BUTTON2, m_autoButton);
}


BEGIN_MESSAGE_MAP(CTab5, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CTab5::OnBnClickedApply)
	ON_BN_CLICKED(IDC_BUTTON2, &CTab5::OnBnClickedAutoName)
END_MESSAGE_MAP()


// CTab5 message handlers

void CTab5::OnBnClickedApply()
{
	// TODO: Add your control notification handler code here
	if ( m_shape == NULL || m_doc == NULL )
	{
		return;
	}

	char s[256];
	m_nameEdit.GetWindowText ( s, 256 );
	CString str ( s );

	if ( m_doc->IsShapeNameFree ( s ) )
	{
		m_shape->SetName ( s );
	}
	else
	{
		MessageBox ( "The specified name already exists or was not in the correct format, please choose another or click 'Auto Name'.\n\nThe shape name must be only letters, numbers or underscores (no spaces) and must not begin with a number.", "Error" );
	}
}

void CTab5::OnBnClickedAutoName()
{
	// TODO: Add your control notification handler code here
	if ( m_shape == NULL || m_doc == NULL )
	{
		return;
	}

	// Get a free shape name
	CString name= m_doc->GetFreeShapeName ( );

	// Copy the name into the edit box
	m_nameEdit.SetWindowText ( name );

	// Reminder to apply changes
	MessageBox ( "You must now click 'Apply' before changes are registered.", "Information" );
}

BOOL CTab5::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_nameEdit.SetWindowText ( "" );
	m_nameEdit.EnableWindow ( FALSE );
	m_applyButton.EnableWindow ( FALSE );
	m_autoButton.EnableWindow ( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
