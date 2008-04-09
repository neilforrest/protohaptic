#include "stdafx.h"
#include "ShapeToAbstractModelCode.h"
#include "math.h"

// OpenHaptics Headers
#include <HL/hl.h>
#include <HLU/hlu.h>

#include <HDU/hduMath.h>
#include <HDU/hduMatrix.h>
#include <HDU/hduQuaternion.h>
#include <HDU/hduError.h>
#include <HLU/hlu.h>

#define PI 3.14159265

CShapeToAbstractModelCode::CShapeToAbstractModelCode(void)
{
}

CShapeToAbstractModelCode::~CShapeToAbstractModelCode(void)
{
}

/// Get the code required to define types and variables to aid rendering
/// One-off setup code for the shape goes here
void CShapeToAbstractModelCode::GetDeclareCode ( CShape* shape, CString *string, CString *identifier, bool compositeMember )
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
void CShapeToAbstractModelCode::GetInitCode ( CShape* shape, CString *string, CString *identifier, bool compositeMember )
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
		GetInitCodeCube ( (CCube*)shape, string, identifier, compositeMember );
		break;
	case SHAPE_SPHERE:
		GetInitCodeSphere ( (CSphere*)shape, string, identifier, compositeMember );
		break;
	case SHAPE_CYLINDER:
		GetInitCodeCylinder ( (CCylinder*)shape, string, identifier, compositeMember );
		break;
	case SHAPE_COMPOSITE:
		GetInitCodeComposite ( (CComposite*)shape, string, identifier, compositeMember );
		break;
	case SHAPE_TRIANGLE:
		GetInitCodeTriangle ( (CTriangle*)shape, string, identifier, compositeMember );
		break;
	case SHAPE_TORUS:
		GetInitCodeTorus ( (CTorus*)shape, string, identifier, compositeMember );
		break;
	case SHAPE_MEMO:
		GetInitCodeMemo ( (CMemo*)shape, string, identifier, compositeMember );
		break;
	}

	// Only add to model if required, e.g. not required for composite members
	if ( !compositeMember )
	{
		*string+= "\r\n// Add object to model";
	
		//string->AppendFormat ( "\r\nAddModelItem ( %s );",	// Old Syntax
		//					  *identifier );
		string->AppendFormat ( "\r\nAddShape ( %s );",			// New Syntax
							  *identifier );
	}
}

/// Get the code required to render the shape
/// Code required for (re-)drawing the shape goes here
void CShapeToAbstractModelCode::GetRenderCode ( CShape* shape, CString *string, CString *identifier, bool compositeMember )
{
	// Initialise the code string
	*string= "";

	// No rendering code required once the abstract model is defined, do nothing
}

// Declaration functions

/// Get Init Code for a Cube
void CShapeToAbstractModelCode::GetDeclareCodeCube ( CCube* cube, CString *string, CString *identifier, bool compositeMember  )
{
	*string+= "\r\n// Declare a Cube object";
	string->AppendFormat ( "\r\nAbstractModel::CCube* %s;", 
		                   *identifier );
}

/// Get Init Code for a Sphere
void CShapeToAbstractModelCode::GetDeclareCodeSphere ( CSphere* sphere, CString *string, CString *identifier, bool compositeMember  )
{
	*string= "\r\n// Declare a Sphere object";
	string->AppendFormat ( "\r\nAbstractModel::CSphere* %s;", 
		                   *identifier );
}

/// Get Init Code for a Cylinder
void CShapeToAbstractModelCode::GetDeclareCodeCylinder ( CCylinder* cylinder, CString *string, CString *identifier, bool compositeMember  )
{
	*string= "\r\n// Declare a Cylinder object";
	string->AppendFormat ( "\r\nAbstractModel::CCylinder* %s;", 
						   *identifier );
}

/// Get Init Code for a Composite
void CShapeToAbstractModelCode::GetDeclareCodeComposite ( CComposite* composite, CString *string, CString *identifier, bool compositeMember  )
{
	*string= "\r\n// Declare a Composite object";
	string->AppendFormat ( "\r\nAbstractModel::CComposite* %s;", *identifier );

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
}

/// Get Declare Code for a Triangle
void CShapeToAbstractModelCode::GetDeclareCodeTriangle ( CTriangle* triangle, CString *string, CString *identifier, bool compositeMember  )
{
	*string= "\r\n// Declare a Triangle Polygon object";
	string->AppendFormat ( "\r\nAbstractModel::CPolygon* %s;", *identifier );
}

