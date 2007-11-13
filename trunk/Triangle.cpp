#include "stdafx.h"
#include "Triangle.h"

#include "TriangleControl.h"
#include "Geometry.h"

#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>

#include <HDU/hduMath.h>
#include <HDU/hduMatrix.h>
#include <HDU/hduQuaternion.h>
#include <HDU/hduError.h>

// Create a new triangle
CTriangle::CTriangle(void)
{
	// Set defaults
	m_sizeX= 1.0; m_sizeY= 1.0; m_sizeZ= 1.0;
	setLocation(0.0,0.0,0.0);

	// Initialise position of verticies in local coords
	a[0]= -0.5; a[1]= -0.5 ; a[2]= 0;		// Vertex A
	b[0]= 0.5; b[1]= -0.5; b[2]= 0;			// Vertex B
	c[0]= -0.5; c[1]= 0.5; c[2]= 0;			// Vertex C

	// Create control widget
	m_control= new CTriangleControl(this);
}

// Destroy the triangle
CTriangle::~CTriangle(void)
{
}

// Get the local coordinates of vertex A
float* CTriangle::GetVertexA ()
{
	return a;
}

// Get the local coordinates of vertex B
float* CTriangle::GetVertexB ()
{
	return b;
}

// Get the local coordinates of vertex C
float* CTriangle::GetVertexC ()
{
	return c;
}


// Set the local coordinates of vertex A
void CTriangle::SetVertexA ( float x, float y, float z )
{
	a[0]= x; a[1]= y; a[2]= z;
}

// Set the local coordinates of vertex B
void CTriangle::SetVertexB ( float x, float y, float z )
{
	b[0]= x; b[1]= y; b[2]= z;
}

// Set the local coordinates of vertex C
void CTriangle::SetVertexC ( float x, float y, float z )
{
	c[0]= x; c[1]= y; c[2]= z;
}

// Calculate the centriod of the triangle in local coords
void CTriangle::GetCentre ( float* centre )
{
/*	// Define position vectors of a b anc c
	hduVector3Dd aV ( a[0], a[1], a[2] );
	hduVector3Dd bV ( b[0], b[1], b[2] );
	hduVector3Dd cV ( c[0], c[1], c[2] );

	// Calculate centroid
	hduVector3Dd cen;
	cen= (1.0f/3.0f) * ( aV + bV + cV );

	// Copy answer
	centre[0]= cen[0]; centre[1]= cen[1]; centre[2]= cen[2]; */

	// Find extremities in local coordinates to define to size
	float max[3];

	for ( int i= 0; i < 3; i ++ )
	{
		if ( a[i] > b[i] && a[i] > c[i] ) max[i]= a[i];
		else if ( b[i] > a[i] && b[i] > c[i]  ) max[i]= b[i];
		else max[i]= c[i];
	}

	float min[3];

	for ( int i= 0; i < 3; i ++ )
	{
		if ( a[i] < b[i] && a[i] < c[i] ) min[i]= a[i];
		else if ( b[i] < a[i] && b[i] < c[i]  ) min[i]= b[i];
		else min[i]= c[i];
	}

	// Copy answer
	centre[0]= min[0] + (max[0]-min[0])/2.0f;
	centre[1]= min[1] + (max[1]-min[1])/2.0f;
	centre[2]= min[2] + (max[2]-min[2])/2.0f;
}

