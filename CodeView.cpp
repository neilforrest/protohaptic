// CodeView.cpp : implementation file
//

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "ProtoHapticDoc.h"
#include "CodeView.h"

// Output options dialog
#include "AMCodeOptions.h"

// basic file IO operations
#include <iostream>
#include <fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCodeView

// Max number of chars in a single shape's rendering code
static const int maxCodeChar= 9999;

IMPLEMENT_DYNCREATE(CCodeView, CView)

CCodeView::CCodeView()
   : CFormView(CCodeView::IDD)
{
   //{{AFX_DATA_INIT(CCodeView)
   //}}AFX_DATA_INIT

	m_comboCodeTypes= NULL;

	// Default code output type
	codeType= 0;

	// Create an AbstractModel translator
	amTranslator= new CShapeToAbstractModelCode ( );
  x3dTranslator= new CShapeToX3DCode ( );

	// Font to use for the edit box
	editFont= new CFont ();
	LOGFONT lf;                   // Used to create the CFont.

	memset(&lf, 0, sizeof(LOGFONT));   // Clear out structure.

	lf.lfHeight = 20;						// Request a 20-pixel-high font
	strcpy(lf.lfFaceName, "Courier New");	//    with face name "Courier New".
	editFont->CreateFontIndirect(&lf);		// Create the font.

	scaleFactor= 1.0;	// Default scale factor

	// Move model to origin before output? Not by default
	toOrigin= false;
}

CCodeView::~CCodeView()
{
	// The AbstractModel translator
	delete amTranslator;
  delete x3dTranslator;

	// Font to use for the edit box
	delete editFont;
}

// Get a translation so that the document is centered on the origin
void CCodeView::GetToOriginVector ( float* vect )
{
	CProtoHapticDoc* doc= (CProtoHapticDoc*)m_doc;

	// Create a composite of all objects in the scene
	CComposite* comp= new CComposite ( );

	// Add all objects
	for ( int i= 0; i < doc->GetShapeCount (); i++ )
	{
		comp->addShape ( doc->GetShape ( i )->clone () );
	}

	// translation is 0 - the composites centre
	vect[0]= -comp->getLocationX ();
	vect[1]= -comp->getLocationY ();
	vect[2]= -comp->getLocationZ ();

	// Delete composite and cloned members
	delete comp;
}

BEGIN_MESSAGE_MAP(CCodeView, CView)
	//{{AFX_MSG_MAP(CCodeView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON1, OnSaveCode)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CCodeView::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON2, &CCodeView::OnBnClickedButtonOptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/// Renders the scene set by SetDocument() as a sequence of OpenGL calls 
