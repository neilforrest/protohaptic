#include "stdafx.h"
#include "TorusControl.h"

CTorusControl::CTorusControl(void)
{
	m_radiusEdit= new CEditPoint ( );
}

CTorusControl::CTorusControl(CTorus* shape)
:
CSphereControl ((CShape*)shape)
{
	m_radiusEdit= new CEditPoint ( );
}

CTorusControl::~CTorusControl(void)
{
	delete m_radiusEdit;
}

void CTorusControl::drawGLScene()
{
	CSphereControl::drawGLScene();

	glPushMatrix();
	glTranslated(m_sphere->getLocationX(),
				 m_sphere->getLocationY(),
				 m_sphere->getLocationZ());
	glMultMatrixf(m_sphere->getRotation());
	m_radiusEdit->setLocation( (m_sphere->getSizeX()) *  ( 0.5 - 2.0*((CTorus*)m_sphere)->getRadius () ),
		                       0,
							   0);
	glColor3d(0.5,1.0,0.5);
	m_radiusEdit->drawGLScene();
	glColor3d(1.0,1.0,1.0);
	glPopMatrix();
}

void CTorusControl::drawHapticScene(bool primary_context)
{
	CSphereControl::drawHapticScene(primary_context);
	glPushMatrix();
	glTranslated(m_sphere->getLocationX(),
				 m_sphere->getLocationY(),
				 m_sphere->getLocationZ());
	glMultMatrixf(m_sphere->getRotation());
	m_radiusEdit->drawHapticScene(primary_context);
	glPopMatrix();
}

int CTorusControl::getControlMode()
{
	if(m_radiusEdit->touching()||m_radiusEdit->touching1())
		return CONTROL_MODE_TORUS_RADIUS;

	return CSphereControl::getControlMode();
}