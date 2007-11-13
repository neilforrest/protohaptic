// AppOptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "AppOptionsDlg.h"
#include "PhantomsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAppOptionsDlg::CAppOptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAppOptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAppOptionsDlg)
	//}}AFX_DATA_INIT
}


void CAppOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAppOptionsDlg)
	DDX_Control(pDX, IDC_SLIDER4, m_stacks);
	DDX_Control(pDX, IDC_CHECK4, m_edit);
	DDX_Control(pDX, IDC_CHECK3, m_snap);
	DDX_Control(pDX, IDC_CHECK2, m_touch);
	DDX_Control(pDX, IDC_CHECK1, m_frontCheck);
	DDX_Control(pDX, IDC_CHECK6, m_sideCheck);
	DDX_Control(pDX, IDC_CHECK5, m_planCheck);
	DDX_Control(pDX, IDC_SLIDER3, m_editPointSnapDist);
	DDX_Control(pDX, IDC_SLIDER2, m_rotationGuess);
	DDX_Control(pDX, IDC_SLIDER1, m_editPointSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAppOptionsDlg, CDialog)
	//{{AFX_MSG_MAP(CAppOptionsDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnConfigDevices)
	ON_WM_HSCROLL()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_CHECK5, OnPlan)
	ON_BN_CLICKED(IDC_CHECK1, OnFront)
	ON_BN_CLICKED(IDC_CHECK6, OnSide)
	ON_BN_CLICKED(IDC_RADIO1, OnBeginner)
	ON_BN_CLICKED(IDC_RADIO2, OnIntermediate)
	ON_BN_CLICKED(IDC_RADIO3, OnExpert)
	ON_BN_CLICKED(IDC_CHECK2, OnTouch)
	ON_BN_CLICKED(IDC_CHECK4, OnSnapTo)
	ON_BN_CLICKED(IDC_CHECK3, OnEditPoint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppOptionsDlg message handlers

/// Display the configure devices dialog
void CAppOptionsDlg::OnConfigDevices() 
{
	CPhantomsDlg d;

	// Pass the current view to the dialog
	// so that it may alter it
	d.setView(m_view);

	d.DoModal();
}

/// Handles the HScroll messages from the sliders
void CAppOptionsDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// o Get positions of all sliders
	// o Set the options in the application
	
	((CProtoHapticApp*)AfxGetApp())->setEditPointSize((float)(m_editPointSize.GetPos()/500.0)+0.1);
	((CProtoHapticApp*)AfxGetApp())->setEditPointSnapDist((float)(m_editPointSnapDist.GetPos()/10.0)+0.1);
	((CProtoHapticApp*)AfxGetApp())->setRotationGuess(1.0f - (float)(m_rotationGuess.GetPos()/100.0));
	((CProtoHapticApp*)AfxGetApp())->setStacks( (int) ( ((float)m_stacks.GetPos()/4.0f) + 3 ) );

	char s[100];
	sprintf(s, "%d\n", m_stacks.GetPos());
	OutputDebugString(s);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

/// Called before painting: Updates slider positions
void CAppOptionsDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// o Read options from application
	// o Set slidder positions
	
	// Slidders
	m_editPointSize.SetPos((int)((((CProtoHapticApp*)AfxGetApp())->getEditPointSize()-0.1)*500.0));
	m_editPointSnapDist.SetPos((int)((((CProtoHapticApp*)AfxGetApp())->getEditPointSnapDist()-0.1)*10.0));
	m_rotationGuess.SetPos(100 - (int)((((CProtoHapticApp*)AfxGetApp())->getRotationGuess())*100.0));

	// Multiple Viewports
	m_planCheck.SetCheck(((CProtoHapticApp*)AfxGetApp())->isPlan());
	m_frontCheck.SetCheck(((CProtoHapticApp*)AfxGetApp())->isFrontElevation());
	m_sideCheck.SetCheck(((CProtoHapticApp*)AfxGetApp())->isElevation());

	// Audio Effects
	m_snap.SetCheck(((CProtoHapticApp*)AfxGetApp())->isAudioEditPoint());
	m_edit.SetCheck(((CProtoHapticApp*)AfxGetApp())->isAudioSnapTo());
	m_touch.SetCheck(((CProtoHapticApp*)AfxGetApp())->isAudioTouch());

	// Rendering
	m_stacks.SetPos( (int)((float)(((CProtoHapticApp*)AfxGetApp())->getStacks() - 3.0f) * 4.0f) );

	// Do not call CDialog::OnPaint() for painting messages
}

