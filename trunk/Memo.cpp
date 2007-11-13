// Memo.cpp: implementation of the CMemo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "Memo.h"
#include "SphereControl.h"

#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemo::CMemo()
{
	m_sizeX= 1; m_sizeY= 1; m_sizeZ= 1;
	setLocation(0.0,0.0,0.0);
	m_control= new CSphereControl(this);
}


CMemo::CMemo(CString name)
{
	m_name= name;
	m_sizeX= 1; m_sizeY= 1; m_sizeZ= 1;
	setLocation(0.0,0.0,0.0);
	m_control= new CSphereControl(this);
}

CMemo::~CMemo()
{

}

void CMemo::drawGLScene()
{
	glPushMatrix();

	glTranslatef(m_locationX,
				 m_locationY,
				 m_locationZ);

	glMultMatrixf(m_rotation);

	glScalef(m_sizeX,m_sizeY,m_sizeZ);

	glColor3f(0.0,
			  1.0,
			  1.0);

	glutSolidSphere(0.5,20,20);

	glPopMatrix();
}

void CMemo::drawHapticScene(bool primary_context)
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

int CMemo::getType()
{
	return SHAPE_MEMO;
}

void CMemo::Serialize(CArchive& ar)
{
	CShape::Serialize(ar);
	if (ar.IsStoring()) {
		ar << m_name;
	} else {
		ar >> m_name;
	}
}

CShape* CMemo::clone()
{
	CMemo* cloned;

	cloned= new CMemo(m_name);
	cloned->setLocation(m_locationX,m_locationY,m_locationZ);
	cloned->setRotation(m_rotation);
	cloned->setSize(m_sizeX,m_sizeY,m_sizeZ);
	cloned->setColour(m_colourRed,m_colourGreen,m_colourBlue);
	cloned->setDampening(m_dampening);
	cloned->setDynamicFriction(m_dynamic_friction);
	cloned->setStaticFriction(m_static_friction);
	cloned->setStiffness(m_stiffness);

	return (CShape*)cloned;
}

CString CMemo::getName()
{
	return m_name;
}

CString CMemo::drawGLCode()
{
	return "DrawMemo();\r\n";
}