void CCodeView::OnDraw(CDC* pDC)
{
	// Get document
	CProtoHapticDoc* doc= (CProtoHapticDoc*)m_doc;

	// This is the string to hold the code generated
	CString s;

	switch ( codeType )
	{
	case 0:		// AbstractModel View Selected

		// Get translation to centre at origin
		float vectToOrigin[3];
		GetToOriginVector ( vectToOrigin );

		// If required to translate to origin
		if ( toOrigin )
		{
			// Translate to origin
			doc->TranslateModel ( vectToOrigin[0], vectToOrigin[1], vectToOrigin[2] );
		}

		// Temp change of model scale
		doc->ScaleModel ( scaleFactor );

		// Add Header Comments
		s= "// ProtoHaptic Code View\r\n// Output Type: AbstractModel API\r\n";
		s+= "// ==============================\r\n";

		// If document set
		if(doc!=NULL)
		{
			// Add Header Comments
			s+= "\r\n// AbstactModel Declaration Code:\r\n";
			s+= "// ------------------------------\r\n";

			// Generate declaration code for each shape
			for(int i= 0; i<doc->GetShapeCount(); i++)
			{
				// Create a unique numeric identifier for the shape
				//CString id;
				//id.Format ( "shape%d", i );

				// Get init code for shape
				CString code;
				amTranslator->GetDeclareCode ( doc->GetShape(i), &code, &(doc->GetShape(i)->GetName()), false );

				// Add a new line to the code
				s= s + code + "\r\n";
			}

			// Add Header Comments
			s+= "\r\n// AbstactModel Initialization Code:\r\n";
			s+= "// ---------------------------------\r\n";

			// Generate init code for each shape
			for( int i= 0; i<doc->GetShapeCount(); i++)
			{
				// Create a unique numeric identifier for the shape
				//CString id;
				//id.Format ( "shape%d", i );

				// Get init code for shape
				CString code;
				amTranslator->GetInitCode ( doc->GetShape(i), &code, &(doc->GetShape(i)->GetName()), false );

				// Add a new line to the code
				s= s + code + "\r\n";
			}

			// Assign the generated string to the edit box
			CWnd *p= NULL;
			p = GetDlgItem(IDC_EDIT1);
			((CEdit*)p)->SetWindowText(s);
		}
		// Revert change of model scale
		doc->ScaleModel ( 1.0f / scaleFactor );

		// If translated to origin
		if ( toOrigin )
		{
			// Translate back from origin
			doc->TranslateModel ( -vectToOrigin[0], -vectToOrigin[1], -vectToOrigin[2] );
		}

		break;
	case 1:		// OpenGL Code View Selected

		s= "// ProtoHaptic Code View\r\n// OpenGL Rendering Code:\r\n\r\n";

		if(doc!=NULL) {
			for(int i= 0; i<doc->GetShapeCount(); i++)
			{
				s= s + doc->GetShape(i)->drawGLCode() + "\r\n";
			}

			CWnd *p= NULL;

			p = GetDlgItem(IDC_EDIT1);

			((CEdit*)p)->SetWindowText(s);
		}

		break;

  case 2:   // X3D Code View Selected

    // Add Header Comments
		s= "<!-- ProtoHaptic Code View\r\n// Output Type: X3D\r\n";
		s+= "// ============================== -->\r\n";

    // If document set
		if(doc!=NULL)
		{

			// Generate declaration code for each shape
			for(int i= 0; i<doc->GetShapeCount(); i++)
			{
				// Create a unique numeric identifier for the shape
				//CString id;
				//id.Format ( "shape%d", i );

				// Get init code for shape
				CString code;
				x3dTranslator->GetDeclareCode ( doc->GetShape(i), &code, &(doc->GetShape(i)->GetName()), false );

				// Add a new line to the code
				s= s + code + "\r\n";
			}

			// Assign the generated string to the edit box
			CWnd *p= NULL;
			p = GetDlgItem(IDC_EDIT1);
			((CEdit*)p)->SetWindowText(s);
		}

    break;
	}

	// Use the font to paint a control.
	GetDlgItem(IDC_EDIT1)->SetFont(editFont, TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CCodeView diagnostics

#ifdef _DEBUG
void CCodeView::AssertValid() const
{
	CView::AssertValid();
}

void CCodeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCodeView message handlers


/// Resizes controls in the dialog resource, namely the CEdit box
void CCodeView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

    CRect l_formRect; 

    GetClientRect(&l_formRect);

	CWnd *p= NULL;

    p = GetDlgItem(IDC_EDIT1);
	
	if(p!=NULL) {
		p->MoveWindow(l_formRect.TopLeft().x, l_formRect.TopLeft().y,
					  l_formRect.Width(),     l_formRect.Height()-40, TRUE);

		p->RedrawWindow();
	}

	p= NULL;
    p = GetDlgItem(IDC_BUTTON1);
	
	if(p!=NULL) {
		p->MoveWindow(l_formRect.TopLeft().x+l_formRect.Width()-150, l_formRect.TopLeft().y+l_formRect.Height()-40,
					  150,     40, TRUE);

		p->RedrawWindow();
	}

	// Re-position the label "Select Code Output Type"
	CWnd* outputLabel= NULL;
    outputLabel = GetDlgItem(IDC_STATIC);
	
	// padding between components
	int padding[2]= { 10, 10 };

	if(outputLabel!=NULL) {
		outputLabel->MoveWindow(l_formRect.TopLeft().x + padding[0], l_formRect.TopLeft().y+l_formRect.Height()-40 + padding[1] + 3,
					             150,     40, TRUE);

		outputLabel->RedrawWindow();
	}

	// Re-position the combo box "Select Code Output Type"
	CWnd* outputCombo= NULL;
	outputCombo = GetDlgItem(IDC_COMBO1);
	
	// Get size of text label
	CRect textRect= NULL;

	if(outputLabel!=NULL)
	{
		outputLabel->GetWindowRect ( &textRect );
	}

	if(outputCombo!=NULL && outputLabel!=NULL)
	{
		outputCombo->MoveWindow(l_formRect.TopLeft().x + textRect.Width() + padding[0]*2, l_formRect.TopLeft().y+l_formRect.Height()-40+ padding[1],
					            150,     200, TRUE);

		outputCombo->RedrawWindow();
	}

	// Re-position the button "Output Options"
	CWnd* options= NULL;
	options = GetDlgItem(IDC_BUTTON2);
	
	// Get size of combo box
	CRect comboRect= NULL;

	if(outputCombo!=NULL)
	{
		outputCombo->GetWindowRect ( &textRect );
	}

	if(outputCombo!=NULL && options!=NULL && outputLabel!=NULL)
	{
		options->MoveWindow(l_formRect.TopLeft().x + 300 + padding[0]*3, l_formRect.TopLeft().y+l_formRect.Height()-40+ padding[1],
					        150, 25, TRUE);

		options->RedrawWindow();
	}

	// Save ptr to combo box
	if ( m_comboCodeTypes == NULL && outputCombo!=NULL )
	{
		m_comboCodeTypes= (CComboBox*)outputCombo;

		// Add options for code output
		m_comboCodeTypes->AddString ( "OpenGL" );
		m_comboCodeTypes->AddString ( "AbstractModel" );
    m_comboCodeTypes->AddString ( "X3D" );

		// Set a default option
		m_comboCodeTypes->SetCurSel ( 0 );	// Set to last string addeded
	}
}

void CCodeView::SetDocument(CDocument *d)
{
	m_doc= d;
}

int CCodeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	return 0;
}

