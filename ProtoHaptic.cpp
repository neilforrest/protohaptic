// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// This file is part of ProtoHaptic

// ProtoHaptic is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.

// ProtoHaptic is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with ProtoHaptic; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

// File: ProtoHaptic.cpp
// Classes: CProtoHapticApp, CAboutDlg
// Purpose: Define behaviour of ProtoHaptic application

#include "stdafx.h"
#include "ProtoHaptic.h"

#include "MainFrm.h"
#include "ProtoHapticDoc.h"
#include "ProtoHapticView.h"
#include "CodeView.h"
#include "ShapeDialog.h"
#include "AMPostImportDialog.h"
#include "GHOSTPreImportDlg.h"

// Import document from abstract model code
#include "AbstractModelCodeToDocument.h"
#include "GhostCodeToDocument.h"

#include <crtdbg.h>

#include <AFXPRIV.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProtoHapticApp

BEGIN_MESSAGE_MAP(CProtoHapticApp, CWinApp)
	//{{AFX_MSG_MAP(CProtoHapticApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_VIEW_CODE, OnViewCode)
	ON_COMMAND(ID_VIEW_GRAPHIC, OnViewGraphic)
//	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen )
	ON_COMMAND(ID_IMPORT_ABSTRACTMODELCODE, CProtoHapticApp::OnImportAbstractmodelcode)
	ON_COMMAND(ID_IMPORT_GHOSTCODE, &CProtoHapticApp::OnImportGhostcode)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProtoHapticApp construction

CProtoHapticApp::CProtoHapticApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CProtoHapticApp object

CProtoHapticApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CProtoHapticApp initialization

BOOL CProtoHapticApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CProtoHapticDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CProtoHapticView));
	AddDocTemplate(pDocTemplate);


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	CView* pActiveView = ((CFrameWnd*) m_pMainWnd)->GetActiveView();
	m_graphicalView = pActiveView;
	m_codeView = (CView*) new CCodeView();

	CDocument* pCurrentDoc = ((CFrameWnd*)m_pMainWnd)->GetActiveDocument();

	// Initialize a CCreateContext to point to the active document.
	// With this context, the new view is added to the document
	// when the view is created in CView::OnCreate().
	CCreateContext newContext;
	newContext.m_pNewViewClass = NULL;
	newContext.m_pNewDocTemplate = NULL;
	newContext.m_pLastView = NULL;
	newContext.m_pCurrentFrame = NULL;
	newContext.m_pCurrentDoc = pCurrentDoc;

	// The ID of the initial active view is AFX_IDW_PANE_FIRST.
	// Incrementing this value by one for additional views works
	// in the standard document/view case but the technique cannot
	// be extended for the CSplitterWnd case.
	UINT viewID = AFX_IDW_PANE_FIRST + 1;
	CRect rect(0, 0, 0, 0); // Gets resized later.

	// Create the new view. In this example, the view persists for
	// the life of the application. The application automatically
	// deletes the view when the application is closed.
	m_codeView->Create(NULL, "", WS_CHILD, rect, m_pMainWnd, viewID, &newContext);

	// When a document template creates a view, the WM_INITIALUPDATE
	// message is sent automatically. However, this code must
	// explicitly send the message, as follows.
	m_codeView->SendMessage(WM_INITIALUPDATE, 0, 0);

	((CProtoHapticView*)m_graphicalView)->SetCodeView(m_codeView);

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	CMenu* pMenu = AfxGetApp()->GetMainWnd()->GetMenu();
	pMenu->CheckMenuItem( ID_VIEW_EDITMODE, MF_CHECKED );
	pMenu->CheckMenuItem( ID_VIEW_VIEWMODE, MF_UNCHECKED );

	m_editPointSize= 0.1f;
	m_editPointSnapDist= 0.3f;
	m_rotationGuess= 0.5;

	m_planViewport= false;
	m_elevationViewport= false;
	m_frontElevation= false;
	m_primaryDevice= "Default PHANToM";
	m_secondaryDevice= "Not Configured";
	m_useDefault= true;
	m_passive= true;
	m_twoDevices= false;

	m_audioTouch= false;
	m_audioSnapTo= false;
	m_audioEditPoint= false;

	m_stacksSlices= 20;

	LoadOptions();

	//_CrtSetBreakAlloc ( 316 );

	return TRUE;
}