/// Called on 'save' button clicked
void CAppOptionsDlg::OnButtonSave() 
{
	// Ask application to save current options
	((CProtoHapticApp*)AfxGetApp())->SaveOptions();
}

/// Called on plan viewport check clicked
void CAppOptionsDlg::OnPlan() 
{
	// Invert option
	((CProtoHapticApp*)AfxGetApp())->setPlan(!((CProtoHapticApp*)AfxGetApp())->isPlan());
}

/// Called on front viewport check clicked
void CAppOptionsDlg::OnFront() 
{
	// Invert option
	((CProtoHapticApp*)AfxGetApp())->setFrontElevation(!((CProtoHapticApp*)AfxGetApp())->isFrontElevation());
}

/// Called on side viewport check clicked
void CAppOptionsDlg::OnSide() 
{
	// Invert option
	((CProtoHapticApp*)AfxGetApp())->setElevation(!((CProtoHapticApp*)AfxGetApp())->isElevation());
}

/// Load preset beginner settings
void CAppOptionsDlg::OnBeginner() 
{
	// Set sliders to preset positions
	m_editPointSize.SetPos(60);
	m_editPointSnapDist.SetPos(30);
	m_rotationGuess.SetPos(70);

	// Set applications options based on slider positions
	((CProtoHapticApp*)AfxGetApp())->setEditPointSize((float)(m_editPointSize.GetPos()/500.0)+0.1);
	((CProtoHapticApp*)AfxGetApp())->setEditPointSnapDist((float)(m_editPointSnapDist.GetPos()/10.0)+0.1);
	((CProtoHapticApp*)AfxGetApp())->setRotationGuess((float)(m_rotationGuess.GetPos()/100.0));
}

/// Load preset intermediate settings
void CAppOptionsDlg::OnIntermediate() 
{
	// Set sliders to preset positions
	m_editPointSize.SetPos(40);
	m_editPointSnapDist.SetPos(30);
	m_rotationGuess.SetPos(50);

	// Set applications options based on slider positions
	((CProtoHapticApp*)AfxGetApp())->setEditPointSize((float)(m_editPointSize.GetPos()/500.0)+0.1);
	((CProtoHapticApp*)AfxGetApp())->setEditPointSnapDist((float)(m_editPointSnapDist.GetPos()/10.0)+0.1);
	((CProtoHapticApp*)AfxGetApp())->setRotationGuess((float)(m_rotationGuess.GetPos()/100.0));	
}

/// Load preset expert settings
void CAppOptionsDlg::OnExpert() 
{
	// Set sliders to preset positions
	m_editPointSize.SetPos(30);
	m_editPointSnapDist.SetPos(20);
	m_rotationGuess.SetPos(20);

	// Set applications options based on slider positions
	((CProtoHapticApp*)AfxGetApp())->setEditPointSize((float)(m_editPointSize.GetPos()/500.0)+0.1);
	((CProtoHapticApp*)AfxGetApp())->setEditPointSnapDist((float)(m_editPointSnapDist.GetPos()/10.0)+0.1);
	((CProtoHapticApp*)AfxGetApp())->setRotationGuess((float)(m_rotationGuess.GetPos()/100.0));	
}

/// Set the view to act upon
void CAppOptionsDlg::setView(CView *view)
{
	m_view= view;
}

/// Called on audio touch check clicked
void CAppOptionsDlg::OnTouch() 
{
	// Invert option
	((CProtoHapticApp*)AfxGetApp())->setAudioTouch(!((CProtoHapticApp*)AfxGetApp())->isAudioTouch());
}

/// Called on audio snap check clicked
void CAppOptionsDlg::OnSnapTo()
{
	// Invert option
	((CProtoHapticApp*)AfxGetApp())->setAudioSnapTo(!((CProtoHapticApp*)AfxGetApp())->isAudioSnapTo());
	OnPaint();
}

/// Called on audio edit point check clicked
void CAppOptionsDlg::OnEditPoint() 
{
	// Invert option
	((CProtoHapticApp*)AfxGetApp())->setAudioEditPoint(!((CProtoHapticApp*)AfxGetApp())->isAudioEditPoint());
	OnPaint();
}

BOOL CAppOptionsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