void CCodeView::OnSaveCode() 
{
	// Write contents of CEdit to file

	// Max number of chars in a line
	static const int maxChars= 9999;

	// output file stream
	ofstream out_file;

	CString title= m_doc->GetTitle();

	title= title + "_CODE.c";

	out_file.open (title, ios::out );
	if(out_file.is_open()) {

		// Get ptr to the edit box
		CEdit *edit= NULL;
		edit= (CEdit*)GetDlgItem(IDC_EDIT1);
		
		if(edit!=NULL) 
		{
			// Current line
			char line[maxChars];

			// For each line in the edit box
			for ( int i= 0; i < edit->GetLineCount (); i++ )
			{
				// Read current line
				edit->GetLine ( i, line, maxChars );

				// Write current line to file
				out_file << line;
				out_file << "\n";	// remember eol
			}
		}

		::MessageBox(::GetFocus(),"Saved file: " + title,"OK",MB_OK);
		out_file.close();
	} else 
	{
		::MessageBox(::GetFocus(),"Can't create file: " + title,"Error",MB_OK);
	}
}

/// Handle change of code type in combo box
void CCodeView::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
	CWnd* outputCombo= NULL;
	outputCombo = GetDlgItem(IDC_COMBO1);

	// Get the selected index
	codeType= ((CComboBox*)outputCombo)->GetCurSel ();

	// Re-Draw the code view

	// Invalidate whole window, but don't erase first
	InvalidateRect ( NULL, false );	
	UpdateWindow ( );
}

// Output options button was clicked
void CCodeView::OnBnClickedButtonOptions()
{
	// TODO: Add your control notification handler code here
	CAMCodeOptions dlg;

	// Set values
	dlg.SetCentreAtOrigin ( toOrigin );

	// Display code options dialog
	INT_PTR ret= dlg.DoModal ();

	// if clicked OK
	if ( ret == IDOK )
	{
		// Get scale factor
		scaleFactor= dlg.GetScaleFactor ();

		// Get translate to origin option
		toOrigin= dlg.IsCentreAtOrigin ( );

		// Re-Draw the code view

		// Invalidate whole window, but don't erase first
		InvalidateRect ( NULL, false );	
		UpdateWindow ( );
	} // else do nothing
}
