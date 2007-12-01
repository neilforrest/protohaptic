// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Cylinder.cpp
// Classes: CCylinder
// Purpose: Represents cylider shape

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "Cylinder.h"
#include "CylinderControl.h"
#include "Geometry.h"
#include <gl/glut.h>
#include <math.h>

#ifndef PI
#define PI 3.1415926535897
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCylinder::CCylinder(float x, float y, float z)
{	
	m_sizeX= x; m_sizeY= y; m_sizeZ= z;
	setLocation(0.0,0.0,0.0);
	m_control= new CCylinderControl(this);
	s= gluNewQuadric();
	d1= gluNewQuadric(); gluQuadricOrientation(d1, GLU_OUTSIDE);
	d2= gluNewQuadric(); gluQuadricOrientation(d1, GLU_INSIDE);
	m_ratio= 1;

	diskATrans= new hduMatrix ( );
	diskBTrans= new hduMatrix ( );

	diskAShape= new CDisk ();
	diskBShape= new CDisk ();

	diskAShape->AddDependant ( this );
	diskAShape->AddDependant ( this );

	customCylinder= new CHapticCylinder ( this );
	diskA= new CHapticDisk ( diskATrans );
	diskB= new CHapticDisk ( diskBTrans );
}

CCylinder::~CCylinder ()
{
	delete customCylinder;
	delete diskATrans;
	delete diskBTrans;

	delete diskA;
	delete diskB;
}

CShape* CCylinder::clone()
{
	CCylinder* cloned;

	cloned= new CCylinder(1,1,1);
	cloned->setLocation(m_locationX,m_locationY,m_locationZ);
	cloned->setRotation(m_rotation);
	cloned->setSize(m_sizeX,m_sizeY,m_sizeZ);
	cloned->setRatio(m_ratio);
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

	return (CShape*)cloned;
}

// Return ptr to the feelable object representing disks
CDisk* CCylinder::GetDiskA ()
{
	return diskAShape;
}

CDisk* CCylinder::GetDiskB ()
{
	return diskBShape;
}

void CCylinder::setRatio(float ratio)
{
	m_ratio= ratio;
}

float CCylinder::getRatio()
{
	return m_ratio;
}

void CCylinder::Serialize(CArchive& ar)
{
	CShape::Serialize(ar);

	if(ar.IsStoring())
		ar << m_ratio;
	else
		ar >> m_ratio;
}

int CCylinder::getType() { return SHAPE_CYLINDER; }

void CCylinder::drawGLScene()
{

	PreRender();
	glPushMatrix();

	glTranslatef(m_locationX,
				 m_locationY,
				 m_locationZ);

	glMultMatrixf(m_rotation);
	glTranslatef(0.0,0.0,(float)(-m_sizeZ/2.0));
	glScalef(m_sizeX, m_sizeY, m_sizeZ);

	glColor3f(m_colourRed,
			  m_colourGreen,
			  m_colourBlue);

	//if ( touching() )
	//{
	//	glColor3f(0,
	//			  1,
	//			  0);
	//}

	int stacks= ((CProtoHapticApp*)AfxGetApp())->getStacks();

	gluCylinder(s, 0.5*m_ratio,0.5,1, stacks, stacks);
	gluDisk(d1,0.0,0.5*m_ratio,stacks,stacks);
	glTranslated(0.0,0.0,1);
	gluDisk(d2,0.0,0.5,stacks,stacks);

	glPopMatrix();
}

void CCylinder::drawHapticScene(bool primary_context)
{
	hlMaterialf(HL_FRONT_AND_BACK, HL_STIFFNESS, m_stiffness);
	hlMaterialf(HL_FRONT_AND_BACK, HL_DAMPING, m_dampening);
	hlMaterialf(HL_FRONT_AND_BACK, HL_STATIC_FRICTION, m_static_friction);
	hlMaterialf(HL_FRONT_AND_BACK, HL_DYNAMIC_FRICTION, m_dynamic_friction);

	if ( !m_customRendered )
	{
		if(primary_context)
			hlBeginShape(HL_SHAPE_FEEDBACK_BUFFER, m_shapeID);
		else
			hlBeginShape(HL_SHAPE_FEEDBACK_BUFFER, m_shapeID_1);

		drawGLScene();
		hlEndShape();
	}
	else
	{
		// Draw cylinder section
		if(primary_context)
			hlBeginShape(HL_SHAPE_CALLBACK, m_shapeID );
		else
			hlBeginShape(HL_SHAPE_CALLBACK, m_shapeID_1 );

		hlCallback(HL_SHAPE_INTERSECT_LS, 
			(HLcallbackProc) CHapticCylinder::intersectSurface, (void *) customCylinder);
		hlCallback(HL_SHAPE_CLOSEST_FEATURES, 
			(HLcallbackProc) CHapticCylinder::closestSurfaceFeatures, (void *) customCylinder);
		hlEndShape();

		// Draw disk A
		if(primary_context)
			hlBeginShape(HL_SHAPE_CALLBACK, diskAShape->GetShapeID () );
		else
			hlBeginShape(HL_SHAPE_CALLBACK, diskAShape->GetShapeID_1 ());

		diskATrans->makeIdentity ();

		diskATrans->multRight ( hduMatrix::createRotation ( 1, 0, 0, 0.5*PI ) );
		diskATrans->multRight ( hduMatrix::createTranslation ( 0, 0, 0.5 ) );

		diskATrans->multRight ( hduMatrix::createScale ( getSizeX(), getSizeY(), getSizeZ() ) );
		diskATrans->multRight ( getHduMatrix ( getRotation () ) );
		diskATrans->multRight ( hduMatrix::createTranslation ( getLocationX(), getLocationY(), getLocationZ () ) );

		hlCallback(HL_SHAPE_INTERSECT_LS, 
			(HLcallbackProc) CHapticDisk::intersectSurface, (void *) diskA);
		hlCallback(HL_SHAPE_CLOSEST_FEATURES, 
			(HLcallbackProc) CHapticDisk::closestSurfaceFeatures, (void *) diskA);
		hlEndShape();

		// Draw disk B
		if(primary_context)
			hlBeginShape(HL_SHAPE_CALLBACK, diskBShape->GetShapeID () );
		else
			hlBeginShape(HL_SHAPE_CALLBACK, diskBShape->GetShapeID_1 ());

		diskBTrans->makeIdentity ();

		diskBTrans->multRight ( hduMatrix::createRotation ( 1, 0, 0, -0.5*PI ) );
		diskBTrans->multRight ( hduMatrix::createTranslation ( 0, 0, -0.5 ) );

		diskBTrans->multRight ( hduMatrix::createScale ( getSizeX(), getSizeY(), getSizeZ() ) );
		diskBTrans->multRight ( getHduMatrix ( getRotation () ) );
		diskBTrans->multRight ( hduMatrix::createTranslation ( getLocationX(), getLocationY(), getLocationZ () ) );

		hlCallback(HL_SHAPE_INTERSECT_LS, 
			(HLcallbackProc) CHapticDisk::intersectSurface, (void *) diskB);
		hlCallback(HL_SHAPE_CLOSEST_FEATURES, 
			(HLcallbackProc) CHapticDisk::closestSurfaceFeatures, (void *) diskB);
		hlEndShape();
	}

}

