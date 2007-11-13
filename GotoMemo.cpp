// GotoMemo.cpp : implementation file
//

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "GotoMemo.h"

#include "ProtoHapticDoc.h"
#include "ProtoHapticView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGotoMemo dialog


CGotoMemo::CGotoMemo(CWnd* pParent /*=NULL*/)
	: CDialog(CGotoMemo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGotoMemo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_count= 0;
}


void CGotoMemo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGotoMemo)
	DDX_Control(pDX, IDC_LIST2, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGotoMemo, CDialog)
	//{{AFX_MSG_MAP(CGotoMemo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGotoMemo message handlers



void CGotoMemo::addName(CString s)
{
	m_list.InsertString(m_count,s);
	m_count++;
}

void CGotoMemo::clear()
{
	m_count= 0;
	int c= m_list.GetCount();
	int i;
	for (i= 0; i < c; i++)
	{
		m_list.DeleteString(0);
	}
}

void CGotoMemo::OnOK() 
{
	// TODO: Add extra validation here
	ShowWindow(SW_HIDE);
	if(m_list.GetCurSel()>=0)
		((CProtoHapticView*)m_view)->OnGotoMemo(m_list.GetCurSel());
}

void CGotoMemo::setView(CView *view)
{
	m_view= view;
}
