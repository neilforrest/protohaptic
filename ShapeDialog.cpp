// ShapeDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "ShapeDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShapeDialog dialog


CShapeDialog::CShapeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CShapeDialog::IDD, pParent)
{
	m_visable= false;
	//{{AFX_DATA_INIT(CShapeDialog)
	//}}AFX_DATA_INIT
}


void CShapeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShapeDialog)
	DDX_Control(pDX, IDC_SLIDER4, m_dynamic_friction);
	DDX_Control(pDX, IDC_SLIDER3, m_static_friction);
	DDX_Control(pDX, IDC_SLIDER2, m_dampening);
	DDX_Control(pDX, IDC_SLIDER1, m_stiffness);
	DDX_Control(pDX, IDC_SLIDER7, m_sliderBlue);
	DDX_Control(pDX, IDC_SLIDER6, m_sliderGreen);
	DDX_Control(pDX, IDC_SLIDER5, m_sliderRed);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShapeDialog, CDialog)
	//{{AFX_MSG_MAP(CShapeDialog)
	ON_WM_CREATE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP

	ON_MESSAGE(WM_HSCROLL, OnHScrollSlider)

END_MESSAGE_MAP()

afx_msg LRESULT OnHScrollSlider(WPARAM wParam, LPARAM lParam);

/////////////////////////////////////////////////////////////////////////////
// CShapeDialog message handlers


LRESULT CShapeDialog::OnHScrollSlider(WPARAM wParam, LPARAM lParam)
{
    HWND hwnd;
    hwnd = (HWND)lParam;    


	if(m_shape!=NULL) {

		if(m_sliderRed.m_hWnd   == hwnd||
		   m_sliderGreen.m_hWnd == hwnd||
		   m_sliderBlue.m_hWnd  == hwnd)
		{
			m_shape->setColour(m_sliderRed.GetPos()/100.0,
							   m_sliderGreen.GetPos()/100.0,
							   m_sliderBlue.GetPos()/100.0);
		}

		m_shape->setStiffness(m_stiffness.GetPos()/100.0);
		m_shape->setDampening(m_dampening.GetPos()/100.0);
		m_shape->setStaticFriction(m_static_friction.GetPos()/100.0);
		m_shape->setDynamicFriction(m_dynamic_friction.GetPos()/100.0);
	}

	return 0;
}

void CShapeDialog::SetShape(CShape* shape)
{
	if(m_shape!=shape) {
		m_shape= shape;
		OnPaint();
	}
}	

int CShapeDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	


	return 0;
}

void CShapeDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if(m_shape!=NULL) {
		m_sliderRed.EnableWindow(true);
		m_sliderGreen.EnableWindow(true);
		m_sliderBlue.EnableWindow(true);

		m_stiffness.EnableWindow(true);
		m_dampening.EnableWindow(true);
		m_static_friction.EnableWindow(true);
		m_dynamic_friction.EnableWindow(true);

		m_sliderRed.SetPos((int)(m_shape->getRed()*100.0));
		m_sliderGreen.SetPos((int)(m_shape->getGreen()*100.0));
		m_sliderBlue.SetPos((int)(m_shape->getBlue()*100.0));

		m_stiffness.SetPos((int)(m_shape->getStiffness()*100.0));
		m_dampening.SetPos((int)(m_shape->getDampening()*100.0));
		m_static_friction.SetPos((int)(m_shape->getStaticFriction()*100.0));
		m_dynamic_friction.SetPos((int)(m_shape->getDynamicFriction()*100.0));
	} else {
		m_sliderRed.EnableWindow(false);
		m_sliderGreen.EnableWindow(false);
		m_sliderBlue.EnableWindow(false);

		m_stiffness.EnableWindow(false);
		m_dampening.EnableWindow(false);
		m_static_friction.EnableWindow(false);
		m_dynamic_friction.EnableWindow(false);
	}
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CShapeDialog::isVisable()
{
	return m_visable;	
}

void CShapeDialog::setVisable(BOOL visable)
{
	m_visable= visable;
}
