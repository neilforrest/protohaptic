// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Composite.cpp
// Classes: CComposite
// Purpose: Represents a composite shape

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "Composite.h"
#include "stdafx.h"
#include "SphereControl.h"
#include "Triangle.h"
#include "Torus.h"

#include <math.h>
#include <float.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CComposite::CComposite()
{	
	m_startSize[0]= 1.0;
	m_startSize[1]= 1.0;
	m_startSize[2]= 1.0;

	m_shapeCount= 0;
	m_firstTime= true;

	setSize(1.0,1.0,1.0);

	setLocation(0.0,0.0,0.0);

	m_control= new CSphereControl(this);

	m_controlMembersHaptic= false;
}

CComposite::~CComposite()
{
	for(int i= 0; i<m_shapeCount; i++)
		delete m_shapes[i];
}

void CComposite::addShape(CShape *shape)
{
	if(m_shapeCount>=MAX_COMPOSITE_SHAPE) return;

	m_shapes[m_shapeCount]= shape;

	m_shapesStartSize[m_shapeCount][0]= shape->getSizeX();
	m_shapesStartSize[m_shapeCount][1]= shape->getSizeY();
	m_shapesStartSize[m_shapeCount][2]= shape->getSizeZ();


	float minX= FLT_MAX;
	float maxX= -FLT_MAX;
	float minY= FLT_MAX;
	float maxY=- FLT_MAX;
	float minZ= FLT_MAX;
	float maxZ= -FLT_MAX;

	int i;
	for(i= 0; i<=m_shapeCount; i++)
	{

		//Find the extreem shapes which define the bounding box
		//Get bounding cube corners
		m_shapes[i]->getCorners();
		
		int j;
		for(j= 0; j<8; j++)
		{
			if(m_shapes[i]->getCorner(j)[0]>maxX) maxX= m_shapes[i]->getCorner(j)[0];
			if(m_shapes[i]->getCorner(j)[0]<minX) minX= m_shapes[i]->getCorner(j)[0];
			if(m_shapes[i]->getCorner(j)[1]>maxY) maxY= m_shapes[i]->getCorner(j)[1];
			if(m_shapes[i]->getCorner(j)[1]<minY) minY= m_shapes[i]->getCorner(j)[1];
			if(m_shapes[i]->getCorner(j)[2]>maxZ) maxZ= m_shapes[i]->getCorner(j)[2];
			if(m_shapes[i]->getCorner(j)[2]<minZ) minZ= m_shapes[i]->getCorner(j)[2];
		}
	}
	//update my position/size based on this
	m_sizeX= (maxX-minX);
	m_sizeY= (maxY-minY);
	m_sizeZ= (maxZ-minZ);
	m_locationX= (float)(minX+m_sizeX/2.0);
	m_locationY= (float)(minY+m_sizeY/2.0);
	m_locationZ= (float)(minZ+m_sizeZ/2.0);

	m_startSize[0]= m_sizeX;
	m_startSize[1]= m_sizeY;
	m_startSize[2]= m_sizeZ;

	//shape settings
	for(i= 0; i<=m_shapeCount; i++)
	{
		m_offset[i][0]= m_shapes[i]->getLocationX()-m_locationX;
		m_offset[i][1]= m_shapes[i]->getLocationY()-m_locationY;
		m_offset[i][2]= m_shapes[i]->getLocationZ()-m_locationZ;
		int j;
		for(j= 0; j<16; j++)
			m_rotations[i][j]= m_shapes[i]->getRotation()[j];
		m_shapesStartSize[i][0]= m_shapes[i]->getSizeX();
		m_shapesStartSize[i][1]= m_shapes[i]->getSizeY();
		m_shapesStartSize[i][2]= m_shapes[i]->getSizeZ();
	}

	m_shapeCount++;
}

