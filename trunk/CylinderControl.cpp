// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: CylinderControl.cpp
// Classes: CCylinderControl
// Purpose: Represents cylinder's  control widget

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "CylinderControl.h"
#include "Cylinder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void ratioUpdateCallback(double pp[3], CShape* shape)
{
	((CCylinder*)shape)->setRatio((float)(((CCylinder*)shape)->getRatio()+pp[0]));
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCylinderControl::CCylinderControl(CShape* shape)
{
	m_sphere= shape;
	m_ratioEdit= new CEditPoint();
	m_ratioEdit->setToolTip("Cone/Cylinder Ratio");

//	m_ratioEdit->setCallback(ratioUpdateCallback, shape);
}

CCylinderControl::~CCylinderControl()
{
	delete m_ratioEdit;
}

void CCylinderControl::drawGLScene()
{
	CSphereControl::drawGLScene();

	glPushMatrix();
	glTranslated(m_sphere->getLocationX(),
				 m_sphere->getLocationY(),
				 m_sphere->getLocationZ());
	glMultMatrixf(m_sphere->getRotation());
	m_ratioEdit->setLocation((m_sphere->getSizeX())*((CCylinder*)m_sphere)->getRatio()/2, 0,-m_sphere->getSizeZ()/2);
	glColor3d(0.5,1.0,0.5);
	m_ratioEdit->drawGLScene();
	glColor3d(1.0,1.0,1.0);
	glPopMatrix();
}

void CCylinderControl::drawHapticScene(bool primary_context)
{
	CSphereControl::drawHapticScene(primary_context);
	glPushMatrix();
	glTranslated(m_sphere->getLocationX(),
				 m_sphere->getLocationY(),
				 m_sphere->getLocationZ());
	glMultMatrixf(m_sphere->getRotation());
	m_ratioEdit->drawHapticScene(primary_context);
	glPopMatrix();
}

int CCylinderControl::getControlMode()
{
	if(m_ratioEdit->touching()||m_ratioEdit->touching1())
		return CONTROL_MODE_RATIO;

	return CSphereControl::getControlMode();
}
