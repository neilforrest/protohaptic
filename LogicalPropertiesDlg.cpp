// LogicalPropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "LogicalPropertiesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogicalPropertiesDlg dialog


CLogicalPropertiesDlg::CLogicalPropertiesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogicalPropertiesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogicalPropertiesDlg)
	//}}AFX_DATA_INIT
	m_shape= NULL;
}


void CLogicalPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogicalPropertiesDlg)
	DDX_Control(pDX, IDC_EDIT3, m_static);
	DDX_Control(pDX, IDC_EDIT6, m_dynamic);
	DDX_Control(pDX, IDC_EDIT2, m_dampening);
	DDX_Control(pDX, IDC_EDIT1, m_stiffness);
	DDX_Control(pDX, IDC_ENABLE_LP, m_enable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogicalPropertiesDlg, CDialog)
	//{{AFX_MSG_MAP(CLogicalPropertiesDlg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_ENABLE_LP, OnEnable)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogicalPropertiesDlg message handlers

void CLogicalPropertiesDlg::setShape(CShape *s)
{
	m_shape= s;
}

void CLogicalPropertiesDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	if(m_shape==NULL) {
		m_dynamic.EnableWindow(false);
		m_dampening.EnableWindow(false);
		m_stiffness.EnableWindow(false);
		m_static.EnableWindow(false);
		m_enable.EnableWindow(false);
	} else {
		m_enable.EnableWindow(true);
		m_enable.SetCheck(m_shape->isLogical());
		if(m_shape->isLogical()) {
			m_dynamic.EnableWindow(true);
			m_dampening.EnableWindow(true);
			m_stiffness.EnableWindow(true);
			m_static.EnableWindow(true);

			m_dynamic.SetWindowText(m_shape->getDynamicFrictionFormula());
			m_dampening.SetWindowText(m_shape->getDampeningFormula());
			m_stiffness.SetWindowText(m_shape->getStiffnessFormula());
			m_static.SetWindowText(m_shape->getStaticFrictionFormula());
		} else {
			m_dynamic.EnableWindow(false);
			m_dampening.EnableWindow(false);
			m_stiffness.EnableWindow(false);
			m_static.EnableWindow(false);
		}
	}

	// Do not call CDialog::OnPaint() for painting messages
}

void CLogicalPropertiesDlg::OnEnable() 
{
	// TODO: Add your control notification handler code here
	if(m_shape!=NULL) {
		m_shape->setLogical(m_enable.GetCheck());
		OnPaint();
	}
}

void CLogicalPropertiesDlg::OnApply() 
{
	// TODO: Add your control notification handler code here
	if(m_shape!=NULL) {
		CString s;

		m_dynamic.GetWindowText(s);
		m_shape->setDynamicFrictionFormula(s);

		m_dampening.GetWindowText(s);
		m_shape->setDampeningFormula(s);

		m_stiffness.GetWindowText(s);
		m_shape->setStiffnessFormula(s);

		m_static.GetWindowText(s);
		m_shape->setStaticFrictionFormula(s);

		if(!m_shape->EvaluateLogical())
			::MessageBox(::GetFocus(),"Syntax or Result Error:\nFunction must evaluate in the range 0 to 1 and be correctly formed.","Error",MB_OK);

	}
}

BOOL CLogicalPropertiesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetWindowPos(&this->wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
