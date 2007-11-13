// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Tab1.cpp
// Classes: CTab1
// Purpose: A tab in the shape properties dialog

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "Tab2.h"
#include "LogicalPropertiesDlg.h"
#include "Composite.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTab2 dialog


CTab2::CTab2(CWnd* pParent /*=NULL*/)
	: CDialog(CTab2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTab2)
	//}}AFX_DATA_INIT
	dlg.Create(IDD_FORMULAS);
}


void CTab2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTab2)
	DDX_Control(pDX, IDC_SLIDER5, m_logicalSlider);
	DDX_Control(pDX, IDC_SLIDER4, m_dynamic_friction);
	DDX_Control(pDX, IDC_SLIDER3, m_static_friction);
	DDX_Control(pDX, IDC_SLIDER2, m_dampening);
	DDX_Control(pDX, IDC_SLIDER1, m_stiffness);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_STIFFNESS1, m_stiffness1);
	DDX_Control(pDX, IDC_EDIT_DAMPING1, m_damping1);
	DDX_Control(pDX, IDC_EDIT_STATIC1, m_static1);
	DDX_Control(pDX, IDC_EDIT_DYNAMIC1, m_dynamic1);
	DDX_Control(pDX, IDC_CHECK1, m_controlMembers);
	DDX_Control(pDX, IDC_CHECK2, m_customRenderCheck);
}


