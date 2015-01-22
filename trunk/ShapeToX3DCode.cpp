#include "stdafx.h"
#include "ShapeToX3DCode.h"
#include "math.h"

// OpenHaptics Headers
#include <HL/hl.h>
#include <HLU/hlu.h>

#include <HDU/hduMath.h>
#include <HDU/hduMatrix.h>
#include <HDU/hduQuaternion.h>
#include <HDU/hduError.h>
#include <HLU/hlu.h>

#define PI 3.14159265f

CShapeToX3DCode::CShapeToX3DCode(void)
{
}

CShapeToX3DCode::~CShapeToX3DCode(void)
{
}

// Get Euler angles from rotation matrix
void GetEulerFromMatrix ( float* matrix, float *x, float *y, float *z )
{
    // Assuming the angles are in radians.
	if (matrix[4] > 0.998) { // singularity at north pole
		*y = atan2(matrix[2],matrix[10]);
		*x = PI/2.0;
		*z = 0;
		return;
	}
	if (matrix[4] < -0.998) { // singularity at south pole
		*y= atan2(matrix[2],matrix[10]);
		*x = -PI/2.0;
		*z = 0;
		return;
	}
	*y = atan2(-matrix[8],matrix[0]);
	*z = atan2(-matrix[6],matrix[5]);
	*x = asin(matrix[4]);
}

/// Get the code required to define types and variables to aid rendering
/// One-off setup code for the shape goes here
void CShapeToX3DCode::GetDeclareCode ( CShape* shape, CString *string, CString *identifier, bool compositeMember )
{
	// Initialise the code string
	*string= "";

	// Get type of shape
	int type= shape->getType ();

	// Switch based on the type of shape
	// and call the required function
	switch ( type )
	{
	case SHAPE_CUBE:
		GetDeclareCodeCube ( (CCube*)shape, string, identifier, compositeMember );
		break;
	case SHAPE_SPHERE:
		GetDeclareCodeSphere ( (CSphere*)shape, string, identifier, compositeMember );
		break;
	case SHAPE_CYLINDER:
		GetDeclareCodeCylinder ( (CCylinder*)shape, string, identifier, compositeMember );
		break;
	case SHAPE_COMPOSITE:
		GetDeclareCodeComposite ( (CComposite*)shape, string, identifier, compositeMember );
		break;
	case SHAPE_TRIANGLE:
		GetDeclareCodeTriangle ( (CTriangle*)shape, string, identifier, compositeMember );
		break;
	case SHAPE_TORUS:
		GetDeclareCodeTorus ( (CTorus*)shape, string, identifier, compositeMember );
		break;
	case SHAPE_MEMO:
		GetDeclareCodeMemo ( (CMemo*)shape, string, identifier, compositeMember );
		break;
	}
}

/// Get the code required to generate / initialise the shape
/// One-off setup code for the shape goes here
void CShapeToX3DCode::GetInitCode ( CShape* shape, CString *string, CString *identifier, bool compositeMember )
{
	// Initialise the code string
	*string= "";
}

/// Get the code required to render the shape
/// Code required for (re-)drawing the shape goes here
void CShapeToX3DCode::GetRenderCode ( CShape* shape, CString *string, CString *identifier, bool compositeMember )
{
	// Initialise the code string
	*string= "";
}

// Declaration functions

/// Get Init Code for a Cube
void CShapeToX3DCode::GetDeclareCodeCube ( CCube* cube, CString *string, CString *identifier, bool compositeMember  )
{
  GetTransformedShape ( *cube, *string, *identifier, "<Box size='1 1 1' />" );
}

/// Get Init Code for a Sphere
void CShapeToX3DCode::GetDeclareCodeSphere ( CSphere* sphere, CString *string, CString *identifier, bool compositeMember  )
{
  GetTransformedShape ( *sphere, *string, *identifier, "<Sphere radius='0.5' />" );
}

