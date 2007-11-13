// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Shape.cpp
// Classes: CShape
// Purpose: Represents a shape

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "Shape.h"
#include <iostream>
using namespace std; 

#include <mmsystem.h>

static float identity[] = { 1, 0, 0, 0,
							0, 1, 0, 0,
							0, 0, 1, 0,
							0, 0, 0, 1 };

CShape::~CShape()
{
	OutputDebugString("Destroying Shape\n");
	if(m_control!=NULL)
		delete m_control;
}

CShape::CShape()
{
	int i;
	for(i= 0; i<16; i++)
		m_rotation[i]= identity[i];

	m_rotationFixed= false;
	m_collisionDynamic= false;
	m_proxyDynamic= false;
	m_surfaceFriction= 0.1;
	m_mass= 100.0;
	m_touchAlert= false;

	m_name= "!";	// For debugging variable names
}

void CShape::setLocation ( float x, float y, float z )
{
	m_locationX= x; m_locationY= y; m_locationZ= z;
}

void CShape::setRotation ( float R[16] )
{
	int i;
	for(i= 0; i<16; i++)
		m_rotation[i]= R[i];
}

/** Get name */
CString CShape::GetName ( void )
{
	return m_name;
}

/** Set name */
void CShape::SetName ( CString name )
{
	m_name= name;
}

float* multvector(float M[16], float v[3])
{
	float* r= (float*)malloc(sizeof(float[3]));



	return r;
}

float* CShape::getCorners()
{

	m_corners[0][0]= -m_sizeX/2.0; m_corners[0][1]= +m_sizeY/2.0; m_corners[0][2]= +m_sizeZ/2.0;
	m_corners[1][0]= +m_sizeX/2.0; m_corners[1][1]= +m_sizeY/2.0; m_corners[1][2]= +m_sizeZ/2.0;
	m_corners[2][0]= +m_sizeX/2.0; m_corners[2][1]= -m_sizeY/2.0; m_corners[2][2]= +m_sizeZ/2.0;
	m_corners[3][0]= -m_sizeX/2.0; m_corners[3][1]= -m_sizeY/2.0; m_corners[3][2]= +m_sizeZ/2.0;
	m_corners[4][0]= -m_sizeX/2.0; m_corners[4][1]= +m_sizeY/2.0; m_corners[4][2]= -m_sizeZ/2.0;
	m_corners[5][0]= +m_sizeX/2.0; m_corners[5][1]= +m_sizeY/2.0; m_corners[5][2]= -m_sizeZ/2.0;
	m_corners[6][0]= +m_sizeX/2.0; m_corners[6][1]= -m_sizeY/2.0; m_corners[6][2]= -m_sizeZ/2.0;
	m_corners[7][0]= -m_sizeX/2.0; m_corners[7][1]= -m_sizeY/2.0; m_corners[7][2]= -m_sizeZ/2.0;

	int i;
	for(i= 0; i<8; i++)
	{
		float r[3];
		float v[3]= { m_corners[i][0], m_corners[i][1], m_corners[i][2] }; 

		r[0]= v[0]*getRotation()[0] + v[1]*getRotation()[4]+ v[2]*getRotation()[8];
		r[1]= v[0]*getRotation()[1] + v[1]*getRotation()[5]+ v[2]*getRotation()[9];
		r[2]= v[0]*getRotation()[2] + v[1]*getRotation()[6]+ v[2]*getRotation()[10]; 

		m_corners[i][0]= r[0]+m_locationX; m_corners[i][1]= r[1]+m_locationY; m_corners[i][2]= r[2]+m_locationZ;
	}

	return (float*)m_corners;
}

CShape* CShape::clone()
{
	//If this is called we've gone wrong
	//OutputDebugString("Error: Can't clone from abstract class.");
	//::MessageBox(::GetFocus(),"!","Error",MB_OK);

	return NULL;
}

void CShape::setSize ( float x, float y, float z )
{
	// Prevents errors from ode when size is 0
	if ( x <= 0 ) x= 0.001;
	if ( y <= 0 ) y= 0.001;
	if ( z <= 0 ) z= 0.001;

	m_sizeX= x;	m_sizeY= y;	m_sizeZ= z;
	m_isCompositeMember= false;
}

float CShape::getLocationX() {return m_locationX;}
float CShape::getLocationY() {return m_locationY;}
float CShape::getLocationZ() {return m_locationZ;}