BEGIN_MESSAGE_MAP(CTab2, CDialog)
	//{{AFX_MSG_MAP(CTab2)
	ON_WM_HSCROLL()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_FORMULA, OnFormula)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_EDIT_STIFFNESS1, &CTab2::OnEnChangeEditStiffness1)
	ON_EN_CHANGE(IDC_EDIT_DAMPING1, &CTab2::OnEnChangeEditDamping1)
	ON_EN_CHANGE(IDC_EDIT_STATIC1, &CTab2::OnEnChangeEditStatic1)
	ON_EN_CHANGE(IDC_EDIT_DYNAMIC1, &CTab2::OnEnChangeEditDynamic1)
	ON_BN_CLICKED(IDC_CHECK1, &CTab2::OnBnClickedControlMembers)
	ON_BN_CLICKED(IDC_CHECK2, &CTab2::OnBnClickedCheck2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTab2 message handlers

void CTab2::setShape(CShape *s)
{
	// If shape has changed, update sliders
	if ( s != m_shape )
	{
		m_shape= s;
		dlg.setShape(m_shape);
		OnPaint();
		UpdateEditBoxes ();
	}
}

void CTab2::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default

		m_shape->setStiffness((float)(m_stiffness.GetPos()/100.0));
		m_shape->setDampening((float)(m_dampening.GetPos()/100.0));
		m_shape->setStaticFriction((float)(m_static_friction.GetPos()/100.0));
		m_shape->setDynamicFriction((float)(m_dynamic_friction.GetPos()/100.0));
		m_shape->setLogicalValue((float)(m_logicalSlider.GetPos()/100.0));

		if(m_shape->isLogical())
			m_shape->EvaluateLogical();

	UpdateEditBoxes ( );

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

// Update the edit boxes from the sliders
void CTab2::UpdateEditBoxes ( )
{
	// If a shape is set
	if ( m_shape != NULL )
	{
		// Set text boxes
		char str[256];

		sprintf_s ( str, 256, "%.4f", m_shape->getStiffness () );
		m_stiffness1.SetWindowText ( str );
		sprintf_s ( str, 256, "%.4f", m_shape->getDampening () );
		m_damping1.SetWindowText ( str );
		sprintf_s ( str, 256, "%.4f", m_shape->getStaticFriction () );
		m_static1.SetWindowText ( str );
		sprintf_s ( str, 256, "%.4f", m_shape->getDynamicFriction () );
		m_dynamic1.SetWindowText ( str );
	}
}

// Convert string to float
bool CTab2::StringToFloat ( char* str, float* flt )
{
	// Count the number of digits before the d.p.
	int prefixCount= 0;

	// Length of whole string
	int strLength= strlen ( str );

	// Initialise answer to 0
	(*flt)= 0;

	// For each char in str
	for ( int i= 0; i < strLength; i++ )
	{
		// If char is not the d.p.
		if ( str[i] != '.' )
		{
			// Icr count
			prefixCount++;
		}
		else
		{
			// char is d.p. finnished counting
			break;
		}
	}

	// Encountered dp. yet?
	bool decimal= false;

	// Current power of 10
	int power= prefixCount-1;

	// For each char in str
	for ( int i= 0; i < strLength; i++ )
	{
		// If char is valid
		if ( isdigit ( str[i] ) || // Char is a number, OR
			((!decimal)&&(str[i]=='.')) )// Decimal not encountered AND char is decimal
		{
			// If char is decimal, record this
			if ( str[i] == '.' )
			{
				decimal= true;
			}
			else	// char is a digit
			{
				// Int value of char
				int digit= str[i] - '0';

				// Add to the answer
				(*flt)+= (float) ( ((float)digit) * pow ( (double)10, power ) );

				// decriment the power of 10
				power--;
			}
		}
		else // invalid char
		{
			return false;
		}
	}

	// Success
	return true;
}

void CTab2::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(m_shape!=NULL) {
		m_stiffness.EnableWindow(true);
		m_dampening.EnableWindow(true);
		m_static_friction.EnableWindow(true);
		m_dynamic_friction.EnableWindow(true);

		m_controlMembers.EnableWindow ( m_shape->getType () == SHAPE_COMPOSITE );
		m_customRenderCheck.EnableWindow ( true );

		if(m_shape->isLogical()) {
			m_logicalSlider.EnableWindow(true);
			m_stiffness.EnableWindow(false);
			m_dampening.EnableWindow(false);
			m_static_friction.EnableWindow(false);
			m_dynamic_friction.EnableWindow(false);

			m_stiffness1.EnableWindow ( false );
			m_damping1.EnableWindow ( false );
			m_static1.EnableWindow ( false );
			m_dynamic1.EnableWindow ( false );

		} else {
			m_stiffness.EnableWindow(true);
			m_dampening.EnableWindow(true);
			m_static_friction.EnableWindow(true);
			m_dynamic_friction.EnableWindow(true);
			m_logicalSlider.EnableWindow(false);

			m_stiffness1.EnableWindow ( true );
			m_damping1.EnableWindow ( true );
			m_static1.EnableWindow ( true );
			m_dynamic1.EnableWindow ( true );
		}

		m_stiffness.SetPos((int)(m_shape->getStiffness()*100.0));
		m_dampening.SetPos((int)(m_shape->getDampening()*100.0));
		m_static_friction.SetPos((int)(m_shape->getStaticFriction()*100.0));
		m_dynamic_friction.SetPos((int)(m_shape->getDynamicFriction()*100.0));
		m_logicalSlider.SetPos((int)(m_shape->getLogicalValue()*100.0));

		m_customRenderCheck.SetCheck ( m_shape->IsEnabledCustomRendering () );

		if ( m_shape->getType () == SHAPE_COMPOSITE )
		{
			m_controlMembers.SetCheck ( ((CComposite*)m_shape)->IsControlMembersHaptic () );

			if ( !((CComposite*)m_shape)->IsControlMembersHaptic () )
			{
				m_stiffness.SetPos(0);
				m_dampening.SetPos(0);
				m_static_friction.SetPos(0);
				m_dynamic_friction.SetPos(0);
				m_logicalSlider.SetPos(0);

				m_stiffness.EnableWindow(false);
				m_dampening.EnableWindow(false);
				m_static_friction.EnableWindow(false);
				m_dynamic_friction.EnableWindow(false);
				m_logicalSlider.EnableWindow(false);

				m_stiffness1.EnableWindow ( false );
				m_damping1.EnableWindow ( false );
				m_static1.EnableWindow ( false );
				m_dynamic1.EnableWindow ( false );
			}
		}

	} else {
		m_stiffness.SetPos(0);
		m_dampening.SetPos(0);
		m_static_friction.SetPos(0);
		m_dynamic_friction.SetPos(0);
		m_logicalSlider.SetPos(0);

		m_stiffness.EnableWindow(false);
		m_dampening.EnableWindow(false);
		m_static_friction.EnableWindow(false);
		m_dynamic_friction.EnableWindow(false);
		m_logicalSlider.EnableWindow(false);

		m_stiffness1.EnableWindow ( false );
		m_damping1.EnableWindow ( false );
		m_static1.EnableWindow ( false );
		m_dynamic1.EnableWindow ( false );

		m_controlMembers.EnableWindow ( false );
		m_customRenderCheck.EnableWindow ( false );
	}

	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CTab2::OnFormula() 
{
	// TODO: Add your control notification handler code here
	dlg.setShape(m_shape);

	dlg.ShowWindow(SW_SHOW);
	

	OnPaint();
}

void CTab2::OnEnChangeEditStiffness1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if ( m_shape == NULL )
	{
		return;
	}

	// Get string from edit
	char str[256];
	m_stiffness1.GetWindowText ( str, 256 );

	// COnvert to float
	float answer;

	// If successfull
	if ( StringToFloat ( str, &answer ) && answer >= 0.0 && answer <= 1.0 )
	{
		// Set the value of the slider
		m_stiffness.SetPos((int)(answer*100.0));

		// Set the shape
		m_shape->setStiffness ( answer );
	}
	else
	{
		MessageBox ( "Please enter a number from 0 to 1, for example: 0.453" );
	}
}