CView* CProtoHapticApp::SwitchView( CView* v )
{
   CView* pActiveView =
      ((CFrameWnd*) m_pMainWnd)->GetActiveView();

   CView* pNewView= v;

   // Exchange view window IDs so RecalcLayout() works.
   #ifndef _WIN32
   UINT temp = ::GetWindowWord(pActiveView->m_hWnd, GWW_ID);
   ::SetWindowWord(pActiveView->m_hWnd, GWW_ID, ::GetWindowWord(pNewView->m_hWnd, GWW_ID));
   ::SetWindowWord(pNewView->m_hWnd, GWW_ID, temp);
   #else
   UINT temp = ::GetWindowLong(pActiveView->m_hWnd, GWL_ID);
   ::SetWindowLong(pActiveView->m_hWnd, GWL_ID, ::GetWindowLong(pNewView->m_hWnd, GWL_ID));
   ::SetWindowLong(pNewView->m_hWnd, GWL_ID, temp);
   #endif

   pActiveView->ShowWindow(SW_HIDE);
   pNewView->ShowWindow(SW_SHOW);
   ((CFrameWnd*) m_pMainWnd)->SetActiveView(pNewView);
   ((CFrameWnd*) m_pMainWnd)->RecalcLayout();
   pNewView->Invalidate();

   return pActiveView;
} 

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CProtoHapticApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CProtoHapticApp message handlers

void CProtoHapticApp::OnEditCopy() 
{
	// TODO: Add your command handler code here
	
}

void CProtoHapticApp::setRotationGuess(float f)
{
	m_rotationGuess= f;
}

float CProtoHapticApp::getRotationGuess()
{
	return m_rotationGuess;
}

void CProtoHapticApp::setEditPointSize(float f)
{
	m_editPointSize= f;
}

float CProtoHapticApp::getEditPointSize()
{
	return m_editPointSize;
}

void CProtoHapticApp::setEditPointSnapDist(float f)
{
	m_editPointSnapDist= f;
}

float CProtoHapticApp::getEditPointSnapDist()
{
	return m_editPointSnapDist;
}

void CProtoHapticApp::Serialize(CArchive& ar) 
{
	if (ar.IsStoring())
	{	// storing code
		ar << m_rotationGuess;
		ar << m_editPointSize;
		ar << m_editPointSnapDist;
		if(m_planViewport) ar << 1; else ar << 0;
		if(m_elevationViewport) ar << 1; else ar << 0;
		if(m_frontElevation) ar << 1; else ar << 0;
		ar << m_primaryDevice;
		ar << m_secondaryDevice;
		if(m_useDefault) ar << 1; else ar << 0;
		if(m_passive) ar << 1; else ar << 0;
		if(m_twoDevices) ar << 1; else ar << 0;
		if(m_audioTouch) ar << 1; else ar << 0;
		if(m_audioEditPoint) ar << 1; else ar << 0;
		if(m_audioSnapTo) ar << 1; else ar << 0;
		ar << m_stacksSlices;
	}
	else
	{	// loading code
		ar >> m_rotationGuess;
		ar >> m_editPointSize;
		ar >> m_editPointSnapDist;
		int i;
		ar >> i;
		if(i==1) m_planViewport= true; else m_planViewport= false;
		ar >> i;
		if(i==1) m_elevationViewport= true; else m_elevationViewport= false;
		ar >> i;
		if(i==1) m_frontElevation= true; else m_frontElevation= false;
		ar >> m_primaryDevice;
		ar >> m_secondaryDevice;
		ar >> i;
		if(i==1) m_useDefault= true; else m_useDefault= false;
		ar >> i;
		if(i==1) m_passive= true; else m_passive= false;
		ar >> i;
		if(i==1) m_twoDevices= true; else m_twoDevices= false;
		ar >> i;
		if(i==1) m_audioTouch= true; else m_audioTouch= false;
		ar >> i;
		if(i==1) m_audioEditPoint= true; else m_audioEditPoint= false;
		ar >> i;
		if(i==1) m_audioSnapTo= true; else m_audioSnapTo= false;
		ar >> m_stacksSlices;
	}
}

