// AMCodeOptions.cpp : implementation file
//

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "AMCodeOptions.h"

#include <math.h>

// CAMCodeOptions dialog

IMPLEMENT_DYNAMIC(CAMCodeOptions, CDialog)

CAMCodeOptions::CAMCodeOptions(CWnd* pParent /*=NULL*/)
	: CDialog(CAMCodeOptions::IDD, pParent)
{
	// Default output scale is 1:1
	scale= 1.0;

	// Default, keep original origin
	//origin= false;
}

CAMCodeOptions::~CAMCodeOptions()
{
}

// Call after IDOK. Gets users specified model scaling factor
float CAMCodeOptions::GetScaleFactor ()
{
	return scale;
}

// Centre model at origin
bool CAMCodeOptions::IsCentreAtOrigin ( )
{
	return origin;
}

// Set centre model at origin
void CAMCodeOptions::SetCentreAtOrigin ( bool b )
{
	origin= b;
}

void CAMCodeOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, scale1Edit);
	DDX_Control(pDX, IDC_EDIT2, scale2Edit);
	DDX_Control(pDX, IDC_CHECK1, m_originCheck);
}


BEGIN_MESSAGE_MAP(CAMCodeOptions, CDialog)
	ON_BN_CLICKED(IDOK, &CAMCodeOptions::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK1, &CAMCodeOptions::OnBnClickedOrigin)
END_MESSAGE_MAP()


// CAMCodeOptions message handlers

void CAMCodeOptions::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	char scale1Str[256];
	int scale1Int;	// before decimal
	char scale2Str[256];
	int scale2Int; // after decimal

	// Get int before decimal place
	scale1Edit.GetWindowText ( scale1Str, 256 );

	// Get int after decimal place
	scale2Edit.GetWindowText ( scale2Str, 256 );

	// Check string length > 0
	if ( strlen ( scale1Str ) == 0 || strlen ( scale2Str ) == 0 )
	{
		// Display error then return
		MessageBox ( "Error: Enter a scale factor greater than 0", "Error" );
		return;
	}

	// Str to ints
	sscanf_s ( scale1Str, "%d", &scale1Int );
	sscanf_s ( scale2Str, "%d", &scale2Int );

	// Calculate scale
	scale= ((float)scale1Int) + ( ((float)scale2Int) / ( pow ( 10.0f, (float)strlen ( scale2Str ) ) ) );


	OnOK();
}

void CAMCodeOptions::OnBnClickedOrigin()
{
	// TODO: Add your control notification handler code here
	origin= m_originCheck.GetCheck () == TRUE;
}

BOOL CAMCodeOptions::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_originCheck.SetCheck ( origin );

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}