CShape* CComposite::clone()
{
	CComposite* cloned;

	cloned= new CComposite();

	//float m_shapesStartSize[100][3];
	int i, j;
	for(i= 0; i<100; i++)
		for(j= 0; j<3; j++)
			cloned->m_shapesStartSize[i][j]= m_shapesStartSize[i][j];
	//float m_startSize[3];
	for(j= 0; j<3; j++)
		cloned->m_startSize[j]= m_startSize[j];
	//float m_rotations[100][16];
	for(i= 0; i<100; i++)
		for(j= 0; j<16; j++)
			cloned->m_rotations[i][j]= m_rotations[i][j];
	//bool m_firstTime;
	cloned->m_firstTime= m_firstTime;
	//float m_offset[100][3];
	for(i= 0; i<100; i++)
		for(j= 0; j<3; j++)
			cloned->m_offset[i][j]= m_offset[i][j];
	//int m_shapeCount;
	cloned->m_shapeCount= m_shapeCount;
	//CShape* m_shapes[100];
	for(i= 0; i<m_shapeCount; i++)
	{
		OutputDebugString("Cloning Shapes");
		cloned->m_shapes[i]= m_shapes[i]->clone();
	}

	cloned->m_locationX= m_locationX;
	cloned->m_locationY= m_locationY;
	cloned->m_locationZ= m_locationZ;
	for(j= 0; j<16; j++)
			cloned->m_rotation[j]= m_rotation[j];
	cloned->m_sizeX= m_sizeX;
	cloned->m_sizeY= m_sizeY;
	cloned->m_sizeZ= m_sizeZ;

	cloned->m_control= new CSphereControl(cloned);

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

void CComposite::drawGLScene()
{
//

	int i;
	for(i= 0; i<m_shapeCount; i++)
		m_shapes[i]->drawGLScene();


//	bool atLeastOne= false; //...shape is touched
//	
//	int i;
//	for(i= 0; i<m_shapeCount; i++)
//		m_shapes[i]->drawGLScene();
//
//	for(i= 0; i<m_shapeCount; i++)
//	{
//		if(m_shapes[i]->touching()) {
//			touched();
//			atLeastOne= true;
//		}
//	}
//
//	if(!atLeastOne) untouched();
//
//	m_firstTime= false;
}

void CComposite::SetControlMembersHaptic ( bool c )
{
	m_controlMembersHaptic= c;
}

bool CComposite::IsControlMembersHaptic ( )
{
	return m_controlMembersHaptic;
}

void CComposite::drawHapticScene(bool primary_context)
{
	if ( m_controlMembersHaptic )
	{
		hlMaterialf(HL_FRONT_AND_BACK, HL_STIFFNESS, m_stiffness);
		hlMaterialf(HL_FRONT_AND_BACK, HL_DAMPING, m_dampening);
		hlMaterialf(HL_FRONT_AND_BACK, HL_STATIC_FRICTION, m_static_friction);
		hlMaterialf(HL_FRONT_AND_BACK, HL_DYNAMIC_FRICTION, m_dynamic_friction);
	

		if(primary_context)
			hlBeginShape(HL_SHAPE_FEEDBACK_BUFFER, m_shapeID);
		else
			hlBeginShape(HL_SHAPE_FEEDBACK_BUFFER, m_shapeID_1);
	}

	for(int i= 0; i<m_shapeCount; i++)
	{
		if ( !m_controlMembersHaptic )
		{
			m_shapes[i]->drawHapticScene( primary_context );
		}
		else
		{
			m_shapes[i]->drawGLScene();
		}
	}

	if ( m_controlMembersHaptic )
	{
		hlEndShape();
	}

//	int i;
//	for(i= 0; i<m_shapeCount; i++)
//	{
//		m_shapes[i]->drawHapticScene();
//	}
}

/** Set the stiffness haptic property
	 Value between 0..1 */
void CComposite::setStiffness(float stiffness)
{
	// Apply to self
	CFeelable::setStiffness ( stiffness );

	if ( m_controlMembersHaptic )
	{
		// Apply to all subshapes
		for(int i= 0; i<m_shapeCount; i++)
		{
			m_shapes[i]->setStiffness ( stiffness );
		}
	}
}

/** Set the dampening haptic property
	 Value between 0..1 */
void CComposite::setDampening(float dampening)
{
	// Apply to self
	CFeelable::setDampening ( dampening );

	if ( m_controlMembersHaptic )
	{
		// Apply to all subshapes
		for(int i= 0; i<m_shapeCount; i++)
		{
			m_shapes[i]->setDampening ( dampening );
		}
	}
}

/** Set the static friction haptic property
	 Value between 0..1 */
void CComposite::setStaticFriction(float staticFriction)
{
	// Apply to self
	CFeelable::setStaticFriction ( staticFriction );

	if ( m_controlMembersHaptic )
	{
		// Apply to all subshapes
		for(int i= 0; i<m_shapeCount; i++)
		{
			m_shapes[i]->setStaticFriction ( staticFriction );
		}
	}
}

/** Set the dynamic friction haptic property
	 Value between 0..1 */
void CComposite::setDynamicFriction(float dynamicFriction)
{
	// Apply to self
	CFeelable::setDynamicFriction ( dynamicFriction );

	if ( m_controlMembersHaptic )
	{
	// Apply to all subshapes
		for(int i= 0; i<m_shapeCount; i++)
		{
			m_shapes[i]->setDynamicFriction ( dynamicFriction );
		}
	}
}

int CComposite::getType() { return SHAPE_COMPOSITE; }

void CComposite::setLocation(float x, float y, float z)
{
	CShape::setLocation(x,y,z);
	setRotation(getRotation());
}

void CComposite::setRotation ( float R[16] )
{
	CShape::setRotation(R);
	int i;
	for(i= 0; i<m_shapeCount; i++)
	{
		float r[16];                             //new rotation
		float *R1= m_shapes[i]->getRotation();//old rotation
		float ratio[3]= { m_sizeX/m_startSize[0],
			              m_sizeY/m_startSize[1],
						  m_sizeZ/m_startSize[2] };// size (now/then)

		// r= R1*R;
		glPushMatrix();
		glLoadIdentity();
		glMultMatrixf(R);
		glMultMatrixf(m_rotations[i]);
		glGetFloatv(GL_MODELVIEW_MATRIX, r);
		glPopMatrix();


		m_shapes[i]->setRotation(r);

		m_shapes[i]->setLocation(m_offset[i][0]*ratio[0]*R[0]+m_offset[i][1]*ratio[1]*R[4]+m_offset[i][2]*ratio[2]*R[8]+getLocationX(),
			                     m_offset[i][0]*ratio[0]*R[1]+m_offset[i][1]*ratio[1]*R[5]+m_offset[i][2]*ratio[2]*R[9]+getLocationY(),
								 m_offset[i][0]*ratio[0]*R[2]+m_offset[i][1]*ratio[1]*R[6]+m_offset[i][2]*ratio[2]*R[10]+getLocationZ());
	}
}

void CComposite::setSize ( float x, float y, float z ) {

	CShape::setSize(x,y,z);

	float ratio[3]= { m_sizeX/m_startSize[0],
		              m_sizeY/m_startSize[1],
					  m_sizeZ/m_startSize[2] };

	int i;
	for(i= 0; i<m_shapeCount; i++)
	{
		m_shapes[i]->setSize(ratio[0]*m_shapesStartSize[i][0],
							 ratio[1]*m_shapesStartSize[i][1],
							 ratio[2]*m_shapesStartSize[i][2]);
	}


	setRotation(getRotation());
}

int CComposite::getShapeCount()
{
	return m_shapeCount;
}

CShape* CComposite::getShape(int i)
{
	return m_shapes[i];
}

void CComposite::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_shapeCount;
		int i;
		for (i= 0; i<m_shapeCount; i++) {
			ar << m_shapes[i]->getType();
			m_shapes[i]->Serialize(ar);
		}

		ar << m_name;
	}
	else
	{
		m_shapeCount= 0;
		int count;
		ar >> count;
		int i;
		for (i= 0; i<count; i++)
		{	
			int shapeType;
			CShape* s= NULL;
			ar >> shapeType;
			if(shapeType==SHAPE_CUBE)		s= new CCube(1,1,1);
			if(shapeType==SHAPE_SPHERE)		s= new CSphere(1,1,1);
			if(shapeType==SHAPE_CYLINDER)	s= new CCylinder(1,1,1);
			if(shapeType==SHAPE_COMPOSITE)  s= new CComposite();
			if(shapeType==SHAPE_TRIANGLE)   s= new CTriangle();
			if(shapeType==SHAPE_TORUS)		s= new CTorus();
			
			// Unknown shape type
			if ( s == NULL )
			{
				MessageBox ( 0, "There was an error whilst reading the specified file", "Error", MB_OK );
				return;
			}
			else
			{
				s->Serialize(ar);
				addShape(s);
			}
		}

		ar >> m_name;
	}
}

