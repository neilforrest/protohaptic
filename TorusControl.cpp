#include "stdafx.h"
#include "TorusControl.h"

CTorusControl::CTorusControl(void)
{
	m_radiusEdit= new CEditPoint ( );
	m_radiusMinorEdit= new CEditPoint ( );
}

CTorusControl::CTorusControl(CTorus* shape)
:
CSphereControl ((CShape*)shape)
{
	m_radiusEdit= new CEditPoint ( );
	m_radiusMinorEdit= new CEditPoint ( );
}

CTorusControl::~CTorusControl(void)
{
	delete m_radiusEdit;
}

void CTorusControl::drawGLScene()
{
	//CSphereControl::drawGLScene();
	float x= m_sphere->getSizeX() * (((CTorus*)m_sphere)->getRadiusMajor () + ((CTorus*)m_sphere)->getRadiusMinor ());
	float y= m_sphere->getSizeY() * (((CTorus*)m_sphere)->getRadiusMajor () + ((CTorus*)m_sphere)->getRadiusMinor ());
	float z= m_sphere->getSizeZ() * (((CTorus*)m_sphere)->getRadiusMinor ());

	m_resize0->setLocation(-x,-y,-z);
	m_resize1->setLocation(x,-y,-z);
	m_resize2->setLocation(x,y,-z);
	m_resize3->setLocation(-x,y,-z);
	m_resize4->setLocation(-x,-y,z);
	m_resize5->setLocation(x,-y,z);
	m_resize6->setLocation(x,y,z);
	m_resize7->setLocation(-x,y,z);

	m_rotate0->setLocation(0, -(y+1), 0);
	m_rotate1->setLocation(0, 0, -(z+1) );
	m_rotate2->setLocation(-(x+1), 0, 0);
	m_rotate3->setLocation(0, (y+1), 0);
	m_rotate4->setLocation(0, 0, (z+1) );
	m_rotate5->setLocation((x+1), 0, 0);
	
	glPushMatrix();

	glTranslatef(m_sphere->getLocationX(),m_sphere->getLocationY(),m_sphere->getLocationZ());

	glMultMatrixf(m_sphere->getRotation());

	if(m_colourScheme==CONTROL_COLOURS_PRIMARY)
		glColor3d(0.5,0.5,1.0);
	else if(m_colourScheme==CONTROL_COLOURS_SECONDARY)
		glColor3d(1.0,1.0,0.5);
	else
		glColor3d(0.7,0.2,0.0);
	m_rotate0->drawGLScene();
	m_rotate3->drawGLScene();
	m_rotate4->drawGLScene();
	m_rotate2->drawGLScene();
	m_rotate5->drawGLScene();
	m_rotate1->drawGLScene();

	if(m_colourScheme==CONTROL_COLOURS_PRIMARY)
		glColor3d(1.0,0.5,0.5);
	else if(m_colourScheme==CONTROL_COLOURS_SECONDARY)
		glColor3d(0.5,1.0,0.5);
	else
		glColor3d(0.5,1.0,1.0);
	m_resize0->drawGLScene();
	m_resize1->drawGLScene();
	m_resize2->drawGLScene();
	m_resize3->drawGLScene();
	m_resize4->drawGLScene();
	m_resize5->drawGLScene();
	m_resize6->drawGLScene();
	m_resize7->drawGLScene();

	glDisable(GL_LIGHTING);

	glColor3d(1.0,1.0,1.0);
	glBegin(GL_LINES);
	glVertex3d(0,
			   0,
			   0);
	glVertex3d(m_rotate0->getLocationX(),
			   m_rotate0->getLocationY(),
			   m_rotate0->getLocationZ());
	glVertex3d(0,
			   0,
			   0);
	glVertex3d(m_rotate1->getLocationX(),
			   m_rotate1->getLocationY(),
			   m_rotate1->getLocationZ());
	glVertex3d(0,
			   0,
			   0);
	glVertex3d(m_rotate2->getLocationX(),
			   m_rotate2->getLocationY(),
			   m_rotate2->getLocationZ());
	glVertex3d(0,
			   0,
			   0);
	glVertex3d(m_rotate3->getLocationX(),
			   m_rotate3->getLocationY(),
			   m_rotate3->getLocationZ());
	glVertex3d(0,
			   0,
			   0);
	glVertex3d(m_rotate4->getLocationX(),
			   m_rotate4->getLocationY(),
			   m_rotate4->getLocationZ());
	glVertex3d(0,
			   0,
			   0);
	glVertex3d(m_rotate5->getLocationX(),
			   m_rotate5->getLocationY(),
			   m_rotate5->getLocationZ());
	glEnd();

	connectEditPoints(m_resize0,m_resize1);
	connectEditPoints(m_resize1,m_resize2);
	connectEditPoints(m_resize2,m_resize3);
	connectEditPoints(m_resize3,m_resize0);
	connectEditPoints(m_resize4,m_resize5);
	connectEditPoints(m_resize5,m_resize6);
	connectEditPoints(m_resize6,m_resize7);
	connectEditPoints(m_resize7,m_resize4);
	connectEditPoints(m_resize3,m_resize7);
	connectEditPoints(m_resize2,m_resize6);
	connectEditPoints(m_resize1,m_resize5);
	connectEditPoints(m_resize0,m_resize4);

	glEnable(GL_LIGHTING);

	glPopMatrix();

	glPushMatrix();
	glTranslated(m_sphere->getLocationX(),
				 m_sphere->getLocationY(),
				 m_sphere->getLocationZ());
	glMultMatrixf(m_sphere->getRotation());
	m_radiusEdit->setLocation( (m_sphere->getSizeX()) * ((CTorus*)m_sphere)->getRadiusMajor (),
		                       0,
							   (m_sphere->getSizeZ()) * ((CTorus*)m_sphere)->getRadiusMinor () );
	glColor3d(0.5,1.0,0.5);
	m_radiusEdit->drawGLScene();

	m_radiusMinorEdit->setLocation( 0,
								   (m_sphere->getSizeY()) * ((CTorus*)m_sphere)->getRadiusMajor (),
								   (m_sphere->getSizeZ()) * ((CTorus*)m_sphere)->getRadiusMinor () );

	glColor3d(1.0,1.0,0.0);
	m_radiusMinorEdit->drawGLScene();

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
	m_radiusMinorEdit->drawHapticScene(primary_context);
	glPopMatrix();
}

int CTorusControl::getControlMode()
{
	if(m_radiusEdit->touching()||m_radiusEdit->touching1())
		return CONTROL_MODE_TORUS_RADIUS_MAJOR;

	if(m_radiusMinorEdit->touching()||m_radiusMinorEdit->touching1())
		return CONTROL_MODE_TORUS_RADIUS_MINOR;

	return CSphereControl::getControlMode();
}