CString CCylinder::drawGLCode()
{
	char translate[100];
	char translate1[100];
	char cylinder[100];
	char disk[100];
	char disk1[100];
	char rotate1[500];
	char rotate2[100];
	char scale[100];
	char colour[100];
	char quads[200];

	sprintf(translate, "glTranslatef(%f,%f,%f);", m_locationX,m_locationY,m_locationZ);
	sprintf(rotate1, "float cube%d_rotation[16]=\r\n   { %f, %f, %f, %f,\r\n     %f, %f, %f, %f,\r\n     %f, %f, %f, %f,\r\n     %f, %f, %f, %f };",
		m_shapeID,m_rotation[0],m_rotation[1],m_rotation[2],m_rotation[3],m_rotation[4],m_rotation[5],m_rotation[6],
		m_rotation[7],m_rotation[8],m_rotation[9],m_rotation[10],m_rotation[11],m_rotation[12],m_rotation[13],
		m_rotation[14],m_rotation[15]);	
	sprintf(rotate2, "glMultMatrixf(cube%d_rotation);", m_shapeID);
	sprintf(scale, "glScalef(%f,%f,%f);", m_sizeX,m_sizeY,m_sizeZ);
	sprintf(colour, "glColor3f(%f,%f,%f);", m_colourRed,m_colourGreen,m_colourBlue);
	sprintf(translate1, "glTranslatef(0.0,0.0,(float)(-%f/2.0));", m_sizeZ);
	sprintf(cylinder, "gluCylinder(a%d, 0.5*%f,0.5,1, %d, %d);", m_shapeID, m_ratio,((CProtoHapticApp*)AfxGetApp())->getStacks(),((CProtoHapticApp*)AfxGetApp())->getStacks());
	sprintf(disk, "gluDisk(b%d,0.0,0.5*%f,%d,%d);", m_shapeID, m_ratio,((CProtoHapticApp*)AfxGetApp())->getStacks(),((CProtoHapticApp*)AfxGetApp())->getStacks());
	sprintf(disk1, "gluDisk(c%d,0.0,0.5,%d,%d);", m_shapeID,((CProtoHapticApp*)AfxGetApp())->getStacks(),((CProtoHapticApp*)AfxGetApp())->getStacks());
	sprintf(quads, "GLUquadric* a%d, *b%d,*c%d;\r\na%d= gluNewQuadric();\r\nb%d= gluNewQuadric();\r\nc%d= gluNewQuadric();\r\ngluQuadricOrientation(b%d, GLU_INSIDE);",
		m_shapeID, m_shapeID, m_shapeID, m_shapeID, m_shapeID, m_shapeID, m_shapeID);

	CString s= "// --(Begin CYLINDER)---------------------------------------- // \r\n";

	s= s + "glPushMatrix();\r\n";

	s= s + CString(translate) + "\r\n";

	s= s + CString(rotate1) + "\r\n";

	s= s + CString(rotate2) + "\r\n";

	s= s + CString(translate1) + "\r\n";

	s= s + CString(scale) + "\r\n";

	s= s + CString(colour) + "\r\n";

	s= s + CString(quads) + "\r\n";

	s= s + CString(cylinder) + "\r\n";

	s= s + CString(disk) + "\r\n";

	s= s + "glTranslated(0.0,0.0,1);\r\n";

	s= s + CString(disk1) + "\r\n";

	s= s + "glPopMatrix();\r\n";

	s= s + "// --(End CYLINDER)------------------------------------------ // \r\n";

	return s;
}