char *pFileName= "settings.dat";

void CProtoHapticApp::SaveOptions()
{
   CFile f;

   if( f.Open( pFileName, CFile::modeCreate | CFile::modeWrite) )
   {
		CArchive ar( &f, CArchive::store );

		Serialize( ar );
   }
}

void CProtoHapticApp::LoadOptions()
{
   CFile f;

   if( f.Open( pFileName, CFile::modeRead) )
   {
		CArchive ar( &f, CArchive::load );

		Serialize( ar );
   }
}

bool CProtoHapticApp::isFrontElevation()
{
	return m_frontElevation;
}
void CProtoHapticApp::setFrontElevation(bool b)
{
	m_frontElevation= b;
}

bool CProtoHapticApp::isElevation()
{
	return m_elevationViewport;
}

void CProtoHapticApp::setElevation(bool b)
{
	m_elevationViewport= b;
}

bool CProtoHapticApp::isPlan()
{
	return m_planViewport;
}

void CProtoHapticApp::setPlan(bool b)
{
	m_planViewport= b;
}

CString CProtoHapticApp::getPrimaryDevice()
{
	return m_primaryDevice;
}

CString CProtoHapticApp::getSecondaryDevice()
{
	return m_secondaryDevice;
}

bool CProtoHapticApp::isUseDefault()
{
	return m_useDefault;
}

bool CProtoHapticApp::isPassive()
{
	return m_passive;
}

void CProtoHapticApp::setPrimaryDevice(CString s)
{
	m_primaryDevice= s;
}

void CProtoHapticApp::setSecondaryDevice(CString s)
{
	m_secondaryDevice= s;
}

void CProtoHapticApp::setUseDefault(bool b)
{
	m_useDefault= b;
}

void CProtoHapticApp::setPassive(bool b)
{
	m_passive= b;
}

bool CProtoHapticApp::isTwoDevices()
{
	return m_twoDevices;
}

void CProtoHapticApp::setTwoDevices(bool b)
{
	m_twoDevices= b;
}

void CProtoHapticApp::setAudioTouch(bool b)
{
	m_audioTouch= b;
	OutputDebugString("Touch");
}

void CProtoHapticApp::setAudioEditPoint(bool b)
{
	m_audioEditPoint= b;
	OutputDebugString("Edit");
}

void CProtoHapticApp::setAudioSnapTo(bool b)
{
	m_audioSnapTo= b;
	OutputDebugString("Snap");
}

bool CProtoHapticApp::isAudioTouch()
{
	return m_audioTouch;
}

bool CProtoHapticApp::isAudioEditPoint()
{
	return m_audioEditPoint;
}

bool CProtoHapticApp::isAudioSnapTo()
{
	return m_audioSnapTo;
}

void CProtoHapticApp::OnViewCode() 
{
	// TODO: Add your command handler code here
	SwitchView(m_codeView);
}

void CProtoHapticApp::OnViewGraphic() 
{
	// TODO: Add your command handler code here
	SwitchView(m_graphicalView);
}

void CProtoHapticApp::setStacks(int s)
{
	m_stacksSlices= s;
}

int CProtoHapticApp::getStacks()
{
	return m_stacksSlices;
}

