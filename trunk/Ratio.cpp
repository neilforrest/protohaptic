// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Ratio.cpp
// Classes: CRatio
// Purpose: Represents a transformation between cylinder and cone

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "Ratio.h"
#include "Cylinder.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRatio::CRatio(CShape *shape, HLdouble x, HLdouble y, HLdouble z)
{
	m_shape= shape;
	m_ratioStart= ((CCylinder*)m_shape)->getRatio();
	m_proxyStartPos[0]= x;
	m_proxyStartPos[1]= y;
	m_proxyStartPos[2]= z;
}

CRatio::~CRatio() {}

void CRatio::setProxyPos(HLdouble x, HLdouble y, HLdouble z)
{
	double A[3][3]= {
	{ m_shape->getRotation()[0], m_shape->getRotation()[1], m_shape->getRotation()[2] },
	{ m_shape->getRotation()[4], m_shape->getRotation()[5], m_shape->getRotation()[6] },
	{ m_shape->getRotation()[8], m_shape->getRotation()[9], m_shape->getRotation()[10] } };

	double   pV[3]= { x-m_proxyStartPos[0],
					  y-m_proxyStartPos[1],
					  z-m_proxyStartPos[2] };

	double   rV[3]= { pV[0]*A[0][0] + pV[1]*A[0][1]+ pV[2]*A[0][2],
		              pV[0]*A[1][0] + pV[1]*A[1][1]+ pV[2]*A[1][2],
					  pV[0]*A[2][0] + pV[1]*A[2][1]+ pV[2]*A[2][2] };

	float delta_ratio= (float)(rV[0]*2/m_shape->getSizeX());
	float new_ratio= m_ratioStart+delta_ratio;

	if(new_ratio<0) new_ratio= 0;
	if(new_ratio>1) new_ratio= 1;

	((CCylinder*)m_shape)->setRatio(new_ratio);
}

int CRatio::getType()
{
	return -1;
}


CShape *CRatio::getShape()
{
	return m_shape;
}

