// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: ShapeTabCtrl.cpp
// Classes: ShapeTabCtrl
// Purpose: Manages the tabs in the shape properties dialog

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "ShapeTabCtrl.h"
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
// ShapeTabCtrl

ShapeTabCtrl::ShapeTabCtrl()
{
   m_DialogID[0] =IDD_DIALOG1;
   m_DialogID[1] =IDD_DIALOG2;
   m_DialogID[2] =IDD_DIALOG4;
   m_DialogID[3] =IDD_DIALOG3;
   m_DialogID[4]= IDD_DIALOG6;


   m_Dialog[0] = new CTab1();
   m_Dialog[1] = new CTab2();
   m_Dialog[2] = new CTab4();
   m_Dialog[3] = new CTab3();
   m_Dialog[4]= new CTab5 ();

   m_nPageCount = 5;
}

ShapeTabCtrl::~ShapeTabCtrl()
{
	OutputDebugString("Destroying ShapeTabCtrl");

	delete m_Dialog[0];
	delete m_Dialog[1];
	delete m_Dialog[2];
	delete m_Dialog[3];
	delete m_Dialog[4];
}

//This function creates the Dialog boxes once
void ShapeTabCtrl::InitDialogs()
{
	int i;
	for(i= 0; i<m_nPageCount; i++)
		m_Dialog[i]->Create(m_DialogID[i],GetParent());
}

void ShapeTabCtrl::ActivateTabDialogs()
{
  int nSel = GetCurSel();

	char s[100];
	sprintf(s, "%d\n", nSel);
	OutputDebugString(s);

  if(m_Dialog[nSel]->m_hWnd)
     m_Dialog[nSel]->ShowWindow(SW_HIDE);

  CRect l_rectClient;
  CRect l_rectWnd;

  GetClientRect(l_rectClient);
  AdjustRect(FALSE,l_rectClient);
  GetWindowRect(l_rectWnd);
  GetParent()->ScreenToClient(l_rectWnd);
  l_rectClient.OffsetRect(l_rectWnd.left,l_rectWnd.top);

  for(int i= 0; i<m_nPageCount; i++)
	m_Dialog[i]->SetWindowPos(&wndTop, l_rectClient.left, l_rectClient.top, l_rectClient.Width(), l_rectClient.Height(), SWP_HIDEWINDOW);


  m_Dialog[nSel]->SetWindowPos(&wndTop, l_rectClient.left, l_rectClient.top, l_rectClient.Width(), l_rectClient.Height(), SWP_SHOWWINDOW);

  m_Dialog[nSel]->ShowWindow(SW_SHOW);

}

BEGIN_MESSAGE_MAP(ShapeTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(ShapeTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ShapeTabCtrl message handlers

void ShapeTabCtrl::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ActivateTabDialogs();
	*pResult = 0;
}

CDialog* ShapeTabCtrl::getPane(int i)
{
	return m_Dialog[i];
}
