// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: CubeControl.cpp
// Classes: CCubeControl
// Purpose: Represents a cube's control widget

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "Cube.h"
#include "CubeControl.h"

#include <gl/gl.h>
#include <gl/glu.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define MAGNETIC_MIN 0.5

CCubeControl::CCubeControl(CCube* cube)
{

	m_cube= cube;
	m_resize0= new CEditPoint();
	m_resize1= new CEditPoint();
	m_resize2= new CEditPoint();
	m_resize3= new CEditPoint();
	m_resize4= new CEditPoint();
	m_resize5= new CEditPoint();
	m_resize6= new CEditPoint();
	m_resize7= new CEditPoint();
	m_resize0->setToolTip("Resize Shape");
	m_resize1->setToolTip("Resize Shape");
	m_resize2->setToolTip("Resize Shape");
	m_resize3->setToolTip("Resize Shape");
	m_resize4->setToolTip("Resize Shape");
	m_resize5->setToolTip("Resize Shape");
	m_resize6->setToolTip("Resize Shape");
	m_resize7->setToolTip("Resize Shape");

	m_rotate0= new CEditPoint();
	m_rotate1= new CEditPoint();
	m_rotate2= new CEditPoint();
	m_rotate3= new CEditPoint();
	m_rotate4= new CEditPoint();
	m_rotate5= new CEditPoint();
	m_rotate0->setToolTip("Rotate Shape");
	m_rotate1->setToolTip("Rotate Shape");
	m_rotate2->setToolTip("Rotate Shape");
	m_rotate3->setToolTip("Rotate Shape");
	m_rotate4->setToolTip("Rotate Shape");
	m_rotate5->setToolTip("Rotate Shape");

	UpdateEditPoints();
}

CCubeControl::CCubeControl(CCubeControl *c)
{

}

CCubeControl::~CCubeControl()
{
	OutputDebugString("Destroying CubeControl\n");
	delete m_resize0;
	delete m_resize1;
	delete m_resize2;
	delete m_resize3;
	delete m_resize4;
	delete m_resize5;
	delete m_resize6;
	delete m_resize7;
	delete m_rotate0;
	delete m_rotate1;
	delete m_rotate2;
	delete m_rotate3;
	delete m_rotate4;
	delete m_rotate5;
}

void CCubeControl::UpdateEditPoints()
{
	float half_x= (float)(m_cube->getSizeX()/2.0);
	float half_y= (float)(m_cube->getSizeY()/2.0);
	float half_z= (float)(m_cube->getSizeZ()/2.0);

	m_resize0->setLocation(-half_x,
						   -half_y,
						   -half_z);
	m_resize1->setLocation(half_x,
						   -half_y,
						   -half_z);
	m_resize2->setLocation(half_x,
						   half_y,
						   -half_z);
	m_resize3->setLocation(-half_x,
						   half_y,
						   -half_z);
	m_resize4->setLocation(-half_x,
						   -half_y,
						   half_z);
	m_resize5->setLocation(half_x,
						   -half_y,
						   half_z);
	m_resize6->setLocation(half_x,
						   half_y,
						   half_z);
	m_resize7->setLocation(-half_x,
						   half_y,
						   half_z);

	m_rotate0->setLocation(0, -((m_cube->getSizeY())/2+1), 0);
	m_rotate1->setLocation(0, 0, -((m_cube->getSizeZ())/2+1) );
	m_rotate2->setLocation(-((m_cube->getSizeX())/2+1), 0, 0);
	m_rotate3->setLocation(0, ((m_cube->getSizeY())/2+1), 0);
	m_rotate4->setLocation(0, 0, ((m_cube->getSizeZ())/2+1) );
	m_rotate5->setLocation(((m_cube->getSizeX())/2+1), 0, 0);
}

void CCubeControl::drawGLScene()
{
	float x= m_cube->getLocationX();
	float y= m_cube->getLocationY();
	float z= m_cube->getLocationZ();

	glPushMatrix();

	glTranslatef(m_cube->getLocationX(),m_cube->getLocationY(),m_cube->getLocationZ());

	glMultMatrixf(m_cube->getRotation());

	UpdateEditPoints();

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

	m_rotate1->drawGLScene();

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

	glDisable(GL_LIGHTING);

	glColor3d(0.5,0.5,0.5);

	glColor3d(1.0,1.0,1.0);
	glBegin(GL_LINES);
	glVertex3d(0,0,0);
	glVertex3d(m_rotate0->getLocationX(),
			   m_rotate0->getLocationY(),
			   m_rotate0->getLocationZ());
	glEnd();

	glColor3d(1.0,1.0,1.0);
	glBegin(GL_LINES);
	glVertex3d(0,0,0);
	glVertex3d(m_rotate1->getLocationX(),
			   m_rotate1->getLocationY(),
			   m_rotate1->getLocationZ());
	glEnd();

	glColor3d(1.0,1.0,1.0);
	glBegin(GL_LINES);
	glVertex3d(0,0,0);
	glVertex3d(m_rotate2->getLocationX(),
			   m_rotate2->getLocationY(),
			   m_rotate2->getLocationZ());
	glEnd();

	glColor3d(1.0,1.0,1.0);
	glBegin(GL_LINES);
	glVertex3d(0,0,0);
	glVertex3d(m_rotate3->getLocationX(),
			   m_rotate3->getLocationY(),
			   m_rotate3->getLocationZ());
	glEnd();

	glColor3d(1.0,1.0,1.0);
	glBegin(GL_LINES);
	glVertex3d(0,0,0);
	glVertex3d(m_rotate4->getLocationX(),
			   m_rotate4->getLocationY(),
			   m_rotate4->getLocationZ());
	glEnd();

	glColor3d(1.0,1.0,1.0);
	glBegin(GL_LINES);
	glVertex3d(0,0,0);
	glVertex3d(m_rotate5->getLocationX(),
			   m_rotate5->getLocationY(),
			   m_rotate5->getLocationZ());
	glEnd();

	glEnable(GL_LIGHTING);
//	glBegin(GL_LINES);
//	glVertex3d(x,y+(m_cube->getSizeY()/2.0),z+(m_cube->getSizeZ()/2.0));
//	glVertex3d(m_rotate1->getLocationX(),
//			   m_rotate1->getLocationY(),
//			   m_rotate1->getLocationZ());
//	glEnd();
//	glBegin(GL_LINES);
//	glVertex3d(x+(m_cube->getSizeX()/2.0),y+(m_cube->getSizeY()/2.0),z);
//	glVertex3d(m_rotate2->getLocationX(),
//			   m_rotate2->getLocationY(),
//			   m_rotate2->getLocationZ());
//	glEnd();

	glPopMatrix();
}

