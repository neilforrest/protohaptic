// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Transform.cpp
// Classes: CTransform
// Purpose: Represents a transformation to a shape

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "Transform.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTransform::CTransform()
{
}

CTransform::~CTransform()
{

}

int CTransform::getType()
{
	//If this is called we've gone wrong
	OutputDebugString("Error: Can't getType from abstract class.");
	::MessageBox(::GetFocus(),"!","Error",MB_OK);

	return -1;
}

CShape* CTransform::getShape()
{
	return m_shape;
}
