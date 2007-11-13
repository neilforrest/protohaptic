// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Tab1.cpp
// Classes: CTab1
// Purpose: A tab in the shape properties dialog

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "Tab1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTab1 dialog


CTab1::CTab1(CWnd* pParent /*=NULL*/)
	: CDialog(CTab1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTab1)
	//}}AFX_DATA_INIT
}


void CTab1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTab1)
	DDX_Control(pDX, IDC_SLIDER3, m_sliderBlue);
	DDX_Control(pDX, IDC_SLIDER2, m_sliderGreen);
	DDX_Control(pDX, IDC_SLIDER1, m_sliderRed);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTab1, CDialog)
	//{{AFX_MSG_MAP(CTab1)
	ON_WM_HSCROLL()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTab1 message handlers

void CTab1::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	m_shape->setColour((float)(m_sliderRed.GetPos()/100.0),
					   (float)(m_sliderGreen.GetPos()/100.0),
					   (float)(m_sliderBlue.GetPos()/100.0) );
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CTab1::setShape(CShape *s)
{
	m_shape= s;
	OnPaint();
}

void CTab1::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if(m_shape!=NULL) {
		m_sliderRed.EnableWindow(true);
		m_sliderGreen.EnableWindow(true);
		m_sliderBlue.EnableWindow(true);
		m_sliderRed.SetPos((int)(m_shape->getRed()*100));
		m_sliderGreen.SetPos((int)(m_shape->getGreen()*100));
		m_sliderBlue.SetPos((int)(m_shape->getBlue()*100));

	} else {
		m_sliderRed.EnableWindow(false);
		m_sliderGreen.EnableWindow(false);
		m_sliderBlue.EnableWindow(false);
		m_sliderRed.SetPos(0);
		m_sliderGreen.SetPos(0);
		m_sliderBlue.SetPos(0);
	}
	// Do not call CDialog::OnPaint() for painting messages
}