void CTab2::OnEnChangeEditDamping1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if ( m_shape == NULL )
	{
		return;
	}

	// Get string from edit
	char str[256];
	m_damping1.GetWindowText ( str, 256 );

	// COnvert to float
	float answer;

	// If successfull
	if ( StringToFloat ( str, &answer ) && answer >= 0.0 && answer <= 1.0 )
	{
		// Set the value of the slider
		m_dampening.SetPos((int)(answer*100.0));

		// Set the shape
		m_shape->setDampening ( answer );
	}
	else
	{
		MessageBox ( "Please enter a number from 0 to 1, for example: 0.453" );
	}
}

void CTab2::OnEnChangeEditStatic1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if ( m_shape == NULL )
	{
		return;
	}

	// Get string from edit
	char str[256];
	m_static1.GetWindowText ( str, 256 );

	// COnvert to float
	float answer;

	// If successfull
	if ( StringToFloat ( str, &answer ) && answer >= 0.0 && answer <= 1.0 )
	{
		// Set the value of the slider
		m_static_friction.SetPos((int)(answer*100.0));

		// Set the shape
		m_shape->setStaticFriction ( answer );
	}
	else
	{
		MessageBox ( "Please enter a number from 0 to 1, for example: 0.453" );
	}
}

void CTab2::OnEnChangeEditDynamic1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if ( m_shape == NULL )
	{
		return;
	}

	// Get string from edit
	char str[256];
	m_dynamic1.GetWindowText ( str, 256 );

	// COnvert to float
	float answer;

	// If successfull
	if ( StringToFloat ( str, &answer ) && answer >= 0.0 && answer <= 1.0 )
	{
		// Set the value of the slider
		m_dynamic_friction.SetPos((int)(answer*100.0));

		// Set the shape
		m_shape->setDynamicFriction ( answer );
	}
	else
	{
		MessageBox ( "Please enter a number from 0 to 1, for example: 0.453" );
	}
}

void CTab2::OnBnClickedControlMembers()
{
	if ( m_shape->getType () != SHAPE_COMPOSITE )
		return;

	CComposite* composite= ((CComposite*)m_shape);

	// TODO: Add your control notification handler code here
	if ( m_controlMembers.GetCheck () )
	{
		composite->SetControlMembersHaptic ( true );
	}
	else
	{
		composite->SetControlMembersHaptic ( false );
	}

	OnPaint ();
}

void CTab2::OnBnClickedCheck2()
{
	// TODO: Add your control notification handler code here
	if ( m_customRenderCheck.GetCheck () )
	{
		m_shape->EnableCustomRendering ( true );
	}
	else
	{
		m_shape->EnableCustomRendering ( false );
	}
}
