// PhantomsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "PhantomsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhantomsDlg dialog


CPhantomsDlg::CPhantomsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPhantomsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPhantomsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPhantomsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPhantomsDlg)
	DDX_Control(pDX, IDC_RADIO4, m_active);
	DDX_Control(pDX, IDC_RADIO5, m_passive);
	DDX_Control(pDX, IDC_EDIT2, m_secondary);
	DDX_Control(pDX, IDC_EDIT1, m_primary);
	DDX_Control(pDX, IDC_CHECK3, m_enable);
	DDX_Control(pDX, IDC_CHECK1, m_default);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPhantomsDlg, CDialog)
	//{{AFX_MSG_MAP(CPhantomsDlg)
	ON_BN_CLICKED(IDC_RADIO5, OnPassive)
	ON_BN_CLICKED(IDC_RADIO4, OnActive)
	ON_BN_CLICKED(IDC_BUTTON1, OnApply)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhantomsDlg message handlers

void CPhantomsDlg::OnPassive() 
{
	// TODO: Add your control notification handler code here
	
}

void CPhantomsDlg::OnActive() 
{
	// TODO: Add your control notification handler code here
	
}

void CPhantomsDlg::OnApply() 
{
	// TODO: Add your control notification handler code here

	m_view->Pause();

	((CProtoHapticApp*)AfxGetApp())->setPassive(m_passive.GetCheck());
	((CProtoHapticApp*)AfxGetApp())->setUseDefault(m_default.GetCheck());
	((CProtoHapticApp*)AfxGetApp())->setTwoDevices(m_enable.GetCheck());
	CString s;
	m_primary.GetWindowText(s);
	((CProtoHapticApp*)AfxGetApp())->setPrimaryDevice(s);
	m_secondary.GetWindowText(s);
	((CProtoHapticApp*)AfxGetApp())->setSecondaryDevice(s);

	m_view->DeInitHaptics();

	m_view->Play();

	UpdateControls();

	m_view->m_deselect= true;
	m_view->m_deselect1= true;

	::MessageBox(::GetFocus(),"Configuration Complete: Remember to save your changes.","Done",MB_OK);
}

int CPhantomsDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here



	return 0;
}

void CPhantomsDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	m_primary.EnableWindow(!m_default.GetCheck());
	m_secondary.EnableWindow(m_enable.GetCheck());
	m_passive.EnableWindow(m_enable.GetCheck() );
	m_active.EnableWindow(m_enable.GetCheck() );

	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CPhantomsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	UpdateControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPhantomsDlg::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	OnPaint();
}

void CPhantomsDlg::OnCheck3() 
{
	// TODO: Add your control notification handler code here
	OnPaint();
}

void CPhantomsDlg::setView(CView *view)
{
	m_view= (CProtoHapticView*)view;
}

void CPhantomsDlg::UpdateControls()
{
	m_primary.SetWindowText(((CProtoHapticApp*)AfxGetApp())->getPrimaryDevice());
	m_secondary.SetWindowText(((CProtoHapticApp*)AfxGetApp())->getSecondaryDevice());

	m_enable.SetCheck(((CProtoHapticApp*)AfxGetApp())->isTwoDevices());
	m_default.SetCheck(((CProtoHapticApp*)AfxGetApp())->isUseDefault());
	m_passive.SetCheck(((CProtoHapticApp*)AfxGetApp())->isPassive());
	m_active.SetCheck(!((CProtoHapticApp*)AfxGetApp())->isPassive());
}