float *CShape::getRotation() {return m_rotation;}

float CShape::getSizeX() {return m_sizeX;}
float CShape::getSizeY() {return m_sizeY;}
float CShape::getSizeZ() {return m_sizeZ;}

CControl* CShape::getControl() { return m_control; }

int boolValue(bool b) { if (b) return 1; else return 0; }

void CShape::Serialize(CArchive& ar) 

{

	if (ar.IsStoring()) {
		ar << m_locationX
		   << m_locationY
		   << m_locationZ;
		int i;
		for(i= 0; i<16; i++) ar << m_rotation[i];
		ar << m_sizeX
		   << m_sizeY
		   << m_sizeZ;
		ar << m_colourRed
			<< m_colourGreen
			<< m_colourBlue
			<< m_stiffness
			<< m_dampening
			<< m_static_friction
			<< m_dynamic_friction
			<< m_surfaceFriction
			<< m_mass
			<< boolValue(m_proxyDynamic)
			<< boolValue(m_collisionDynamic)
			<< boolValue(m_rotationFixed);

		if(isLogical()) ar << 1; else ar << 0;

		ar << m_stiffnessString;
		ar << m_dampeningString;
		ar << m_staticFString;
		ar << m_dynamicFString;
	
		// Name of shape (used in code)
		ar << m_name;
			
	} else {
		int boolValue;
		ar >> m_locationX
		   >> m_locationY
		   >> m_locationZ;
		int i;
		for(i= 0; i<16; i++) ar >> m_rotation[i];
		ar >> m_sizeX
		   >> m_sizeY
		   >> m_sizeZ;
		ar >> m_colourRed
			>> m_colourGreen
			>> m_colourBlue
			>> m_stiffness
			>> m_dampening
			>> m_static_friction
			>> m_dynamic_friction
			>> m_surfaceFriction
		    >> m_mass;
		ar >> boolValue;
		if(boolValue==1) m_proxyDynamic= true; else m_proxyDynamic= false;
		ar >> boolValue;
		if(boolValue==1) m_collisionDynamic= true; else m_collisionDynamic= false;
		ar >> boolValue;
		if(boolValue==1) m_rotationFixed= true; else m_rotationFixed= false;

  		ar >> boolValue;
  		if(boolValue)
			m_logical= true; 
		else 
			m_logical= false;

  		ar >> m_stiffnessString;
  		ar >> m_dampeningString;
  		ar >> m_staticFString;
  		ar >> m_dynamicFString;

		// Name of shape (used in code)
		ar >> m_name;
	}
}

bool CShape::isCompositeMember()
{
	return m_isCompositeMember;
}

void CShape::setCompositeMember(bool composite)
{
	m_isCompositeMember= composite;
}

float* CShape::getCorner(int i)
{
	return m_corners[i];
}

int CShape::getType()
{
	//If this is called we've gone wrong
	OutputDebugString("Error: Can't getType from abstract class.");
	::MessageBox(::GetFocus(),"!","Error",MB_OK);

	return -1;
}

float CShape::getMass()
{
	return m_mass;
}

float CShape::getSurfaceFriction()
{
	return m_surfaceFriction;
}

bool CShape::isProxyDynamic()
{
	return m_proxyDynamic;
}

bool CShape::isCollisionDynamic()
{
	return m_collisionDynamic;
}

bool CShape::isFixedRotation()
{
	return m_rotationFixed;
}

void CShape::setMass(float m)
{
	m_mass= m;
}

void CShape::setSurfaceFriction(float sf)
{
	m_surfaceFriction= sf;
}

void CShape::setProxyDynamic(bool pd)
{
	m_proxyDynamic= pd;
}

void CShape::setCollisionDynamic(bool cd)
{
	m_collisionDynamic= cd;
}

void CShape::setFixedRotation(bool fr)
{
	m_rotationFixed= fr;
}

void CShape::PreRender()
{
		if(m_touchAlert!=touching() && ((CProtoHapticApp*)AfxGetApp())->isAudioTouch())
		{
			if(touching())
				PlaySound(TOUCH_SHAPE, NULL, SND_FILENAME | SND_ASYNC );
			//else
				//PlaySound(UNTOUCH_SHAPE, NULL, SND_FILENAME | SND_ASYNC );

			m_touchAlert= touching();
		}
}