void CProtoHapticApp::OnImportAbstractmodelcode()
{
	// TODO: Add your command handler code here
	CFileDialog* dlg; // File chooser dialog
	CString fname;		// Filename
	
	// Create the dialog
	dlg= new CFileDialog (true, ".*", NULL, OFN_EXPLORER| OFN_OVERWRITEPROMPT,
						"All files(*.*)|*.*||", AfxGetMainWnd() );

	// Display the dialog 
	if (dlg->DoModal() == IDOK)	// If user clicked OK
	{
		// Get the filename requested
		fname= dlg->GetPathName ();
		
		// Create Abstract Model import translator
		CAbstractModelCodeToDocument* amTranslator= 
			new CAbstractModelCodeToDocument ( );

		// Get ptr to document and view
		CProtoHapticDoc* doc= (CProtoHapticDoc*)m_graphicalView->GetDocument();
		CProtoHapticView* view= (CProtoHapticView*)m_graphicalView;

		// If currewnt document contains shapes
		if ( doc->GetShapeCount () > 0 )
		{
			// Ask whether to clear current model first
			if ( MessageBox ( 0, "Would you like to clear the current model first?\n\nSelect Yes to clear the document first. (You may be asked to save any changes).\nSelect no to append the imported items to the current document.", "", MB_YESNO ) == IDYES )
			{
				OnFileNew ();	// Create empty document
			}
		}

		// Do conversion, create document
		amTranslator->BuildDocument ( fname, doc );
		
		// Re-Centre the camera view
		view->CenterCamera ( );

		// Done converting document

		// Display post-import document options
		CAMPostImportDialog dlg;

		// Set pointer to translator under control
		dlg.SetTranslator ( amTranslator );

		// Set ptr to document under control
		dlg.SetDocument ( doc );

		// Set ptr to view under control
		dlg.SetView ( view );

		// Display dialog

		INT_PTR ret= dlg.DoModal ();

		// Process return value
		if ( ret == IDOK )
		{
			// OK, close window
		}
		else
		{
			// Cancel, reset model scale factor to default
		}

		// Destroy import translator
		delete amTranslator;
	}
	// else, dialog close without IDOK, do nothing

	// Delete dialog
	delete dlg;
}
void CProtoHapticApp::OnImportGhostcode()
{
	// TODO: Add your command handler code here
	CFileDialog* dlg; // File chooser dialog
	CString fname;		// Filename
	
	// Create the dialog
	dlg= new CFileDialog (true, ".*", NULL, OFN_EXPLORER| OFN_OVERWRITEPROMPT,
						"All files(*.*)|*.*||", AfxGetMainWnd() );

	// Display the dialog 
	if (dlg->DoModal() == IDOK)	// If user clicked OK
	{
		// Get the filename requested
		fname= dlg->GetPathName ();
		
		// Create Ghost import translator
		CGhostCodeToDocument* ghostTranslator= 
			new CGhostCodeToDocument ( );

		// Get ptr to document and view
		CProtoHapticDoc* doc= (CProtoHapticDoc*)m_graphicalView->GetDocument();
		CProtoHapticView* view= (CProtoHapticView*)m_graphicalView;

		// If currewnt document contains shapes
		if ( doc->GetShapeCount () > 0 )
		{
			// Ask whether to clear current model first
			if ( MessageBox ( 0, "Would you like to clear the current model first?\n\nSelect Yes to clear the document first. (You may be asked to save any changes).\nSelect no to append the imported items to the current document.", "", MB_YESNO ) == IDYES )
			{
				OnFileNew ();	// Create empty document
			}
		}

		// Get pre-import options
		CGHOSTPreImportDlg preDlg;
		if ( preDlg.DoModal () != IDOK )
		{
			// User cancelled import
			return;
		}

		// Use a root separator
		if ( preDlg.m_useSeparator == 1 )
		{
			// Set root separator name
			ghostTranslator->SetRootSeparatorName ( preDlg.GetRootSepName () );
		}

		// Do conversion, create document
		ghostTranslator->BuildDocument ( fname, doc );
		
		// Re-Centre the camera view
		view->CenterCamera ( );

		// Done converting document

		//// Display post-import document options
		CAMPostImportDialog dlg;

		//// Set pointer to translator under control
		dlg.SetTranslator ( ghostTranslator );

		//// Set ptr to document under control
		dlg.SetDocument ( doc );

		//// Set ptr to view under control
		dlg.SetView ( view );

		// Display dialog

		INT_PTR ret= dlg.DoModal ();

		// Process return value
		if ( ret == IDOK )
		{
			// OK, close window
		}
		else
		{
			// Cancel, reset model scale factor to default
		}

		//// Destroy import translator
		delete ghostTranslator;
	}
	// else, dialog close without IDOK, do nothing

	// Delete dialog
	delete dlg;
}
