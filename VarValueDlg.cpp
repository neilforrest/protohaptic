// VarValueDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "VarValueDlg.h"


// CVarValueDlg dialog

IMPLEMENT_DYNAMIC(CVarValueDlg, CDialog)

CVarValueDlg::CVarValueDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVarValueDlg::IDD, pParent)
{
	// Set defaults
	varValue= NULL;
	varIdent= "?";
	varLine= 0;
	dataOpened= false;		// database already opened?
	userOpenedData= false;	// database opened this time?
	translator= NULL;
}

CVarValueDlg::~CVarValueDlg()
{
}


// Set input to dialog
void CVarValueDlg::SetParameters ( CString* ident, int lineNumber,
								   CString* value,
								   CCodeToDocument* tran,
								   bool databaseOpen)
{
	// Set values
	varValue= value;
	varIdent= *ident;
	varLine= lineNumber;
	dataOpened= databaseOpen; // database already opened?
	translator= tran;
}

void CVarValueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, editIdentifier);
	DDX_Control(pDX, IDC_EDIT2, editLine);
	DDX_Control(pDX, IDC_EDIT3, editValue);
}


BEGIN_MESSAGE_MAP(CVarValueDlg, CDialog)
	ON_BN_CLICKED(IDC_LOAD, &CVarValueDlg::OnBnClickedLoad)
	ON_EN_CHANGE(IDC_EDIT3, &CVarValueDlg::OnEnChangeEdit3)
END_MESSAGE_MAP()


// CVarValueDlg message handlers

void CVarValueDlg::OnBnClickedLoad()
{
	// TODO: Add your control notification handler code here

	// Data already opened
	if ( dataOpened )
		return;

	// Get name of file to load database from
	CString  fname= "";

	CFileDialog dlg(true, ".*", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					"All files(*.*)|*.*||");

	// Check return value of database
	if ( translator != NULL && dlg.DoModal() == IDOK )
	{
		// Load database
    fname= dlg.GetPathName();

		// Load database
		translator->LoadVariableDatabase ( fname );
		userOpenedData= true; // database opened this time

		// Close with IDOK return code
		EndDialog ( IDOK );
	}
}

BOOL CVarValueDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	// Set identifier name
	editIdentifier.SetWindowText ( varIdent );

	// Render line number as string
	char tmp[256];
	sprintf_s ( tmp, 256, "%d", varLine );

	// Set line number text
	editLine.SetWindowText ( tmp );

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CVarValueDlg::OnEnChangeEdit3()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	// Save value of varValue to match edit box
	char tmp[256];
	editValue.GetWindowText ( tmp, 256 );

	if ( varValue != NULL )
	{
		// Update value
		*varValue= CString ( tmp );
	}
}

// Has user opened a database
bool CVarValueDlg::IsOpenedDatabase ( )
{
	return userOpenedData;
}