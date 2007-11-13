// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Control.cpp
// Classes: CControl
// Purpose: Represents a generic shapes control widget

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "Control.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CControl::CControl()
{

}

CControl::~CControl()
{

}

int CControl::getControlMode()
{
	//If this is called we've gone wrong
	OutputDebugString("Error: Can't get control mode from abstract class.");
	::MessageBox(::GetFocus(),"!","Error",MB_OK);
	return -1;
}

void CControl::setColourScheme(int cs)
{
	m_colourScheme= cs;
}