/// Get Declare Code for a Torus
void CShapeToAbstractModelCode::GetDeclareCodeTorus ( CTorus* torus, CString *string, CString *identifier, bool compositeMember  )
{
	*string= "\r\n// Declare a Torus object";
	string->AppendFormat ( "\r\nAbstractModel::CTorus* %s;", *identifier );
}

/// Get Init Code for a Torus
void CShapeToAbstractModelCode::GetInitCodeTorus ( CTorus* torus, CString *string, CString *identifier, bool compositeMember  )
{
	*string+= "\r\n// Initialise a Torus object";

	// Instatiate the object
	string->AppendFormat ( "\r\n%s= new AbstractModel::CTorus ( );", *identifier );

	// Set outer radius
	//string->AppendFormat ( "\r\n%s->SetRadius ( %f );", *identifier, torus->getRadius ( ) );
	string->AppendFormat ( "\r\n%s->SetRadiusMajor ( %f );", *identifier, torus->getRadiusMajor ( ) );
	string->AppendFormat ( "\r\n%s->SetRadiusMinor ( %f );", *identifier, torus->getRadiusMinor ( ) );

	// Add code to set the shapes transform
	GetTransform ( torus, string, identifier, compositeMember );

	// Add code to set the graphic properties
	GetGraphicProperties ( torus, string, identifier );

	// Add code to set the haptic properties
	GetHapticProperties ( torus, string, identifier );
}

/// Get Init Code for a Memo
void CShapeToAbstractModelCode::GetDeclareCodeMemo ( CMemo* memo, CString *string, CString *identifier, bool compositeMember  )
{
	*string+= "\r\n// WARNING: Memo object not supported by AbstractModel translator!";
}

// Initialisation functions

// Get Init Code for a Cube
void CShapeToAbstractModelCode::GetInitCodeCube ( CCube* cube, CString *string, CString *identifier, bool compositeMember  )
{
	*string+= "\r\n// Initialise a Cube object";

	// Instatiate the object
	string->AppendFormat ( "\r\n%s= new AbstractModel::CCube ( );", *identifier );

	// Add code to set the shapes transform
	GetTransform ( cube, string, identifier, compositeMember );

	// Add code to set the graphic properties
	GetGraphicProperties ( cube, string, identifier );

	// Add code to set the haptic properties
	GetHapticProperties ( cube, string, identifier );
}

// Get Init Code for a Sphere
void CShapeToAbstractModelCode::GetInitCodeSphere ( CSphere* sphere, CString *string, CString *identifier, bool compositeMember  )
{
	*string+= "\r\n// Initialise a Sphere object";

	// Instatiate the object
	string->AppendFormat ( "\r\n%s= new AbstractModel::CSphere ( );", *identifier );

	// Add code to set the shapes transform
	GetTransform ( sphere, string, identifier, compositeMember );

	// Add code to set the graphic properties
	GetGraphicProperties ( sphere, string, identifier );

	// Add code to set the haptic properties
	GetHapticProperties ( sphere, string, identifier );
}

// Get Init Code for a Cylinder
void CShapeToAbstractModelCode::GetInitCodeCylinder ( CCylinder* cylinder, CString *string, CString *identifier, bool compositeMember  )
{
	*string+= "\r\n// Initialise a Cylinder object";

	// Instatiate the object
	string->AppendFormat ( "\r\n%s= new AbstractModel::CCylinder ( );", *identifier );

	// Add code to set the shapes transform
	GetTransform ( cylinder, string, identifier, compositeMember );

	// Add code to set the graphic properties
	GetGraphicProperties ( cylinder, string, identifier );

	// Add code to set the haptic properties
	GetHapticProperties ( cylinder, string, identifier );

	// Cylinder specific init code
	string->AppendFormat ( "\r\n%s->SetRatio ( %f );", *identifier, cylinder->getRatio ( ) );
}

