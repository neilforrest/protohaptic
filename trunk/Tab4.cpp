// Tab4.cpp : implementation file
//

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "Tab4.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTab4 dialog


CTab4::CTab4(CWnd* pParent /*=NULL*/)
	: CDialog(CTab4::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTab4)
	//}}AFX_DATA_INIT
}


void CTab4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTab4)
	DDX_Control(pDX, IDC_EDIT1, m_mass);
	DDX_Control(pDX, IDC_PROXY_DYN, m_proxyDynamic);
	DDX_Control(pDX, IDC_CHECK3, m_collisionDynamic);
	DDX_Control(pDX, IDC_SLIDER1, m_frictionSlider);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTab4, CDialog)
	//{{AFX_MSG_MAP(CTab4)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit)
	ON_BN_CLICKED(IDC_PROXY_DYN, OnProxyDynamic)
	ON_BN_CLICKED(IDC_CHECK3, OnCollisionDynamic)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTab4 message handlers

void CTab4::setShape(CShape *s)
{
	m_shape= s;

	if(m_shape!=NULL)
	{
		m_mass.EnableWindow(true);
		m_proxyDynamic.EnableWindow(true);
		m_collisionDynamic.EnableWindow(true);
		m_frictionSlider.EnableWindow(true);
	} else {
		m_mass.SetWindowText("");
		m_proxyDynamic.SetCheck(0);
		m_collisionDynamic.SetCheck(0);
		m_frictionSlider.SetPos(0);

		m_mass.EnableWindow(false);
		m_proxyDynamic.EnableWindow(false);
		m_collisionDynamic.EnableWindow(false);
		m_frictionSlider.EnableWindow(false);
	}

	if(m_shape!=NULL) {
		char mass[50];

		CString s;
		float massf= 0.0;
		m_mass.GetWindowText(s);
		sscanf(s, "%f", &massf);
		if(massf!=m_shape->getMass()) {
			sprintf(mass, "%.0f", m_shape->getMass());
			m_mass.SetWindowText(mass);
		}

		if(m_shape->isProxyDynamic())
			m_proxyDynamic.SetCheck(1);
		else
			m_proxyDynamic.SetCheck(0);
		if(m_shape->isCollisionDynamic())
			m_collisionDynamic.SetCheck(1);
		else
			m_collisionDynamic.SetCheck(0);
		m_frictionSlider.SetPos((int)(m_shape->getSurfaceFriction()*100.0));
	}
}

void CTab4::OnPaint()
{
	CPaintDC dc(this); // device context for paintin

}

void CTab4::OnChangeEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_shape!=NULL) {
		CString s;
		float mass= 0.0;
		m_mass.GetWindowText(s);
		sscanf(s, "%f", &mass);
		m_shape->setMass(mass);
	}
}

void CTab4::OnProxyDynamic() 
{
	// TODO: Add your control notification handler code here
	if(m_shape!=NULL) {
		if(m_proxyDynamic.GetCheck())
			m_shape->setProxyDynamic(true);
		else
			m_shape->setProxyDynamic(false);
	}
}

void CTab4::OnCollisionDynamic() 
{
	// TODO: Add your control notification handler code here
	if(m_shape!=NULL) {
		if(m_collisionDynamic.GetCheck())
			m_shape->setCollisionDynamic(true);
		else
			m_shape->setCollisionDynamic(false);
	}
}

void CTab4::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_shape!=NULL) {
		m_shape->setSurfaceFriction((float)(m_frictionSlider.GetPos()/100.0));
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
