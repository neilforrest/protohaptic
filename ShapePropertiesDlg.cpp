// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: ShapePropertiesDlg.cpp
// Classes: CShapePropertiesDlg
// Purpose: Represents the shape properties dialog box

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "ShapePropertiesDlg.h"
#include "Tab1.h"
#include "Tab2.h"
#include "Tab3.h"
#include "Tab4.h"
#include "Tab5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShapePropertiesDlg dialog


CShapePropertiesDlg::CShapePropertiesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShapePropertiesDlg::IDD, pParent)
{
	setVisable(false);
	m_shape= NULL;
	m_doc= NULL;
	//{{AFX_DATA_INIT(CShapePropertiesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

/** Set the document in which the specified shape resides */
void CShapePropertiesDlg::SetDocument ( CProtoHapticDoc* doc )
{
	m_doc= doc;

	((CTab3*)m_tabCtrl.getPane(3))->SetDocument(doc);
	((CTab5*)m_tabCtrl.getPane(4))->SetDocument(doc);
}

void CShapePropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShapePropertiesDlg)
	DDX_Control(pDX, IDC_TAB1, m_tabCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShapePropertiesDlg, CDialog)
	//{{AFX_MSG_MAP(CShapePropertiesDlg)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CLOSESHAPES, OnCloseShapes)
	ON_WM_SHOWWINDOW()
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShapePropertiesDlg message handlers

BOOL CShapePropertiesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_tabCtrl.InitDialogs(); 
	m_tabCtrl.InsertItem(0,"Visual");
	m_tabCtrl.InsertItem(1,"Haptic");
	m_tabCtrl.InsertItem(2,"Dynamics");
	m_tabCtrl.InsertItem(3,"Model");
	m_tabCtrl.InsertItem(4,"Code Generation");

	m_tabCtrl.ActivateTabDialogs();

	// TODO: Add extra initialization here
//	SetWindowPos(&this->wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



BOOL CShapePropertiesDlg::isVisable()
{
	return m_visable;
}

void CShapePropertiesDlg::setVisable(BOOL visable)
{
	m_visable= visable;
}

void CShapePropertiesDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	// Do not call CDialog::OnPaint() for painting messages
}

void CShapePropertiesDlg::setShape(CShape *shape)
{
	//if(shape!=m_shape) {
		m_shape= shape;
		((CTab1*)m_tabCtrl.getPane(0))->setShape(shape);
		((CTab2*)m_tabCtrl.getPane(1))->setShape(shape);
		((CTab4*)m_tabCtrl.getPane(2))->setShape(shape);
		((CTab3*)m_tabCtrl.getPane(3))->setShape(shape);
		((CTab5*)m_tabCtrl.getPane(4))->setShape(shape);
	//}
}

void CShapePropertiesDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CShapePropertiesDlg::OnCloseShapes() 
{
	// TODO: Add your control notification handler code here
	ShowWindow(0);
}

void CShapePropertiesDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	setVisable(bShow);
	
}

void CShapePropertiesDlg::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}