// Get Init Code for a Composite
void CShapeToAbstractModelCode::GetInitCodeComposite ( CComposite* composite, CString *string, CString *identifier, bool compositeMember  )
{
	*string+= "\r\n// Initialise a Composite object";

	// Instatiate the object
	string->AppendFormat ( "\r\n%s= new AbstractModel::CComposite ( %d );", *identifier,
		                                                                   composite->getShapeCount () );

	// Add code to set the shapes transform
	//GetTransform ( composite, string, identifier );

	// Add code to set the graphic properties
	//GetGraphicProperties ( composite, string, identifier );

	// Add code to set the haptic properties
	//GetHapticProperties ( composite, string, identifier );

	// Recursively add shapes to the composite
	for ( int i= 0; i < composite->getShapeCount (); i++ )
	{
		// Create sub-identifier for sub-shape
		//CString subIdentifier;
		//subIdentifier.Format ( "subShape%d_%s", i, *identifier );

		CString stringA;
		GetInitCode ( composite->getShape ( i ), &stringA, &(composite->getShape(i)->GetName ()), true /*is a composite member*/ );
		*string+= stringA;

		*string+= "\r\n// Add sub-shape to composite";
		string->AppendFormat ( "\r\n%s->AddShape ( %s );", *identifier, (composite->getShape(i)->GetName ()) );
	}
}

// Get Init Code for a Triangle
void CShapeToAbstractModelCode::GetInitCodeTriangle ( CTriangle* triangle, CString *string, CString *identifier, bool compositeMember  )
{
	*string+= "\r\n// Initialise a Triangle Polygon object";

	// Instatiate the object
	string->AppendFormat ( "\r\n%s= new AbstractModel::CPolygon ( 3 );", *identifier );

	// Add vertex A
	string->AppendFormat ( "\r\n%s->AddVertex ( %f, %f, %f );", *identifier,
		triangle->GetVertexA()[0], triangle->GetVertexA()[1], triangle->GetVertexA()[2] );

	// Add vertex B
	string->AppendFormat ( "\r\n%s->AddVertex ( %f, %f, %f );", *identifier,
		triangle->GetVertexB()[0], triangle->GetVertexB()[1], triangle->GetVertexB()[2] );

	// Add vertex C
	string->AppendFormat ( "\r\n%s->AddVertex ( %f, %f, %f );", *identifier,
		triangle->GetVertexC()[0], triangle->GetVertexC()[1], triangle->GetVertexC()[2] );

	// Add code to set the shapes transform
	GetTransform ( triangle, string, identifier, compositeMember );

	// Add code to set the graphic properties
	GetGraphicProperties ( triangle, string, identifier );

	// Add code to set the haptic properties
	GetHapticProperties ( triangle, string, identifier );
}

// Get Init Code for a Memo
void CShapeToAbstractModelCode::GetInitCodeMemo ( CMemo* memo, CString *string, CString *identifier, bool compositeMember  )
{
	*string+= "\r\n// WARNING: Memo object not supported by AbstractModel translator!";
}

// Get Euler angles from rotation matrix
void CShapeToAbstractModelCode::GetEulerFromMatrix ( float* matrix, float *x, float *y, float *z )
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