/// Get Init Code for a Cylinder
void CShapeToX3DCode::GetDeclareCodeCylinder ( CCylinder* cylinder, CString *string, CString *identifier, bool compositeMember  )
{
  float rotation[4]= { 1, 0, 0, PI/2 };
	GetTransformedShape ( *cylinder, *string, *identifier, "<Cylinder radius='0.5' height='1' />", rotation );
}

/// Get Init Code for a Composite
void CShapeToX3DCode::GetDeclareCodeComposite ( CComposite* composite, CString *string, CString *identifier, bool compositeMember  )
{
	string->AppendFormat ( "\r\n<Group DEF='G_%s'>", *identifier );

	// Also recursively declare all composite members
	for ( int i= 0; i < composite->getShapeCount (); i++ )
	{
		// Create sub-identifier for sub-shape
		//CString subIdentifier;
		//subIdentifier.Format ( "subShape%d_%s", i, *identifier );

		CString stringA;
		GetDeclareCode ( composite->getShape ( i ), &stringA, &(composite->getShape(i)->GetName ()), true /*is a composite member*/ );
		*string+= stringA;
	}

  string->AppendFormat ( "\r\n</Group>", *identifier );
}

/// Get Declare Code for a Triangle
void CShapeToX3DCode::GetDeclareCodeTriangle ( CTriangle* triangle, CString *string, CString *identifier, bool compositeMember  )
{
	*string= "\r\n<!-- Warning: Triangle not implemented! -->";
}

/// Get Declare Code for a Torus
void CShapeToX3DCode::GetDeclareCodeTorus ( CTorus* torus, CString *string, CString *identifier, bool compositeMember  )
{
	*string= "\r\n<!-- Warning: Triangle not implemented! -->";
}

/// Get Declare Code for a Memo
void CShapeToX3DCode::GetDeclareCodeMemo ( CMemo* torus, CString *string, CString *identifier, bool compositeMember  )
{
	*string= "\r\n<!-- Warning: Memo not implemented! -->";
}

void CShapeToX3DCode::GetTransformedShape ( CShape& shape, CString& string, const CString& identifier, const CString& shapeCode, float* rotation ) {
  float rotate[3];
  GetEulerFromMatrix ( shape.getRotation(), &rotate[0], &rotate[1], &rotate[2] );


  string.AppendFormat ( "\r\n<Transform DEF='T_%s' translation='%f %f %f'>", 
    identifier, 
    shape.getLocationX(), shape.getLocationY(), shape.getLocationZ() );


  string.AppendFormat ( "\r\n<Transform DEF='T_%s' rotation='1 0 0 %f'>", 
    identifier, -rotate[2] );

  string.AppendFormat ( "\r\n<Transform DEF='T_%s' rotation='0 0 1 %f'>", 
    identifier, -rotate[0] );

	string.AppendFormat ( "\r\n<Transform DEF='T_%s' rotation='0 1 0 %f'>", 
    identifier, -rotate[1] );


  string.AppendFormat ( "\r\n<Transform DEF='T_%s' scale='%f %f %f'>", 
    identifier, 
    shape.getSizeX(), shape.getSizeY(), shape.getSizeZ() );

  if ( rotation ) {
    string.AppendFormat ( "\r\n<Transform DEF='T_%s' rotation='%f %f %f %f'>", 
    identifier, rotation[0], rotation[1], rotation[2], rotation[3] );
  }

  string.AppendFormat ( "\r\n  <Shape DEF='S_%s'>", identifier );
  string.AppendFormat ( "\r\n    <Appearance DEF='A_%s'>", identifier );
  string.AppendFormat ( "\r\n      <Material DEF='M_%s' />", identifier );
  string.AppendFormat ( "\r\n    </Appearance>" );
  string.Append ( "\r\n    " + shapeCode );
  string.AppendFormat ( "\r\n  </Shape>" );
  if ( rotation ) {
    string.AppendFormat ( "\r\n</Transform>" );
  }
  string.AppendFormat ( "\r\n</Transform>" );
  string.AppendFormat ( "\r\n</Transform>" );
  string.AppendFormat ( "\r\n</Transform>" );
  string.AppendFormat ( "\r\n</Transform>" );
  string.AppendFormat ( "\r\n</Transform>" );
}