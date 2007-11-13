// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Cube.cpp
// Classes: CCube
// Purpose: Represents a cube shape

#include "stdafx.h"
#include "Cube.h"
#include "CubeControl.h"
#include <math.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>

CCube::~CCube()
{

}

CCube::CCube(float x, float y, float z)
{	
	m_sizeX= x; m_sizeY= y; m_sizeZ= z;
	setLocation(0.0,0.0,0.0);
	m_control= new CCubeControl(this);
}

CShape* CCube::clone()
{
	CCube* cloned;

	cloned= new CCube(1,1,1);
	cloned->setLocation(m_locationX,m_locationY,m_locationZ);
	cloned->setRotation(m_rotation);
	cloned->setSize(m_sizeX,m_sizeY,m_sizeZ);
	cloned->setColour(m_colourRed,m_colourGreen,m_colourBlue);
	cloned->setDampening(m_dampening);
	cloned->setDynamicFriction(m_dynamic_friction);
	cloned->setStaticFriction(m_static_friction);
	cloned->setStiffness(m_stiffness);

	cloned->m_surfaceFriction= m_surfaceFriction;
	cloned->m_mass= m_mass;
	cloned->m_proxyDynamic= m_proxyDynamic;
	cloned->m_collisionDynamic= m_collisionDynamic;

	cloned->m_logical= m_logical;
	cloned->m_stiffnessString= m_stiffnessString;
	cloned->m_dampeningString= m_dampeningString;
	cloned->m_staticFString= m_staticFString;
	cloned->m_dynamicFString= m_dynamicFString;

	cloned->m_name= m_name;

	return cloned;;
}

#define PI1 3.141592653589793238

float deg2rad1(float deg)
{
	return deg*((2*PI1)/360.0);
}

void CCube::drawGLScene()
{
	PreRender();
	glPushMatrix();

	glTranslatef(m_locationX,
				 m_locationY,
				 m_locationZ);

	glMultMatrixf(m_rotation);

	glScalef(m_sizeX,m_sizeY,m_sizeZ);

	glColor3f(m_colourRed,
			  m_colourGreen,
			  m_colourBlue);

	glutSolidCube(1);

	glPopMatrix();
}

void CCube::drawHapticScene(bool primary_context)
{
	hlMaterialf(HL_FRONT_AND_BACK, HL_STIFFNESS, m_stiffness);
	hlMaterialf(HL_FRONT_AND_BACK, HL_DAMPING, m_dampening);
	hlMaterialf(HL_FRONT_AND_BACK, HL_STATIC_FRICTION, m_static_friction);
	hlMaterialf(HL_FRONT_AND_BACK, HL_DYNAMIC_FRICTION, m_dynamic_friction);
	if(primary_context)
		hlBeginShape(HL_SHAPE_FEEDBACK_BUFFER, m_shapeID);
	else
		hlBeginShape(HL_SHAPE_FEEDBACK_BUFFER, m_shapeID_1);
	drawGLScene();
	hlEndShape();
}

int CCube::getType() { return SHAPE_CUBE; }

void CCube::Serialize(CArchive& ar)
{
	CShape::Serialize(ar);
	if (ar.IsStoring()) {

	} else {

	}
}

void CCube::setActive(bool active)
{
	CFeelable::setActive(active);
	if(active)
	{
		OutputDebugString("ACTIVE");
		((CCubeControl*)m_control)->setCube(this);
	}
}

CString CCube::drawGLCode()
{
	char translate[100];
	char rotate1[500];
	char rotate2[100];
	char scale[100];
	char colour[100];

	sprintf(translate, "glTranslatef(%f,%f,%f);", m_locationX,m_locationY,m_locationZ);
	sprintf(rotate1, "float cube%d_rotation[16]=\r\n   { %f, %f, %f, %f,\r\n     %f, %f, %f, %f,\r\n     %f, %f, %f, %f,\r\n     %f, %f, %f, %f };",
		m_shapeID,m_rotation[0],m_rotation[1],m_rotation[2],m_rotation[3],m_rotation[4],m_rotation[5],m_rotation[6],
		m_rotation[7],m_rotation[8],m_rotation[9],m_rotation[10],m_rotation[11],m_rotation[12],m_rotation[13],
		m_rotation[14],m_rotation[15]);	
	sprintf(rotate2, "glMultMatrixf(cube%d_rotation);", m_shapeID);
	sprintf(scale, "glScalef(%f,%f,%f);", m_sizeX,m_sizeY,m_sizeZ);
	sprintf(colour, "glColor3f(%f,%f,%f);", m_colourRed,m_colourGreen,m_colourBlue);

	CString s= "// --(Begin CUBE)-------------------------------------------- // \r\n";

	s= s + "glPushMatrix();\r\n";

	s= s + CString(translate) + "\r\n";

	s= s + CString(rotate1) + "\r\n";

	s= s + CString(rotate2) + "\r\n";

	s= s + CString(scale) + "\r\n";

	s= s + CString(colour) + "\r\n";

	s= s + "glutSolidCube(1);\r\n";

	s= s + "glPopMatrix();\r\n";

	s= s + "// --(End CUBE)---------------------------------------------- // \r\n";

	return s;
}