// Get code to set the shapes transform
void CShapeToAbstractModelCode::GetTransform ( CShape* shape, CString *string /* in/out */, CString *identifier /* in */, bool compositeMember  )
{
	// Do in 3 transformation operations scaling->rotation->trnslation

	// Scaling
	CString scaleString;
	float scale[3];		// scaling factor
	
	// Unit Spheres in AM twice the size of those in PH 
	if ( shape->getType () == SHAPE_SPHERE )
	{
		scale[0]= shape->getSizeX () / 2.0f;
		scale[1]= shape->getSizeY () / 2.0f;
		scale[2]= shape->getSizeZ () / 2.0f;
	}
	else
	{
		scale[0]= shape->getSizeX ();
		scale[1]= shape->getSizeY ();
		scale[2]= shape->getSizeZ ();
	}

	// Apply additional scaling to comply with model scale
	float wholeModelScale= modelScale;
//	if ( compositeMember )
//	{
		// If composite member then scale is applied to 1st level composite
//		wholeModelScale= 1.0;
//	}

	// If the shape is not a triangle
	if ( shape->getType () != SHAPE_TRIANGLE )
	{
		// Apply the scale
		scaleString.Format ( "\r\n%s->Scale ( %f, %f, %f );", *identifier, scale[0]*wholeModelScale,
			scale[1]*wholeModelScale, scale[2]*wholeModelScale );
	}
	else
	{
		// No scale thanks, the scale is included in vertex positions

		// Only apply the model scale
		scaleString.Format ( "\r\n%s->Scale ( %f, %f, %f );", *identifier, wholeModelScale,
			wholeModelScale, wholeModelScale );
	}

	// Rotation
	CString rotateString1, rotateString2, rotateString3;
	float rotate[3];		// rotation about x, y, z
	
	// shapes rotation matrix
	float* rotationArray= shape->getRotation ();

	GetEulerFromMatrix ( rotationArray, &rotate[0], &rotate[1], &rotate[2] );

	rotateString1.Format ( "\r\n%s->Rotate ( 0, %f, 0 );", *identifier, -rotate[1] );
	rotateString2.Format ( "\r\n%s->Rotate ( 0, 0, %f );", *identifier, -rotate[0] );
	rotateString3.Format ( "\r\n%s->Rotate ( %f, 0, 0 );", *identifier, -rotate[2] );

	// Translation
	CString translateString;
	
	translateString.Format ( "\r\n%s->Translate ( %f, %f, %f );", *identifier, 
												shape->getLocationX ()*wholeModelScale,
												shape->getLocationY ()*wholeModelScale,
												shape->getLocationZ ()*wholeModelScale );

	// Add all strings together
	*string+= scaleString + rotateString1 + rotateString2 + rotateString3 + translateString;

/*	// Do in one SetTransform Operation

	// Build the transform to apply to the object
	hduMatrix transform;

	// Apply translation to the object
	hduMatrix translation;
	translation= hduMatrix::createTranslation ( shape->getLocationX (),
												shape->getLocationY (),
												shape->getLocationZ () );

	// Apply rotation to the object
	hduMatrix rotation;
	for ( int i= 0; i < 16; i++ )
	{
		int row= i/4;
		int col= i - row*4;

		rotation[row][col]= (double)shape->getRotation ()[i];

		char s[256];
		sprintf ( s, "%d,%d\n", row, col );
		OutputDebugString ( s );	
	}

	// Apply scaling to the object
	hduMatrix scaling;

	// Unit Spheres in AM twice the size of those in PH 
	if ( shape->getType () == SHAPE_SPHERE )
	{
		scaling= hduMatrix::createScale ( shape->getSizeX () / 2.0,
										  shape->getSizeY () / 2.0,
										  shape->getSizeZ () / 2.0 );
	}
	else
	{
		scaling= hduMatrix::createScale ( shape->getSizeX (),
										  shape->getSizeY (),
										  shape->getSizeZ () );
	}

	// Multiply the transforms together
	transform.multRight ( scaling );
	transform.multRight ( rotation );
	transform.multRight ( translation );

	// Create code to define a transformation matrix
	CString transformString;
	transformString.Format ( "\r\nfloat %s_transform[16]=\r\n   { %f, %f, %f, %f,\r\n     %f, %f, %f, %f,\r\n     %f, %f, %f, %f,\r\n     %f, %f, %f, %f };",
		(char*)identifier->GetBuffer (),
		transform[0][0],transform[0][1],transform[0][2],transform[0][3],
		transform[1][0],transform[1][1],transform[1][2],transform[1][3],
		transform[2][0],transform[2][1],transform[2][2],transform[2][3],
		transform[3][0],transform[3][1],transform[3][2],transform[3][3] );

	// Append this to out code string
	*string+= transformString;

	// Set the transform
	string->AppendFormat ( "\r\n%s->SetTransform ( %s_transform );", 
														*identifier,
														*identifier );
*/
}


/// Add code to set the graphic properties
void CShapeToAbstractModelCode::GetGraphicProperties ( CShape* shape, CString *string /* in/out */, CString *identifier /* in */ )
{
	// Set the color of the object
	string->AppendFormat ( "\r\n%s->SetColor ( %f, %f, %f );", *identifier, 
																shape->getRed (),
																shape->getGreen (),
																shape->getBlue () );
}

/// Add code to set the haptic properties
void CShapeToAbstractModelCode::GetHapticProperties ( CShape* shape, CString *string /* in/out */, CString *identifier /* in */ )
{
	// Set the stiffness of the object
	string->AppendFormat ( "\r\n%s->SetStiffness ( %f );", 
							*identifier, shape->getStiffness() );

	// Set the stiffness of the object
	string->AppendFormat ( "\r\n%s->SetStaticFriction ( %f );", 
		*identifier, shape->getStaticFriction () );

	// Set the stiffness of the object
	string->AppendFormat ( "\r\n%s->SetDynamicFriction ( %f );", 
		*identifier, shape->getDynamicFriction () );

	// Set the stiffness of the object
	string->AppendFormat ( "\r\n%s->SetDamping ( %f );", 
		*identifier, shape->getDampening () );
}