void CCubeControl::drawHapticScene(bool primary_context)
{
	if(m_cube->getSizeX()<MAGNETIC_MIN||
	   m_cube->getSizeY()<MAGNETIC_MIN||
	   m_cube->getSizeZ()<MAGNETIC_MIN)
	{
		m_resize0->setMagnetic(false);
		m_resize1->setMagnetic(false);
		m_resize2->setMagnetic(false);
		m_resize3->setMagnetic(false);
		m_resize4->setMagnetic(false);
		m_resize5->setMagnetic(false);
		m_resize6->setMagnetic(false);
		m_resize7->setMagnetic(false);
	} 
	else
	{
		m_resize0->setMagnetic(true);
		m_resize1->setMagnetic(true);
		m_resize2->setMagnetic(true);
		m_resize3->setMagnetic(true);
		m_resize4->setMagnetic(true);
		m_resize5->setMagnetic(true);
		m_resize6->setMagnetic(true);
		m_resize7->setMagnetic(true);
	}

	
	glPushMatrix();
	glTranslatef(m_cube->getLocationX(),m_cube->getLocationY(),m_cube->getLocationZ());

	glMultMatrixf(m_cube->getRotation());

	UpdateEditPoints();
	m_resize0->drawHapticScene(primary_context);
	m_resize1->drawHapticScene(primary_context);
	m_resize2->drawHapticScene(primary_context);
	m_resize3->drawHapticScene(primary_context);
	m_resize4->drawHapticScene(primary_context);
	m_resize5->drawHapticScene(primary_context);
	m_resize6->drawHapticScene(primary_context);
	m_resize7->drawHapticScene(primary_context);
	m_rotate0->drawHapticScene(primary_context);
	m_rotate1->drawHapticScene(primary_context);
	m_rotate2->drawHapticScene(primary_context);
	m_rotate3->drawHapticScene(primary_context);
	m_rotate4->drawHapticScene(primary_context);
	m_rotate5->drawHapticScene(primary_context);
	glPopMatrix();
}

int CCubeControl::getControlMode()
{
	if(m_resize0->touching() || m_resize0->touching1())
	   return CONTROL_MODE_RESIZE0;
	if(m_resize1->touching() || m_resize1->touching1())
	   return CONTROL_MODE_RESIZE1;
	if(m_resize2->touching() || m_resize2->touching1())
	   return CONTROL_MODE_RESIZE2;
	if(m_resize3->touching() || m_resize3->touching1())
	   return CONTROL_MODE_RESIZE3;
	if(m_resize4->touching() || m_resize4->touching1())
	   return CONTROL_MODE_RESIZE4;
	if(m_resize5->touching() || m_resize5->touching1())
	   return CONTROL_MODE_RESIZE5;
	if(m_resize6->touching() || m_resize6->touching1())
	   return CONTROL_MODE_RESIZE6;
	if(m_resize7->touching() || m_resize7->touching1())
	   return CONTROL_MODE_RESIZE7;

	if(m_rotate0->touching() || m_rotate0->touching1())
	   return CONTROL_MODE_ROTATE0;
	if(m_rotate1->touching() || m_rotate1->touching1())
	   return CONTROL_MODE_ROTATE1;
	if(m_rotate2->touching() || m_rotate2->touching1())
	   return CONTROL_MODE_ROTATE2;
	if(m_rotate3->touching() || m_rotate3->touching1())
	   return CONTROL_MODE_ROTATE3;
	if(m_rotate4->touching() || m_rotate4->touching1())
	   return CONTROL_MODE_ROTATE4;
	if(m_rotate5->touching() || m_rotate5->touching1())
	   return CONTROL_MODE_ROTATE5;

	if(m_cube->touching() || m_cube->touching1())
		return CONTROL_MODE_MOVE;

	return CONTROL_MODE_NULL;
}

void CCubeControl::setCube(CCube *c)
{
	m_cube= c;
}

CCubeControl* CCubeControl::clone()
{
	CCubeControl* c= new CCubeControl(this);
	return c;
}