// Move vertices (local) and triangle position (model) so that the centroid of
// the triangle is at origin of local coords system
void CTriangle::ReCentreVertices ( )
{
	// Calculate new centre
	float newCentre[3];
	GetCentre ( newCentre );

	// Move triangle verticies by Old Centre (0,0,0) - New Centre. i.e. -newCentre

	hduVector3Dd newCentreV ( newCentre[0], newCentre[1], newCentre[2] );
	hduVector3Dd offset;

	a[0]-= newCentre[0];
	a[1]-= newCentre[1];
	a[2]-= newCentre[2];	// Vertex A

	b[0]-= newCentre[0];
	b[1]-= newCentre[1];
	b[2]-= newCentre[2];	// Vertex B

	c[0]-= newCentre[0];
	c[1]-= newCentre[1];
	c[2]-= newCentre[2];	// Vertex C

	// Get the shapes current transform
	hduMatrix transform;

	// Define the items scaling as an hduMatrix
	hduMatrix scaling= hduMatrix::createScale ( getSizeX (),
												getSizeY (),
												getSizeZ () );

	// Define the items rotation as an hduMatrix
	hduMatrix rotation 
			( getRotation()[0],  getRotation()[1],  getRotation()[2],  getRotation()[3], 
			  getRotation()[4],  getRotation()[5],  getRotation()[6],  getRotation()[7], 
			  getRotation()[8],  getRotation()[9],  getRotation()[10], getRotation()[11], 
			  getRotation()[12], getRotation()[13], getRotation()[14], getRotation()[15] );

	//transform.multRight ( scaling );
	transform.multRight ( rotation );

	// Apply the inverse of shape's transform to the new centre position
	transform.multVecMatrix ( newCentreV, offset );

	// Move whole trianlge by New Centre - Old Centre. i.e. newCentre, to compensate
	setLocation ( getLocationX() + offset[0],
				  getLocationY() + offset[1],
				  getLocationZ() + offset[2] );

	// Now deal with rotation

	// Set rotation to match normal of the triangle surface and (z) and vector a-c (y)

	// Axes of new rotation
	// x= perp to y, z
	// y= vector a-c
	// z= surface normal

	// Get normal to triangle (in local coords)
	float norm[3];
	getSurfaceNormal ( norm, a, b, c );

	// Construct known axes of rotation
	hduVector3Dd axisZ ( norm[0], norm[1], norm[2] );
	hduVector3Dd axisY ( c[0]-a[0], c[1]-a[1], c[2]-a[2] );

	// Normalize
	axisZ.normalize ();
	axisY.normalize ();

	// To cross product to find X axis
	hduVector3Dd axisX= axisZ.crossProduct ( axisY );

	// Make sure x is normalised 
	axisX.normalize ();

	// now construct a rotation with these axes
	hduMatrix newRotation ( -axisX[0], -axisX[1], -axisX[2], 0,
							axisY[0], axisY[1], axisY[2], 0,
							axisZ[0], axisZ[1], axisZ[2], 0,  
								   0,		 0,        0, 1 );

	// Apply inverse of new rotation to local vertices
	hduMatrix invNewRotation ( newRotation );
	invNewRotation.invert ();

	hduVector3Dd a_in ( a[0], a[1], a[2] );
	hduVector3Dd b_in ( b[0], b[1], b[2] );
	hduVector3Dd c_in ( c[0], c[1], c[2] );

	hduVector3Dd a_out;
	hduVector3Dd b_out;
	hduVector3Dd c_out;

	invNewRotation.multVecMatrix ( a_in, a_out );
	invNewRotation.multVecMatrix ( b_in, b_out );
	invNewRotation.multVecMatrix ( c_in, c_out );

	a[0]= a_out[0];
	a[1]= a_out[1];
	a[2]= a_out[2];

	b[0]= b_out[0];
	b[1]= b_out[1];
	b[2]= b_out[2];

	c[0]= c_out[0];
	c[1]= c_out[1];
	c[2]= c_out[2];

	// Then apply the rotation itself to the whole triangle

	// Define the items rotation as an hduMatrix
	hduMatrix rotation1 
			( getRotation()[0],  getRotation()[1],  getRotation()[2],  getRotation()[3], 
			  getRotation()[4],  getRotation()[5],  getRotation()[6],  getRotation()[7], 
			  getRotation()[8],  getRotation()[9],  getRotation()[10], getRotation()[11], 
			  getRotation()[12], getRotation()[13], getRotation()[14], getRotation()[15] );

	hduMatrix finalRotation ( newRotation );
	finalRotation.multRight ( rotation1 );

	float rotationArray[16]=
	{
		finalRotation[0][0], finalRotation[0][1], finalRotation[0][2], finalRotation[0][3],
		finalRotation[1][0], finalRotation[1][1], finalRotation[1][2], finalRotation[1][3],
		finalRotation[2][0], finalRotation[2][1], finalRotation[2][2], finalRotation[2][3],
		finalRotation[3][0], finalRotation[3][1], finalRotation[3][2], finalRotation[3][3]
	};

	setRotation ( rotationArray );

	// Now we've handled rotation and translation, which just leaves scaling */

	// Find extremities in local coordinates to define to size
	float max[3];

	for ( int i= 0; i < 3; i ++ )
	{
		if ( a[i] > b[i] && a[i] > c[i] ) max[i]= a[i];
		else if ( b[i] > a[i] && b[i] > c[i]  ) max[i]= b[i];
		else max[i]= c[i];
	}

	float min[3];

	for ( int i= 0; i < 3; i ++ )
	{
		if ( a[i] < b[i] && a[i] < c[i] ) min[i]= a[i];
		else if ( b[i] < a[i] && b[i] < c[i]  ) min[i]= b[i];
		else min[i]= c[i];
	}

	m_sizeX= max[0]-min[0];
	m_sizeY= max[1]-min[1];
	//m_sizeZ= max[2]-min[2];
}

