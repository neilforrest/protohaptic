// DocumentOptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "DocumentOptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDocumentOptionsDlg dialog


CDocumentOptionsDlg::CDocumentOptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDocumentOptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDocumentOptionsDlg)
	//}}AFX_DATA_INIT
}


void CDocumentOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDocumentOptionsDlg)
	DDX_Control(pDX, IDC_CHECK1, m_hideMemos);
	DDX_Control(pDX, IDC_SLIDER1, m_gravitySlider);
	DDX_Control(pDX, IDC_SLIDER2, m_simSpeedSlider);
	DDX_Control(pDX, IDC_SLIDER3, m_airResistanceSlider);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK2, m_enableSurfaceChange);
}


BEGIN_MESSAGE_MAP(CDocumentOptionsDlg, CDialog)
	//{{AFX_MSG_MAP(CDocumentOptionsDlg)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CHECK1, OnHideMemos)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK2, &CDocumentOptionsDlg::OnBnClickedCheck2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDocumentOptionsDlg message handlers

void CDocumentOptionsDlg::setDocument(CProtoHapticDoc *d)
{
	m_document= d;
}

void CDocumentOptionsDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	m_gravitySlider.SetPos((int)(m_document->getGravity()*100.0));
	m_simSpeedSlider.SetPos((int)(m_document->getSimSpeed()*100.0));
	m_airResistanceSlider.SetPos((int)(m_document->getAirResistance()*100.0));

	m_hideMemos.SetCheck(m_document->isHideMemos());

	m_enableSurfaceChange.SetCheck ( m_document->IsDynamicSurfaceChangeEnabled ( ) );

	// Do not call CDialog::OnPaint() for painting messages
}

void CDocumentOptionsDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	m_document->setGravity(((float)m_gravitySlider.GetPos())/100.0f);
	m_document->setSimSpeed(((float)m_simSpeedSlider.GetPos())/100.0f);
	m_document->setAirResistance(((float)m_airResistanceSlider.GetPos())/100.0f);


	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDocumentOptionsDlg::OnHideMemos() 
{
	// TODO: Add your control notification handler code here
	m_document->setHideMemos(m_hideMemos.GetCheck());
}

void CDocumentOptionsDlg::OnBnClickedCheck2()
{
	// TODO: Add your control notification handler code here
	m_document->EnableDynamicSurfaceChange ( m_enableSurfaceChange.GetCheck () );
}
