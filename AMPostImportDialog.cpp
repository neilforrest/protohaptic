// AMPostImportDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "AMPostImportDialog.h"

#include "Composite.h"
#include <math.h>

// CAMPostImportDialog dialog

IMPLEMENT_DYNAMIC(CAMPostImportDialog, CDialog)

CAMPostImportDialog::CAMPostImportDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAMPostImportDialog::IDD, pParent)
{
	translator= NULL;
	document= NULL;
	view= NULL;

	// The last scale factor that was applied
	lastScale= 1.0;
}

CAMPostImportDialog::~CAMPostImportDialog()
{
}

// Set ptr to translator
void CAMPostImportDialog::SetTranslator ( CCodeToDocument* t )
{
	translator= t;
}

// Set ptr to document
void CAMPostImportDialog::SetDocument ( CProtoHapticDoc* d )
{
	document= d;
}

// Set ptr to view
void CAMPostImportDialog::SetView ( CProtoHapticView* v )
{
	view= v;
}

void CAMPostImportDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, scale1);
	DDX_Control(pDX, IDC_EDIT2, scale2);
}


BEGIN_MESSAGE_MAP(CAMPostImportDialog, CDialog)
	ON_EN_CHANGE(IDC_EDIT1, &CAMPostImportDialog::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CAMPostImportDialog::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON1, &CAMPostImportDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_SCALE, &CAMPostImportDialog::OnBnClickedButtonScale)
END_MESSAGE_MAP()


// CAMPostImportDialog message handlers

void CAMPostImportDialog::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CAMPostImportDialog::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CAMPostImportDialog::OnBnClickedButton1()
{
	CFileDialog dlg(false, "", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					"All files(*.*)|*.*||");

	// Get name of file to load database from
	CString  fname= "";

	// Get filename for import
	if( ( translator != NULL ) && ( dlg.DoModal() == IDOK ) )
	{
		// Save data
		fname= dlg.GetPathName();
		translator->SaveVariableDatabase ( fname );
	}
}

void CAMPostImportDialog::OnBnClickedButtonScale()
{
	// Cannot complete if document not set
	if ( document == NULL || view == NULL)
	{
		return;
	}

	char scale1Str[256];
	int scale1Int;	// before decimal
	char scale2Str[256];
	int scale2Int; // after decimal
	float scale; // scale model by this factor

	// Get int before decimal place
	scale1.GetWindowText ( scale1Str, 256 );

	// Get int after decimal place
	scale2.GetWindowText ( scale2Str, 256 );

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

	// Save current scale as last scale
	float tmp= scale;

	// Calculate scale relative to last scale that was applied
	scale/= lastScale;

	// Save current scale as last scale
	lastScale= tmp;

	// Don't scale by <=0
	if ( scale <= 0.0 )
	{
		// Display error then return
		MessageBox ( "Error: Enter a scale factor greater than 0", "Error" );
		return;
	}

	// Scale whole document by "scale", centered at origin
	document->ScaleModel ( scale );

	// Now re-center the camera view
	view->CenterCamera ();

/*	// Create one composite of all shapes in model
	CComposite* composite= new CComposite ( );

	// Add the shapes to composite
	for ( int i= 0; i < document->GetShapeCount (); i++ )
	{
		composite->addShape ( document->GetShape ( i ) );
	}

	// Remove shapes from model
	while ( document->GetShapeCount () > 0 )
	{
		document->RemoveShape ( document->GetShape ( 0 ) );
	}

	// Add composite to model
	document->AddShape ( composite );

	// Scale the composite by "scale"

	// Get current size
	float sizeX= composite->getSizeX ();
	float sizeY= composite->getSizeY ();
	float sizeZ= composite->getSizeZ ();

	// Set the scaled size
	composite->setSize ( sizeX * scale, sizeY * scale, sizeZ * scale );

	// Decompose the composite

	// Add composite members to model
	for ( int i= 0; i < composite->getShapeCount (); i++ )
	{
		document->AddShape ( composite->getShape ( i ) );
	}

	// Remove composite from model
	document->RemoveShape ( composite );
	*/

}