/** Sets the size of the triangle (override) */
void CTriangle::setSize ( float x, float y, float z )
{
	static const float min= 0.00001;

	if ( x < min ) x= min;
	if ( y < min ) y= min;
	if ( z < min ) z= min;

	float prev[3]=
	{
		m_sizeX, m_sizeY, m_sizeZ
	};

	m_sizeX= x;
	m_sizeY= y;
	//m_sizeZ= 1.0;

	float delta[3]=
	{
		m_sizeX/prev[0], m_sizeY/prev[1], m_sizeZ/prev[2]
	};

	hduMatrix scale= hduMatrix::createScale ( delta[0], delta[1], delta[2] );

	hduVector3Dd a_in ( a[0], a[1], a[2] );
	hduVector3Dd b_in ( b[0], b[1], b[2] );
	hduVector3Dd c_in ( c[0], c[1], c[2] );

	hduVector3Dd a_out;
	hduVector3Dd b_out;
	hduVector3Dd c_out;

	scale.multVecMatrix ( a_in, a_out );
	scale.multVecMatrix ( b_in, b_out );
	scale.multVecMatrix ( c_in, c_out );

	a[0]= a_out[0];
	a[1]= a_out[1];
	a[2]= a_out[2];

	b[0]= b_out[0];
	b[1]= b_out[1];
	b[2]= b_out[2];

	c[0]= c_out[0];
	c[1]= c_out[1];
	c[2]= c_out[2];
}

// Overrides CDrawable in order to render the cube graphically
void CTriangle::drawGLScene()
{
	// Initialise
	PreRender();

	// Do shape transformation
	glPushMatrix();

	glTranslatef(m_locationX,
				 m_locationY,
				 m_locationZ);

	glMultMatrixf(m_rotation);

	//glScalef(m_sizeX,m_sizeY,m_sizeZ);

	glColor3f(m_colourRed,
			  m_colourGreen,
			  m_colourBlue);

	// Render the triangle
	glBegin ( GL_TRIANGLES );
	glNormal3f ( 0, 0, 1 );	// Normal in local coords
	glVertex3f ( a[0], a[1], a[2] );
	glVertex3f ( b[0], b[1], b[2] );
	glVertex3f ( c[0], c[1], c[2] );	
	glEnd ();

	glPopMatrix();
}

// Save or load data from/to archive
void CTriangle::Serialize(CArchive& ar)
{
	// Do generic parameters
	CShape::Serialize(ar);

	// Handle specific parameters
	if (ar.IsStoring()) 
	{
		// Save parameters
		ar << a[0];
		ar << a[1];
		ar << a[2];	// Vertex A

		ar << b[0];
		ar << b[1];
		ar << b[2];	// Vertex B

		ar << c[0];
		ar << c[1];
		ar << c[2];	// Vertex C
	} else {
		// Load parameters
		ar >> a[0];
		ar >> a[1];
		ar >> a[2];	// Vertex A

		ar >> b[0];
		ar >> b[1];
		ar >> b[2];	// Vertex B

		ar >> c[0];
		ar >> c[1];
		ar >> c[2];	// Vertex C
	}
}

// Overrides CShape in order to return OpenGL code for rendering the cube
CString CTriangle::drawGLCode()
{
	return "";
}

// Overrides CFeelable to render the cube haptically
void CTriangle::drawHapticScene(bool primary_context)
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

// Return a pointer to a copy of this cube
CShape* CTriangle::clone()
{
	// Copy of current triangle
	CTriangle* cloned;

	// Construct copy
	cloned= new CTriangle();
	cloned->setLocation(m_locationX,m_locationY,m_locationZ);
	cloned->setRotation(m_rotation);
	cloned->m_sizeX= m_sizeX;
	cloned->m_sizeY= m_sizeY;
	cloned->m_sizeZ= m_sizeZ;
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

	// Copy vertices
	cloned->a[0]= a[0];
	cloned->a[1]= a[1];
	cloned->a[2]= a[2];	// Vertex A

	cloned->b[0]= b[0];
	cloned->b[1]= b[1];
	cloned->b[2]= b[2];	// Vertex B

	cloned->c[0]= c[0];
	cloned->c[1]= c[1];
	cloned->c[2]= c[2];	// Vertex C

	cloned->m_name= m_name;

	return cloned;
}

// Return an integer representing the type of shape (i.e. cube)
//   See shape.h for possible values
int CTriangle::getType()
{
	return SHAPE_TRIANGLE;
}