void CComposite::setColour(float r, float g, float b)
{
	m_colourRed= r; 
	m_colourGreen= g;
	m_colourBlue= b;

	for(int i= 0; i<m_shapeCount; i++)
		m_shapes[i]->setColour(r,g,b);
}

float CComposite::getRed()
{
	return m_colourRed;
}

float CComposite::getGreen()
{
	return m_colourGreen;
}

float CComposite::getBlue()
{
	return m_colourBlue;
}

CString CComposite::drawGLCode()
{
	CString s= "// --(Begin COMPOSITE)--------------------------------------- // \r\n";

	int i;
	for(i= 0; i<m_shapeCount; i++)
		s= s + m_shapes[i]->drawGLCode();

	s= s + "// --(End COMPOSITE------------------------------------------ // \r\n";

	return s;
}

/** Return true iff feelable is being touched in context c */
bool CComposite::touching ()
{
	if ( m_controlMembersHaptic )
	{
		return CFeelable::touching ( );
	}
	else
	{
		for ( int i= 0; i < m_shapeCount; i++ )
		{
			if ( m_shapes[i]->touching () )
				return true;
		}

		return false;
	}
}

/** Return true iff feelable is being touched in context c_1 */
bool CComposite::touching1 ()
{
	if ( m_controlMembersHaptic )
	{
		return CFeelable::touching1 ( );
	}
	else
	{
		for ( int i= 0; i < m_shapeCount; i++ )
		{
			if ( m_shapes[i]->touching1 () )
				return true;
		}

		return false;
	}
}